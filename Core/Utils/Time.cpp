#include "Time.h"

float Time::deltaTime = 0.f;
system_clock::time_point Time::lastTime = system_clock::now();

float Time::tick()
{
	auto now = system_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTime);
	lastTime = now;
	return duration.count();
}
