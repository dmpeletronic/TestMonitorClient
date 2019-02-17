#include <utils.hpp>

#include <thread>

using namespace std;

namespace crossover {
namespace monitor {
namespace utils {

interruptible_sleep_result
interruptible_sleep(const std::chrono::milliseconds& time,
	const std::chrono::milliseconds& check_period,
	const std::atomic<bool>& interrupt) noexcept
{
	this_thread::yield();
	if (interrupt.load())
		return interruptible_sleep_result::interrupted;
	else
		return interruptible_sleep_result::timeout;
}

} //namespace utils
} //namespace monitor
} //namespace crossover
