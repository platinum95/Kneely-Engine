#pragma once

#include <vector>
#include <thread>
#include <chrono>

#define SELF_TIMER		0
#define EXT_TIMER		1


struct TimerEvent {
	double time;
	void* data;
	void(*p)(void*);
	TimerEvent(void(*_p)(void*), void* _data, double _time) : p(_p), data(_data), time(_time) {}
};

class Timer {
public:
	Timer(int, bool);
	~Timer();
	TimerEvent* addEvent(void(*_p)(void*), void* data, double time);
	
	void(Timer::*eventFunction)();
	void start();
	void stop();
	void end();
	void addEvent(TimerEvent*);
	void setTime(double newTime);
	double getTime();

private:
	std::vector<TimerEvent*> events;
	bool runThread, looper, sorted;
	int type;
	double currentTime, lastTime;
	void checkEvents();
	void checkEventsSorted();
	void checkTimes();
	void checkTimesSelf();
	
	std::thread timerThread;

};

