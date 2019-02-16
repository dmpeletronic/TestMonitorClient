#pragma once

#include <stdexcept>
#include <string>

namespace crossover {
namespace monitor {

/**
 * Class representing the data sent and received 
 * by both client and server components.
 */
class data final {
public:
	/**
	 * Constructor. Throws std::invalid_argument in case any arguments
	 * are out of range.
	 * @param cpu_percent CPU use percentage (0 to 100).
	 * @param process_count Number of processes (1 to UINT_MAX).
	 * @param total_memory Total available memory in bytes (0 to UINT_MAX)
	 * @param used_memory Used memory in bytes (0 to UINT_MAX)
	 */
	data(float cpu_percent,
		unsigned long long used_memory,
		unsigned long long total_memory,
		unsigned process_count,
		unsigned long long total_disk_read,
		unsigned long long total_disk_write ) {
		set_cpu_percent(cpu_percent);
		set_used_memory(used_memory);
		set_total_memory(total_memory);
		set_process_count(process_count);
		set_total_disk_read(total_disk_read);
		set_total_disk_write(total_disk_write);
	}
	data(const data& other) = default;
	data& operator=(const data& rhs) = default;	

	/**
	 * Setter. Throws std::invalid_argument if the argument is out of range.
	 * @param cpu_percent CPU use percentage (0 to 100).
	 */
	void set_cpu_percent(float cpu_percent) {
		if (cpu_percent < 0 || cpu_percent > 100) {
			throw std::invalid_argument(
				"cpu_percent out of range: " + std::to_string(cpu_percent));
		}
		cpu_percent_ = cpu_percent;
	}
	float get_cpu_percent() const noexcept {
		return cpu_percent_;
	}

	/**
	* Setter. Throws std::invalid_argument if the argument is out of range.
	* @param process_count Process count (1 to UINT_MAX).
	*/
	void set_process_count(unsigned process_count) {
		if (process_count == 0) {
			throw std::invalid_argument("process_count cannot be zero");
		}
		process_count_ = process_count;
	}
	unsigned get_process_count() const noexcept {
		return process_count_;
	}

	/**
	* Setter.
	* @param total_memory Total memory in bytes (0 to UINT_MAX).
	*/
	void set_total_memory(unsigned long long total_memory) {
		total_memory_ = total_memory;
	}
	unsigned long long get_total_memory() const noexcept {
		return total_memory_;
	}

	/**
	* Setter.
	* @param total_memory Used memory in bytes (0 to UINT_MAX).
	*/
	void set_used_memory(unsigned long long used_memory) {
		used_memory_ = used_memory;
	}
	unsigned long long get_used_memory() const noexcept {
		return used_memory_;
	}

	/**
	* Setter.
	* @param total_disk_read Total read bytes from disk
	*/
	void set_total_disk_read(unsigned long long total_disk_read) {
		total_disk_read_ = total_disk_read;
	}
	unsigned long long get_total_disk_read() const noexcept {
		return total_disk_read_;
	}

	/**
	* Setter.
	* @param total_disk_write Total write bytes to disk.
	*/
	void set_total_disk_write(unsigned long long total_disk_write) {
		total_disk_write_ = total_disk_write;
	}
	unsigned long long get_total_disk_write() const noexcept {
		return total_disk_write_;
	}

private:
	float cpu_percent_;
	unsigned long long used_memory_;
	unsigned long long total_memory_;
	unsigned process_count_;
	unsigned long long total_disk_read_;
	unsigned long long total_disk_write_;
}; //struct data

} //namespace monitor
} //namespace crossover
