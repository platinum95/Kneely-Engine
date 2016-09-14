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
	if (events.size() > 0) {
		//double dur = currentTime - events[0]->time;
		//std::chrono::duration<double> cdur(dur);

//		if (dur < -2)
			//std::this_thread::sleep_for(cdur);
		

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
