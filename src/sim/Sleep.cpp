#include "Sleep.h"

#include <chrono>
#include <thread>

namespace sim {

void sleep(int millis) {
	 std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

} // namespace sim
