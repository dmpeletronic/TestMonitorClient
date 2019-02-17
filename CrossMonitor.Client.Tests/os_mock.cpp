#include <os.hpp>

namespace crossover {
namespace monitor {
namespace client {
namespace os {

// Mock values
unsigned int _process_count = 0;
float _cpu_use_percent = 0;
float _memory_use_percent = 0;
unsigned long long used_memory_ = 0;
unsigned long long total_memory_ = 0;
unsigned long long total_disk_read_ = 0;
unsigned long long total_disk_write_ = 0;

void set_process_count(unsigned int n) {
	_process_count = n;
}

unsigned process_count() noexcept {
	return _process_count;
}

void set_cpu_use_percent(float percent) {
	_cpu_use_percent = percent;
}

float cpu_use_percent() noexcept {
	return _cpu_use_percent;
}

void set_memory_use_percent(float percent) {
	_memory_use_percent = percent;
}

float memory_use_percent() noexcept {
	return _memory_use_percent;
}

void set_used_memory(unsigned long long used_memory) {
	used_memory_ = used_memory;
}

void set_total_memory(unsigned long long total_memory) {
	total_memory_ = total_memory;
}

void set_total_disk_read(unsigned long long total_disk_read) {
	total_disk_read_ = total_disk_read;
}

void set_total_disk_write(unsigned long long total_disk_write) {
	total_disk_write_ = total_disk_write;
}

/*
* Get total physical memory available
*/
unsigned long long total_memory() noexcept {
	return total_memory_;
}
/*
* Get total physical memory available
*/
unsigned long long used_memory() noexcept {
	return used_memory_;
}
/*
* Get total physical disk read in bytes
*/
unsigned long long total_disk_read() noexcept {
	return total_disk_read_;
}
/*
* Get total physical disk write in bytes
*/
unsigned long long total_disk_write() noexcept {
	return total_disk_write_;
}

} //namespace os
} //namespace client
} //namespace monitor
} //namespace crossover