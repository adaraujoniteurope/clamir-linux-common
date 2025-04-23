#include <nit/embedded/drivers/framebuffer_core.h>
#include <nit/embedded/utils/config_file.h>
#include <nit/embedded/utils/memory_map.hpp>
#include <nit/embedded/vision/frame_generator.hpp>
#include <nit/embedded/math/algorithm.hpp>

#include <sys/fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

typedef struct metadata_wrapper_struct
{

} metadata_wrapper_t;

typedef struct nit_framebuffer_core_state_priv_struct
{
    volatile int* metadata;
    volatile int16_t* frame;
} nit_framebuffer_core_state_priv_t;

const nit_framebuffer_core_config_t framebuffer_core_config_default = {
    // TBD
};

#include <random>

template<typename type>
class sensor
{
    public:
    static void gain_table_generate(type* buffer, double dispersion, size_t width, size_t height)
    {
        std::random_device random_device{};
        std::mt19937 random_number_generator{random_device()};
        std::normal_distribution normal_distribution{1.0, dispersion};

        for (size_t row = 0; row < width; row++) {
            for (size_t col = 0; col < height; col++) {
                auto gain = (INT16_MAX >> 1) * normal_distribution(random_number_generator);
                buffer[row * width + col] = gain;
            }
        }
    }

    static void gain_table_apply(type* buffer, type* table, size_t width, size_t height) {

        for (size_t row = 0; row < width; row++) {
            for (size_t col = 0; col < height; col++) {
                auto idx = row * width + col;
                buffer[idx] = (int16_t) (double) buffer[idx] * ((double) table[idx] / (double) (INT16_MAX >> 1));
            }
        }
    }

    static void bias_apply(type* buffer, type bias, size_t width, size_t height) {

        for (size_t row = 0; row < width; row++) {
            for (size_t col = 0; col < height; col++) {
                auto idx = row * width + col;
                buffer[idx] = buffer[idx] + bias;
            }
        }
    }

    static void generate_circle(type* buffer, size_t x = 32, size_t y = 32, size_t radius = 10, size_t width = 64, size_t height = 64, type low = 500, type high = 2000)
    {
        for (size_t row = 0; row < height; row++) {
            for (size_t col = 0; col < width; col++) {
                double d = std::sqrt( std::pow((double)col - (double)x, 2) + std::pow((double)row - (double)y, 2));
                size_t idx = row * width + col;
                if (d < radius) {
                    buffer[idx] = high;
                } else {
                    buffer[idx] = low;
                }
            }
        }
    }

    static void generate_uniform(type* buffer, type value = 500, size_t width = 64, size_t height = 64)
    {
        for (size_t row = 0; row < height; row++) {
            for (size_t col = 0; col < width; col++) {
                size_t idx = row * width + col;
                buffer[idx] = value;
            }
        }
    }

};

nit_framebuffer_core_state_priv_t* nit_framebuffer_core_private_state_get(nit_framebuffer_core_state_t* state);
int nit_framebuffer_core_loop(nit_framebuffer_core_state_t* state);

nit_framebuffer_core_state_t nit_framebuffer_core_driver;

using namespace utils;

int nit_framebuffer_core_open(nit_framebuffer_core_state_t* state, nit_framebuffer_core_config_t* config)
{

    if (state == NULL)
    {
        return -1;
    }

    if (state->is_open == true)
    {
        return 0;
    }

    state->is_open = false;
    state->priv = nullptr;

    state->priv = (nit_framebuffer_core_state_priv_t*) malloc(sizeof(nit_framebuffer_core_state_priv_t));

    if (state->priv == NULL) {
        return -1;
    }

    auto priv = nit_framebuffer_core_private_state_get(state);

    state->fd = -1;
    state->fd = open("/dev/mem", O_RDWR | O_SYNC);

    priv->frame = (int16_t*) memory_map_open(NULL, NIT_FRAMEBUFFER_CORE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, state->fd, NIT_FRAMEBUFFER_CORE_BASE_ADDRESS);
    priv->metadata = (int*) ((uint8_t*)priv->frame + 8192);

    if (state->priv == NULL)
    {
        close(state->fd);
        return -3;
    }

    state->is_open = true;
    nit_framebuffer_core_operating_mode_set(state, config->operating_mode);

    return 0;
}

int nit_framebuffer_core_close(nit_framebuffer_core_state_t* state)
{
    if (state == NULL)
    {
        return -1;
    }

    state->is_open = false;
    if (state->priv != NULL) {
        memory_map_close((void*)state->priv, NIT_FRAMEBUFFER_CORE_SIZE);
    }

    if (state->fd >= 0)
    {
        close(state->fd);
    }

    return 0;
}

int nit_framebuffer_core_assert(nit_framebuffer_core_state_t *state)
{

    if (state == NULL)
    {
        return -1;
    }

    if (state->priv == NULL)
    {
        return -2;
    }

    if (state->is_open == false)
    {
        return -3;
    }

    return 0;
}

int nit_framebuffer_core_config_save_to_file(nit_framebuffer_core_state_t* state, const char* path)
{
    return 0;
}

int nit_framebuffer_core_config_load_from_file(nit_framebuffer_core_state_t* state, const char* path)
{
    auto retval = config_file_load_from_file(state, path);

    if (retval < 0) {
        return retval;
    }

    return retval;
}

int nit_framebuffer_core_state_assert(nit_framebuffer_core_state_t* state)
{

    if (state == NULL) {
        return -1;
    }

    if (state->priv == NULL) {
        return -2;
    }

    if (state->is_open == false) {
        return -3;
    }

    return 0;
}

nit_framebuffer_core_state_priv_t* nit_framebuffer_core_private_state_get(nit_framebuffer_core_state_t* state) {
    return (nit_framebuffer_core_state_priv_t*)(state->priv);
}

volatile int16_t* nit_framebuffer_core_get_memory_map(nit_framebuffer_core_state_t* state)
{

    if (nit_framebuffer_core_state_assert(state) < 0) {
        return nullptr;
    }

    return (int16_t*)nit_framebuffer_core_private_state_get(state)->frame;
}


int* nit_framebuffer_core_metadata(nit_framebuffer_core_state_t* state)
{

    if (nit_framebuffer_core_state_assert(state) < 0) {
        return nullptr;
    }

    return (int*)nit_framebuffer_core_private_state_get(state)->metadata;
}

#include <thread>


int nit_framebuffer_core_run(nit_framebuffer_core_state_t* state, std::shared_ptr<utils::waitable> timer, std::atomic_bool& shutdown)
{

    while(!shutdown) {
        // timer->wait();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        nit_framebuffer_core_loop(state);
    }

    return 0;
}

int nit_framebuffer_core_loop(nit_framebuffer_core_state_t* state)
{
    static int16_t x = 32;
    static int16_t y = 32;
    static int16_t radius = 10;
    static int16_t x_increment = 1;
    static int16_t y_increment = 1;
    static int16_t radius_increment = 1;

    static int32_t power = 6;
    static int32_t track_number = 1;
    static int32_t frame_max = 2;
    static int32_t frame_number = 3;
    static int32_t timestamp = 4;
    static int32_t io_status = 5;

    static int16_t gain_table[4192];
    static bool initialized = false;

    if (!initialized) {
        sensor<int16_t>::gain_table_generate(gain_table, 1.0e-1, 64, 64);
        initialized = true;
    }

    static auto last = std::chrono::high_resolution_clock::now();

    auto frame = nit_framebuffer_core_private_state_get(state)->frame;
    auto metadata = nit_framebuffer_core_private_state_get(state)->metadata;

    if ((std::chrono::high_resolution_clock::now() - last) > std::chrono::milliseconds(1)) {

        if (x > (64 - radius / 2) || x < ( 0 + radius / 2)) {

            if (x_increment > 0) {
                x_increment = -1;
            } else {
                x_increment = 1;
            }
        }
    
        if (y > (64 - radius / 2) || y < ( 0 + radius / 2 )) {
            if (y_increment > 0)
            {
                y_increment = -1;
            } else {
                y_increment = 1;
            }
        }

        if (radius > 10) {
            radius_increment = -1;
        }

        if (radius < 5) {
            radius_increment = 1;
        }

        x += x_increment;
        y += y_increment;
        radius += radius_increment;

        nit_framebuffer_core_operating_mode_t operating_mode;
        nit_framebuffer_core_operating_mode_get(state, (uint16_t*)&operating_mode);

        switch(operating_mode) {

            case NIT_FRAMEBUFFER_CORE_OPERATING_MODE_TEST_PATTERN_BALL:
            sensor<int16_t>::generate_circle((int16_t*)frame, x, y, radius, 64, 64, 1000, 2000);
            break;

            case NIT_FRAMEBUFFER_CORE_OPERATING_MODE_TEST_UNIFORM_SHUTTER_OPEN:
            sensor<int16_t>::generate_uniform((int16_t*)frame, 2000, 64, 64);
            break;

            case NIT_FRAMEBUFFER_CORE_OPERATING_MODE_TEST_UNIFORM_SHUTTER_CLOSED:
            sensor<int16_t>::generate_uniform((int16_t*)frame, 100, 64, 64);
            break;

            default:
            break;
        }

        sensor<int16_t>::gain_table_apply((int16_t*)frame, gain_table, 64, 64);
        last = std::chrono::high_resolution_clock::now();
        track_number += 1;
        frame_max += 1;
        frame_number += 1;
        timestamp += 1;
    }

    auto m00 = math::algorithm::moments::moments<int16_t>((int16_t*)frame, 64, 64, 0, 0);
    auto m01 = math::algorithm::moments::moments<int16_t>((int16_t*)frame, 64, 64, 1, 0);
    auto m10 = math::algorithm::moments::moments<int16_t>((int16_t*)frame, 64, 64, 0, 1);
    auto m11 = math::algorithm::moments::moments<int16_t>((int16_t*)frame, 64, 64, 1, 1);
    auto m02 = math::algorithm::moments::moments<int16_t>((int16_t*)frame, 64, 64, 2, 0);
    auto m20 = math::algorithm::moments::moments<int16_t>((int16_t*)frame, 64, 64, 0, 2);

    metadata[0] = power;
    metadata[1] = m00;
    metadata[2] = m01;
    metadata[3] = m10;
    metadata[4] = m11;
    metadata[5] = m02;
    metadata[6] = m20;

    // Track Nmbr, Frame Max, Frame Number, Timestamp, IO Status
    // metadata[7] = track_number;
    metadata[8] = frame_number;
    metadata[9] = frame_number;
    metadata[10] = timestamp;
    metadata[11] = io_status;

    return 0;
}

int nit_framebuffer_core_operating_mode_get(nit_framebuffer_core_state_t *state, uint16_t *value)
{
    int retval = 0;

    if ((retval = nit_framebuffer_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    *value = state->config.operating_mode;

    return retval;
}

int nit_framebuffer_core_operating_mode_set(nit_framebuffer_core_state_t *state, uint16_t value)
{
    print_debug("%s: (0x%04x): %d\n", __func__, nit_framebuffer_core_operating_mode_offset, value);

    int retval = 0;

    if ((retval = nit_framebuffer_core_assert(state)) != 0)
    {
        print_debug("%s: %s %d\n", __func__, "failed", retval);
        return retval;
    }

    state->config.operating_mode = value;
    return retval;
}