#pragma once

namespace crossover {
namespace monitor {
namespace client {
namespace os {

void set_process_count(unsigned int n);
void set_cpu_use_percent(float percent);
void set_used_memory(unsigned long long used_memory);
void set_total_memory(unsigned long long total_memory);
void set_total_disk_read(unsigned long long total_disk_read);
void set_total_disk_write(unsigned long long total_disk_write);

} //namespace os
} //namespace client
} //namespace monitor
} //namespace crossover
