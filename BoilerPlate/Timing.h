#pragma once
//Class to handle timings, modelled after the OpenGL convention of "buffer" creation

#include <chrono>
#include <vector>

namespace Timing {
	typedef unsigned int uint;
	//Generate #count number of timer IDs
	void genTimingID(uint *buffer, uint count);

	//Get time since function was last called, in resolution of some chrono type
	template <typename TimeType>
	TimeType getTimeDiff(unsigned int id) {
		using namespace std::chrono;
		std::vector<high_resolution_clock::time_point> *timestamps = getTimingList();
		high_resolution_clock::time_point current_time = high_resolution_clock::now();
		TimeType duration = duration_cast<TimeType>(current_time - timestamps->at(id));
		timestamps->at(id) = current_time;
		return duration;
	}
	
	//Initialise timer with current time
	std::chrono::steady_clock::time_point start(uint id);

	//Get the current system time
	std::chrono::steady_clock::time_point getCurrentTime();
	std::vector<std::chrono::high_resolution_clock::time_point>* getTimingList();

}