#pragma once
//Class to handle timings, modelled after the OpenGL convention of "buffer" creation

#include <chrono>
#include <vector>

namespace Timing {
	typedef unsigned int uint;
	//Generate #count number of timer IDs
	void genTimingID(uint *buffer, uint count);

	//Get time since function was last called, in resolution of some chrono type
	
	/*
	template <typename TimeType>
	TimeType Timing::getTimeDiff(unsigned int id) {
		using namespace std::chrono;
		high_resolution_clock::time_point current_time = high_resolution_clock::now();
		//TimeType time_diff = duration_cast<TimeType>(current_time - timestamps[id]).count();
		std::chrono::nanoseconds duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - timestamps[id]);
		timestamps[id] = current_time;
		return duration;

	}
	*/

	std::chrono::nanoseconds getTimeDiffNanos(unsigned int id);
	//TODO - delete commented block if template stuff works
	/*
	std::chrono::milliseconds getTimeDiffMillis(uint id);
	std::chrono::microseconds getTimeDiffMicros(uint id);
	std::chrono::seconds getTimeDiffSeconds(uint id);
	*/

	//Initialise timer with current time
	std::chrono::steady_clock::time_point start(uint id);

	//Get the current system time
	std::chrono::steady_clock::time_point getCurrentTime();

}