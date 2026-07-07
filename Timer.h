#pragma once
#include <ctime>
using namespace std;

class Timer
{
private:
	clock_t _time;
public:
	////////////
	// Structors
	Timer() { _time = clock(); };
	~Timer() {};

	////////////
	// Accessors
	clock_t getTime() const		{ return _time; }

	////////////
	// Functions
	void resetTimer()			{ _time = clock(); }
	float getTimeElapsed()		{ return (float)((clock() - _time) * 1.0f / CLOCKS_PER_SEC); }
	float getTimeSince(Timer& t){ return (float)((t.getTime() - _time) * 1.0f / CLOCKS_PER_SEC); }
};
