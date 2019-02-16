#include <gtest/gtest.h>
#include <application.hpp>
#include <cstdlib>
#include <stdexcept>
#include <iostream>
#include <string>
#include <chrono>


using namespace std;
using namespace crossover::monitor;

namespace crossover {
	namespace monitor {
		namespace client {

			TEST(CrossMonitorTest, ZeroArgumentIsNotAllowed) {
				ASSERT_THROW(client::application app{ chrono::seconds(0) }, std::invalid_argument);
			}

			int main(int argc, char* argv[]) {
				::testing::InitGoogleTest(&argc, argv);
				int val = RUN_ALL_TESTS();
				return val;

			}
		}
	}
}
