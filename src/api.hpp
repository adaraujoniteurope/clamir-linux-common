#ifndef _API_H_
#define _API_H_

#include <cstdint>
#include <thread>
#include <memory>
#include <mutex>
#include <vector>

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


class AbstractController
{
	public:
	virtual void loop() = 0;
};

template<typename T>
class AbstractParameter
{
	virtual T& get() { m_value; }
	virtual void set(T& value) { if (m_value != value) { m_value = value }; }

	AbstractParameter<T>& operator=(AbstractParameter<T>& self, T& other) {
		set(other);
		return self;
	}

	operator T&() {return get(); }
	operator T() {return get(); }
	T operator() { return get(); }
	
	private:
	T m_value;
};

template<typename T>
class ClassicController : AbstractController
{
	public:

	AbstractParameter<T> input;
	AbstractParameter<T> output;

	AbstractParameter<T> ki;
	AbstractParameter<T> kp;
	AbstractParameter<T> kd;

	AbstractParameter<T> ki_max;
	AbstractParameter<T> ki_min;

	AbstractParameter<T> kp_max;
	AbstractParameter<T> kp_min;

	AbstractParameter<T> kd_max;
	AbstractParameter<T> kd_min;

	AbstractParameter<T> input_max;
	AbstractParameter<T> input_min;

	AbstractParameter<T> error_max;
	AbstractParameter<T> error_min;

	AbstractParameter<T> error_diff_max;
	AbstractParameter<T> error_diff_min;

	AbstractParameter<T> error_sum_max;
	AbstractParameter<T> error_sum_min;

	AbstractParameter<T> output_max;
	AbstractParameter<T> output_min;

	void loop() {
		current_time = hw::get_current_time_seconds();
		error_last = error;
		error_diff = time_window * (error() - error_last);
		error_accumulated += error / time_window;
		output = error_accumulated * ki() + error * kp() + error_diff * kd();
	}

	protected:
	private:
};

namespace StateMachine
{
	class Engine;
	
	template<typename ReturnType(Args...)>
	class AbstractCallable
	{
		public:
		virtual ReturnType call(Args...args) = 0;
	};

	class StateGuard : public AbstractCallable<bool(Engine&, State&, State&)> {

		public:

		static const bool Valid = true;
		static const bool Invalid = false;

		virtual void set(std::function<bool(Engine&, State&, State&)>& guard_function) {
			m_function = guard_function;
		}

		virtual bool call(Engine& engine, State& current, State& next) {

			if (m_function != nullptr) {
				return m_function(engine, current, next);
			}

			return false;
		}

		private:

		std::function<bool(Engine&, State&, State&)> m_function;
	};

	class State {
		public:
		State() {}

		add_transition(State& state, StateGuard& guard) {
			transitions[state.id] = std::make_pair(state, guard);
		}

		remove_transition(State& state, StateGuard& guard) {

		}

		std::map<std::string, std::pair<State, StateGuard>> transitions;
	};

	class Engine {
		public:

		void poll()
		{

			for (auto& [next_state, guard] : current_state.transitions) {
				if (guard() == StateGuard::Valid) {
					current_state = next_state;
					break;
				}
			}

			current_state();

		}

		State& current_state;
		std::vector<State> states;
	};

	// The usage would be:

	void test_engine_usage() {

		State idle;
		State first;
		State second;

		StateGuard idle_to_first_guard([&](Engine&, State&, State&) -> bool {
			if (controller.output() > 0)
			{
				return true;
			}

			return false;
		});

		idle.add_transition(first, idle_to_first_guard);

		first.add_transition(idle, [&](Engine&, State&, State&) -> bool {
			if (controller.output() < 0)
			{
				return true;
			}

			return false;
		});

		Engine stateMachine;

		idle.add_transition(first, first);

		staeMachine.setInitialState(idle);

		for(;;) {
			stateMachine.poll();
		}
	}
};

class AbstractApplication
{
	public:
	virtual void init() = 0;
	virtual void loop() = 0;
};

#endif