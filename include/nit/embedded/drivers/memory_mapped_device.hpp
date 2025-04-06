#ifndef _nit_embedded_drivers_memory_mapped_device_hpp_
#define _nit_embedded_drivers_memory_mapped_device_hpp_

#include <string>
#include <format>
#include <memory>
#include <cstdbool>
#include <mutex>

#include <sys/fcntl.h>
#include <sys/mman.h>

#include <boost/serialization/nvp.hpp>

#define MEMORY_MAPPED_DRIVER_DECLARE_GETTER(name, type, offset) \
type name##_get() \
{ \
    std::unique_lock<std::mutex> lk(m_mutex); \
    return *((type*)(m_priv+offset)); \
} \

#define MEMORY_MAPPED_DRIVER_DECLARE_SETTER(name, type, offset) \
void name##_set(const type& value) \
{ \
    std::unique_lock<std::mutex> lk(m_mutex); \
    *((type*)(m_priv+offset)) = value; \
}


#define MEMORY_MAPPED_DRIVER_DECLARE_ACCESSORS(name, type, offset) \
MEMORY_MAPPED_DRIVER_DECLARE_GETTER(name,type,offset) \
MEMORY_MAPPED_DRIVER_DECLARE_SETTER(name,type,offset)

namespace nit::embedded::drivers
{

    enum class io_value : uint8_t
    {
        high,
        low
    };

    template<typename type>
    struct is_memory_mapeed_config_type
    {
        static const bool value = false;
    };

    struct config_base
    {
        std::string path = "/dev/mem";
        size_t offset = 0x00000000;
        size_t size = 0x00000000;

        template<typename archiver>
        void serialize(archiver& ar, const unsigned int version)
        {
            ar & boost::serialization::make_nvp("path", path);
            ar & boost::serialization::make_nvp("offset", offset);
            ar & boost::serialization::make_nvp("size", size);
        }

        template<typename archiver>
        void load(archiver& ar, const unsigned int version)
        {
            ar & boost::serialization::make_nvp("path", path);
            ar & boost::serialization::make_nvp("offset", offset);
            ar & boost::serialization::make_nvp("size", size);
        }
    };

    template<>
    struct is_memory_mapeed_config_type<config_base>
    {
        static const bool value = true;
    };

    template<class instance, typename config_type = config_base>
    class memory_mapped_device : std::enable_shared_from_this<instance>
    {
        protected:
        memory_mapped_device() {}

        public:

        virtual ~memory_mapped_device()
        {
            close();
        }

        virtual int open() {
            std::unique_lock<std::mutex> lk(m_mutex);

            auto _config = static_cast<config_base>(m_config);

            if ((this->m_fd = ::open(_config.path.c_str(), O_RDWR | O_SYNC)) < 0) {
                throw std::runtime_error(std::format("failed to open memory device at {}", this->m_config.path));
            }

            if ((m_priv = (volatile uint8_t*) mmap(NULL, this->m_config.size, PROT_READ | PROT_WRITE, MAP_SHARED, this->m_fd, this->m_config.offset)) == (uint8_t*) -1) {
                throw std::runtime_error(std::format("failed to open memory map at {} with size {} at {}", this->m_config.offset, this->m_config.size, this->m_config.path));
            }

            this->m_is_open = true;
        }

        virtual int close() {

            std::unique_lock<std::mutex> lk(m_mutex);

            if (m_priv != nullptr && m_priv != (uint8_t*) -1) {
                munmap((void*)m_priv, m_config.size);
            }

            if (m_fd >= 0) {
                ::close(m_fd);
            }

            this->m_is_open = false;

            return 0;
        }

        virtual bool is_open() { std::unique_lock<std::mutex> lk(m_mutex); return this->m_is_open; }

        virtual void* priv() {
            std::unique_lock<std::mutex> lk(m_mutex);
            return (void*) m_priv;
        }

        virtual const config_type& config()
        {
            return m_config;
        }

        template<typename type>
        type& read(size_t offset)
        {
            std::unique_lock<std::mutex> lk(m_mutex);

            if (!is_open()) {
                throw std::runtime_error("driver is not open!");
            }

            if (offset > this->m_config.offset) {
                throw std::runtime_error(std::format("given offset {} is bigger than device address range plus variable size {}", offset, this->m_config.offset + sizeof(type)));
            }

            return *((type*)(m_priv + offset));
        }

        template<typename type>
        void write(size_t offset, type value)
        {
            std::unique_lock<std::mutex> lk(m_mutex);

            if (!is_open()) {
                throw std::runtime_error("driver is not open!");
            }

            *((type*)(m_priv + offset)) = value;
        }

        template<typename...args_types>
        static std::shared_ptr<instance> create(args_types...args)
        {
            return std::make_shared<instance>(std::forward<args_types>(args)...);
        }

        protected:

        std::mutex m_mutex;
        std::shared_ptr<memory_mapped_device> m_base;
        config_type m_config;
        bool m_is_open = false;
        int m_fd = -1;
        volatile uint8_t* m_priv = nullptr;
    };
};

#endif