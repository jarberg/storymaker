#pragma once

#include "Events.h" 
#include <Singleton.h>

struct TimeFormat {

	int hour;
	int minute;
	int second;
	TimeFormat(int h, int m, int s) : hour(h), minute(m), second(s) {};
};

class Timer : public Singleton<Timer>{
	friend class Singleton<Timer>;

private:
	int _currentTime = 0;
	int _currentDay = 0;
	int _currentWeek = 0;

public:
	EventDispatcher advanceTime;
	EventDispatcher advanceDay;
	EventDispatcher advanceWeek;

	static const int MAXTIME = 3;
	static const int MAXDAY = 7;
	static const int MAXWEEK = 4;

	Timer() {
		this->advanceTime.Subscribe<advanceTimeEvent>([this](std::shared_ptr<advanceTimeEvent> AT)
			{
				std::cout << "time is advanced" << "\n";
			}
		);
	}

	~Timer() {};

	void advancetime() {
		bool updatedTime = false;
		bool updatedDay = false;

		int time = this->_currentTime;
		int day = this->_currentDay;
		int week = this->_currentWeek;

		if (MAXTIME > 0) {
			time = (this->_currentTime + 1) % MAXTIME; 
		}
		if (MAXDAY > 0 && time == 0) {
		    day = (this->_currentDay + 1) % MAXDAY;
			updatedDay = true;
		}
		if (MAXWEEK > 0 && updatedDay && day == 0) {
			week = (this->_currentWeek + 1) % MAXWEEK;
		}
		this->setTime(time, day, week);
		this->advanceTime.Dispatch(std::make_shared<advanceTimeEvent>(this->_currentTime, this->_currentDay, this->_currentWeek));
	};

	void setTime(int time, int day, int week) {
		this->_currentTime = time;
		this->_currentDay = day;
		this->_currentWeek = week;
	};

	void getTime(int* time, int* day, int* week) {

		*time = this->_currentTime;
		*day = this->_currentDay;
		*week = this->_currentWeek;
	};
	void showTime() {
		std::cout << "Time: " << this->_currentTime << " Day: " << this->_currentDay << " Week: " << this->_currentWeek << "\n";
	};

};