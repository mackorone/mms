#ifndef SIM_METHODS_H_
#define SIM_METHODS_H_

#include <chrono>
#include <thread>

namespace sim{

void sleep(int millis){
	 std::this_thread::sleep_for(std::chrono::milliseconds(millis));
	 return;
}

}

#endif // SIM_METHODS_H_
