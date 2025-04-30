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

    template<class instance>
    class memory_mapped_device : std::enable_shared_from_this<instance>
    {
        protected:
        memory_mapped_device() {}
        memory_mapped_device(size_t offset, size_t size, std::string path = "/dev/mem", std::shared_ptr<memory_mapped_device> base = nullptr)
            : m_offset(offset), m_size(size), m_path(path)
        {}

        public:

        virtual ~memory_mapped_device()
        {
            close();
        }

        virtual int open() {
            std::unique_lock<std::mutex> lk(m_mutex);

            if(m_base != nullptr)
            {
                m_priv = (volatile uint8_t*) m_base->priv();
                m_is_open = true;
                return 0;
            }

            if ((m_fd = ::open(m_path.c_str(), O_RDWR | O_SYNC)) < 0) {
                throw std::runtime_error(std::format("failed to open memory device at {}", m_path));
            }

            unsigned int pagesize = (unsigned)sysconf(_SC_PAGESIZE);

            m_size = pagesize*(m_size/pagesize);

            if ((m_priv = (volatile uint8_t*) mmap(NULL, m_size, PROT_READ | PROT_WRITE, MAP_SHARED, m_fd, m_offset)) == (uint8_t*) -1) {
                throw std::runtime_error(std::format("failed to open memory map at {} with size {} at {}", m_offset, m_size, m_path));
            }

            this->m_is_open = true;

            return 0;
        }

        virtual int close() {

            std::unique_lock<std::mutex> lk(m_mutex);

            if (m_priv != nullptr && m_priv != (uint8_t*) -1) {
                munmap((void*)m_priv, m_size);
            }

            if (m_fd >= 0) {
                ::close(m_fd);
            }

            this->m_is_open = false;

            return 0;
        }

        virtual bool is_open() { return m_is_open; }

        virtual void* priv() {
            std::unique_lock<std::mutex> lk(m_mutex);
            return (void*) m_priv;
        }

        const size_t& offset() { return m_offset; }
        const size_t& size() { return m_size; }

        template<typename type>
        type& read(size_t offset)
        {
            std::unique_lock<std::mutex> lk(m_mutex);

            if (!is_open()) {
                throw std::runtime_error("driver is not open!");
            }

            if (offset > m_offset) {
                throw std::runtime_error(std::format("given offset {} is bigger than device address range plus variable size {}", offset, m_offset + sizeof(type)));
            }

            return ((type*)m_priv)[offset];
        }

        template<typename type>
        void write(size_t offset, type value)
        {
            std::unique_lock<std::mutex> lk(m_mutex);

            if (!is_open()) {
                throw std::runtime_error("driver is not open!");
            }

            ((type*)m_priv)[offset] = value;
        }

        template<typename...args_types>
        static std::shared_ptr<instance> create(args_types...args)
        {
            return std::make_shared<instance>(std::forward<args_types>(args)...);
        }

        protected:

        std::mutex m_mutex;
        std::shared_ptr<memory_mapped_device> m_base;

        size_t m_offset;
        size_t m_size;
        std::string m_path;

        bool m_is_open = false;
        int m_fd = -1;
        volatile uint8_t* m_priv = nullptr;
    };
};

#endif