#include "Timer.h"

Timer* Timer::theInstance = nullptr;

Timer::Timer() {

}

Timer::~Timer() {

}

double Timer::GetSeconds() {
	frequency = GetTicksPerSecond();
	time = GetTicks();
	double seconds = time.QuadPart / frequency.QuadPart;

	return seconds;
}

Timer& Timer::GetInstance() {
	if (theInstance == nullptr) {
		theInstance = new Timer();
	}
	return *theInstance;
}