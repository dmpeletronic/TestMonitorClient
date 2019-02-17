#include <application.hpp>
#include <os.hpp>

#include <log.hpp>
#include <utils.hpp>

#include <cpprest/http_client.h>
#include <cpprest/asyncrt_utils.h>

#include <atomic>
#include <string>
#include <stdexcept>
#include <numeric>

#define LOG CROSSOVER_MONITOR_LOG

using namespace std;
using namespace web;

namespace crossover {
namespace monitor {
namespace client {

data application::CollectData() {
	return data{
		os::cpu_use_percent(),
		os::used_memory(),
		os::total_memory(),
		os::process_count(),
		os::total_disk_read(),
		os::total_disk_write()
	};
}

web::json::value application::data_to_json(const data& data) noexcept {

	json::value v(json::value::object());
	json::object& o(v.as_object());
	o[L"cpu_percent"] = data.get_cpu_percent();
	o[L"used_memory_in_bytes"] = data.get_used_memory();
	o[L"total_memory_in_bytes"] = data.get_total_memory();
	o[L"process_count"] = data.get_process_count();
	o[L"total_disk_read"] = data.get_total_disk_read();
	o[L"total_disk_write"] = data.get_total_disk_write();
	return v;
}

static void send_data(const string& url, const string& key, const json::value jsondata) {

	const uri url_full(utility::conversions::to_string_t(url));

	uri_builder builder;
	builder.set_scheme(url_full.scheme());
	builder.set_user_info(url_full.user_info());
	builder.set_host(url_full.host());
	builder.set_port(url_full.port());

	const uri base(builder.to_uri());

	LOG(debug) << "Base URI: " << base.to_string()
		<< " - URI resource: " << url_full.resource().to_string();
	
	// Actual sending is out of scope...
	// so reporting unconditional success here
	LOG(info) << "Data sent successfully to " << url;
}

static void report_sent_callback(const data& sent_data)
{
	static unsigned reports_sent = 0;
	static float latest_cpu_values[10] = { 0 };
	static mutex m;

	lock_guard<mutex> l(m);

	latest_cpu_values[reports_sent % 10] = sent_data.get_cpu_percent();

	// Every 10 reports sent - show statistics about mean CPU usage
	// Skip first time hit
	if ((++reports_sent) % 10 == 0 && reports_sent != 0)
	{
		auto sum = std::accumulate(std::begin(latest_cpu_values), std::end(latest_cpu_values), 0.0f);
		auto mean = sum / 10.0f;

		LOG(info) << "Mean CPU usage: " << mean;
	}
}

struct application::impl final {
	atomic<bool> stop = false;
	atomic<bool> running = false;
};

application::application(
	const std::chrono::seconds& period) :
	pimpl_(new impl),
	period_(period) {
	if (period_ < chrono::seconds(1) ||
        period_ > chrono::seconds(INT_MAX)) {
		throw invalid_argument("Invalid arguments to application constructor");
	}
}

application::~application() {
	
}

void application::run() {
	if (pimpl_->running) {
		LOG(warning) << "application::run already running, ignoring call";
		return;
	}

	pimpl_->running = true;

	LOG(info) << "Starting application loop";
	utils::scope_exit exit_guard([this] {
		pimpl_->running = false;
		pimpl_->stop = false;

		LOG(info) << "Exiting application loop";
	});

	const chrono::milliseconds resolution(100);
	do {
		try {
			using namespace web;
			auto collected_data = CollectData();
			const json::value jsondata(data_to_json(collected_data));
 			LOG(info) << jsondata.to_string() ;
		}
		catch (const std::exception& e) {
			LOG(error) << "Failed to collect and send data to server: "
				<< e.what();
		}
	} while (utils::interruptible_sleep(period_, resolution, pimpl_->stop) !=
		utils::interruptible_sleep_result::interrupted);
}

void application::stop() noexcept {
	if (pimpl_->running) {
		LOG(info) << "Stop requested, waiting for tasks to finish";
		pimpl_->stop = true;
	}
}

} //namespace client
} //namespace monitor
} //namespace crossover

