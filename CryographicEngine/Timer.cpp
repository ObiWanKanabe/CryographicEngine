#include "Timer.h"

Timer* Timer::theInstance = nullptr;

Timer::Timer() {

}

Timer::~Timer() {

}

Timer& Timer::GetInstance() {
	if (theInstance == nullptr) {
		theInstance = new Timer();
	}
	return *theInstance;
}

double Timer::GetSeconds() {
	frequency = GetTicksPerSecond();
	currtime = GetTicks();
	double seconds = currtime.QuadPart / frequency.QuadPart;

	return seconds;
}

void Timer::Start() {
	frequency = GetTicksPerSecond();
	prevTime = GetTicks();
	currtime = GetTicks();
}

void Timer::Update() {
	prevTime = currtime;
	currtime = GetTicks();
}

LARGE_INTEGER Timer::GetDeltaTime() {
	LARGE_INTEGER deltaTime;
	deltaTime.QuadPart = currtime.QuadPart - prevTime.QuadPart;
	deltaTime.QuadPart *= 1000;
	deltaTime.QuadPart /= frequency.QuadPart;
	return deltaTime;
}