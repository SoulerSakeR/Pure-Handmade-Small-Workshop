#pragma once
#include <chrono>

using std::chrono::system_clock;

class Time
{
public:
	Time() { current_time = system_clock::now(); }
	typedef std::chrono::system_clock::time_point TimePoint;
	// returns the time in milliseconds since the last call of tick

	float operator-(const Time& other) const
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(current_time - other.current_time).count()/1000.f;
	}


	static float tick();
	static Time now() { return Time(); }
	static float deltaTime;

	TimePoint current_time;
private:
	static system_clock::time_point lastTime;

};

