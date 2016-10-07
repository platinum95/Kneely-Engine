#include "Timer.h"
#include <algorithm>



Timer::Timer(int _type, bool _sorted) {
	events = std::vector<TimerEvent*>();
	this->type = _type;
	looper = true;
	runThread = false;
	sorted = _sorted;
}


Timer::~Timer() {
	stop();
}

TimerEvent * Timer::addEvent(void(*_p)(void *), void * data, double time) {
	TimerEvent *newEvent = new TimerEvent(_p, data, time);
	addEvent(newEvent);
	return newEvent;
}

void Timer::start() {
	runThread = true;
	this->eventFunction = (this->sorted) ? &Timer::checkEventsSorted : &Timer::checkEvents;
	this->timerThread = (this->type == SELF_TIMER) ? std::thread([this] { checkTimesSelf(); }) : std::thread([this] { checkTimes(); });

	

}

void Timer::stop() {
	runThread = false;


}

void Timer::end() {
	stop();
	looper = false;
	timerThread.join();
}

void Timer::addEvent(TimerEvent *_te) {
	this->events.push_back(_te);
}

void Timer::setTime(double newTime) {
	this->currentTime = newTime;
}

double Timer::getTime() {
	return currentTime;
}

void Timer::checkEvents() {
	TimerEvent* tr = NULL;
	for (TimerEvent* te : events) {
		double timeDiff = currentTime - te->time;
		if (timeDiff > 0.0) {
			te->p(te->data);
			tr = te;
		}
	}
	if (tr != NULL) {
		std::vector<TimerEvent*>::iterator tri = std::find(events.begin(), events.end(), tr);
		if (tri != events.end())
			events.erase(tri);
	}
}

void Timer::checkEventsSorted() {
	if (events.size() <= 0) {
		runThread = false;
		return;
	}
	double timeDiff =  events[0]->time - currentTime;
	if (timeDiff > 0.2)
		std::this_thread::sleep_for(std::chrono::milliseconds((int) (timeDiff * 1000.0)));
	timeDiff = events[0]->time - currentTime;
	if (timeDiff < 0.0) {
		events[0]->p(events[0]->data);
		events.erase(events.begin());
	}
	
	
}

void Timer::checkTimesSelf() {
	while (looper) {
		if (runThread) {
			(this->*eventFunction)();
		}
	}

}
void Timer::checkTimes() {
	while (looper) {
		if (runThread) {
			(this->*eventFunction)();
		}
	}

}
