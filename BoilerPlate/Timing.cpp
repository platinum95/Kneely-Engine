#include "Timing.h"


using namespace std;
using namespace chrono;
using namespace Timing;


typedef std::chrono::high_resolution_clock::time_point tp;

std::vector<tp> timestamps = std::vector<tp>();


std::chrono::nanoseconds Timing::getTimeDiffNanos(uint id) {
	high_resolution_clock::time_point current_time = high_resolution_clock::now();
	std::chrono::nanoseconds duration = std::chrono::duration_cast<std::chrono::nanoseconds>(current_time - timestamps[id]);
	timestamps[id] = current_time;
	return duration;
}

std::chrono::steady_clock::time_point Timing::start(uint id) {
	timestamps[id] = high_resolution_clock::now();
	return high_resolution_clock::now();
}

chrono::steady_clock::time_point Timing::getCurrentTime() {
	return high_resolution_clock::now();
}

void Timing::genTimingID(uint *buffer, uint count) {
	for (int i = 0; i < count; i++) {
		uint newId = timestamps.size();
		tp timep = high_resolution_clock::now();
		timestamps.push_back(high_resolution_clock::now());
		buffer[i] = newId;
	}
	return;
}

