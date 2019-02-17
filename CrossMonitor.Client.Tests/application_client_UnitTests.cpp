#include <gtest/gtest.h>

#include <application.hpp>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <string>
#include <chrono>
#include <process.h>
#include <Windows.h>
#include <data.hpp>
#include <os.hpp>
#include <os_mock.hpp>
#include <log.hpp>
#include <cpprest/json.h>

using namespace std;
using namespace crossover::monitor;
using namespace web;

#define LOG CROSSOVER_MONITOR_LOG

namespace crossover {
	namespace monitor {
		namespace client {

			TEST(CrossMonitorTest, ZeroArgumentIsNotAllowed) {
				ASSERT_THROW(client::application app{ chrono::seconds(0) }, std::invalid_argument);
			}

			TEST(CrossMonitorTest, NegativeArgIsNotAllowed) {
				ASSERT_THROW(client::application app{ chrono::seconds(-1) }, std::invalid_argument);
			}

			TEST(CrossMonitorTest, CreateDestroy) {
				ASSERT_NO_THROW(client::application app{ chrono::seconds(1) });
			}

			TEST(CrossMonitorTest, CreateRun) {
				ASSERT_NO_THROW(client::application app{ chrono::seconds(1) });
			}
			void SimpleRunThread(void *param) {
				client::application* app = (client::application*) param;
				app->run();
			}

			TEST(CrossMonitorTest, RunStop) {
				EXPECT_NO_THROW(os::set_cpu_use_percent(10));
				EXPECT_NO_THROW(os::set_process_count(50));
				EXPECT_NO_THROW(os::set_used_memory(100));
				EXPECT_NO_THROW(os::set_total_memory(101));
				EXPECT_NO_THROW(os::set_total_disk_read(102));
				EXPECT_NO_THROW(os::set_total_disk_write(103));
				client::application app(chrono::seconds(1));
			    data d = app.CollectData();
				ASSERT_EQ(d.get_cpu_percent(), 10);
				ASSERT_EQ(d.get_process_count(), 50);
				ASSERT_EQ(d.get_used_memory(), 100);
				ASSERT_EQ(d.get_total_memory(), 101);
				ASSERT_EQ(d.get_total_disk_read(), 102);
				ASSERT_EQ(d.get_total_disk_write(), 103);	
				ASSERT_EQ(app.period_, chrono::seconds(1));
				/* Call Run in a separated thread*/
			    HANDLE thr = (HANDLE)_beginthread(SimpleRunThread, 0, (void*) &app);
				/* Wait for Running flag to be set*/
				Sleep(10);
				EXPECT_NO_THROW(app.stop());
				WaitForSingleObject(thr, INFINITE);
			}

			TEST(CrossMonitorData, Create) {
				data d(100, 101, 102, 103, 104, 105);
				ASSERT_EQ(d.get_cpu_percent(), 100);
				ASSERT_EQ(d.get_used_memory(), 101);
				ASSERT_EQ(d.get_total_memory(), 102);
				ASSERT_EQ(d.get_process_count(), 103);
				ASSERT_EQ(d.get_total_disk_read(), 104);
				ASSERT_EQ(d.get_total_disk_write(), 105);
			}

			TEST(CrossMonitorData, SetFail) {
				data d(100, 101, 102, 103, 104, 105);
				ASSERT_THROW(d.set_cpu_percent(-1), std::invalid_argument);
				ASSERT_THROW(d.set_cpu_percent(100.1f), std::invalid_argument);
				ASSERT_THROW(d.set_cpu_percent(101), std::invalid_argument);
				ASSERT_THROW(d.set_process_count(0), std::invalid_argument);
				ASSERT_THROW(data d2(0,0,0,0,0,0), std::invalid_argument);
			}


			TEST(CrossMonitorClient, JsonData) {
				EXPECT_NO_THROW(os::set_cpu_use_percent(10));
				EXPECT_NO_THROW(os::set_process_count(50));
				EXPECT_NO_THROW(os::set_used_memory(100));
				EXPECT_NO_THROW(os::set_total_memory(101));
				EXPECT_NO_THROW(os::set_total_disk_read(102));
				EXPECT_NO_THROW(os::set_total_disk_write(103));
				client::application app(chrono::seconds(1));
				data d = app.CollectData();
				web::json::value j = app.data_to_json(d);
				web::json::value var=j.at(U("cpu_percent"));
				ASSERT_EQ(var.as_integer(), 10);
				var = j.at(U("process_count")); 
				ASSERT_EQ(var.as_integer(), 50);
				var = j.at(U("used_memory_in_bytes"));
				ASSERT_EQ(var.as_integer(), 100);
				var = j.at(U("total_memory_in_bytes"));
				ASSERT_EQ(var.as_integer(), 101);
				var = j.at(U("total_disk_read"));
				ASSERT_EQ(var.as_integer(), 102);
				var = j.at(U("total_disk_write"));
				ASSERT_EQ(var.as_integer(), 103);			
			}

			TEST(CrossMonitorOSMocks, GetOSParameters) {
				os::set_cpu_use_percent(10);
				ASSERT_EQ(os::cpu_use_percent(), 10);
				os::set_process_count(50);
				ASSERT_EQ(os::process_count(), 50);
				os::set_used_memory(100);
				ASSERT_EQ(os::used_memory(), 100);
				os::set_total_memory(101);
				ASSERT_EQ(os::total_memory(), 101);
				os::set_total_disk_read(102);
				ASSERT_EQ(os::total_disk_read(), 102);
				os::set_total_disk_write(103);
				ASSERT_EQ(os::total_disk_write(), 103);
			}

			int main(int argc, char* argv[]) {
				::testing::InitGoogleTest(&argc, argv);
				int val = RUN_ALL_TESTS();
				return val;
			}
		}
	}
}
