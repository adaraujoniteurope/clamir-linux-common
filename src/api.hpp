#ifndef _API_H_
#define _API_H_

#include <cstdint>
#include <thread>
#include <memory>
#include <mutex>

class AbstractController
{
	public:
	virtual void compute() = 0;
};

class PIDController : public AbstractController
{
	public:
	virtual void compute() override {}
};

class AbstractMemoryMappingController
{
	public:
	virtual void open(size_t address, size_t size, size_t offset) = 0;
	virtual void close() = 0;
	virtual void read(const void* buffer, size_t size, size_t offset = 0) = 0;
	virtual void write(const void* buffer, size_t size, size_t offset = 0) = 0;

	protected:
	size_t m_address;
	volatile void* m_mapping;
};

class SharedMemoryMapping : public AbstractMemoryMappingController
{
	public:

	virtual void open(size_t address, size_t size, size_t offset) override {

		if (m_path == nullptr)
		{
			m_path = "/dev/mem";
		}

		// m_memory_map_fd = ::open(m_path, O_RDWR);
		// m_memory_map = ::mmap();

		if (m_memory_map == nullptr) {}
	}

	virtual void close() override {
		std::unique_lock<std::mutex> lk(m_memory_map_mutex);
		if (m_memory_map == nullptr) {
			// log::warn() << "memory map at " << m_path << "isn't open" << std::endl;
			return;
		}
	}

	virtual void write(const void* buffer, size_t size, size_t offset = 0) override {}
	
	virtual void read(const void* buffer, size_t size, size_t offset = 0) override {}

	private:
	const char* m_path;
	size_t m_address;
	size_t m_length;
	size_t m_offset;

	std::mutex m_memory_map_mutex;
	int m_memory_map_fd;
	void* m_memory_map;
};

template<typename MappingInterface>
class MemoryMappedDevice
{
	public:
	private:
};

template<typename MappingInterface>
class ControlUnit : public MemoryMappedDevice<MappingInterface>
{
	public:
	ControlUnit(MappingInterface& interface) : m_interface(interface) {

	}

	template<typename T>
	T read_register(size_t offset) {}
	template<typename T>
	void write_register(size_t offset, T value) {
		if (!m_interface.is_open()) {
			m_interface.open();
		}
	}
	private:
	const MappingInterface& m_interface;
};

template<typename MappingInterface>
class ImageBuffer : public MemoryMappedDevice<MappingInterface>
{
	public:
	uint16_t* get_image() {}
	private:
};

template<typename MappingInterface>
class ImageMetadata : public MemoryMappedDevice<MappingInterface>
{
	public:
	private:
};

template<typename MappingInterface>
class MomentsCore : public MemoryMappedDevice<MappingInterface>
{
	public:
	private:
};

template<typename MappingInterface>
class ProcessorCore : public MemoryMappedDevice<MappingInterface>
{
	public:
	private:
};

class AbstractApplication
{
	public:
	virtual void init() = 0;
	virtual void loop() = 0;
};

#endif