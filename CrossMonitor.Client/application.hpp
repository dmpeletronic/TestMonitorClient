#pragma once

#include <boost/noncopyable.hpp>

#include <memory>
#include <string>
#include <chrono>
#include <data.hpp>
#include <cpprest/json.h>

using namespace web;

namespace crossover {
namespace monitor {
namespace client {

/**
 * Class handling main application logic.
 * Call run() after construction to run main logic.
 */
class application final: public boost::noncopyable {
public:
	/**
	 * Constructs a ready to use application object.
	 * May throw std::exception derived exceptions.
	 * @param period minutes between reports.
	 */
	application(const std::chrono::seconds& period);
	~application();

	/**
	 * Runs the application logic. Blocking.
	 * Call stop() from any thread or signal handler to break from this
	 * call.
	 * May throw std::exception derived classes.
	 */
	void run();
	/**
	 * Call this from any thread or signal handler 
	 * to stop executing after using run().
	 */
	void stop() noexcept;

private:
	friend class CrossMonitorTest_RunStop_Test;
	friend class CrossMonitorClient_JsonData_Test;
	data CollectData();
	web::json::value data_to_json(const data& data) noexcept;


	struct impl;

	std::unique_ptr<impl> pimpl_;
	const std::chrono::seconds period_;
}; //class application

} //namespace client
} //namespace monitor
} //namespace crossover
