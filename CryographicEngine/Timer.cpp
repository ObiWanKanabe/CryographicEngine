#include "Timer.h"

Timer* Timer::theInstance = nullptr;

Timer::Timer() {

}

Timer::~Timer() {

}

double Timer::getSeconds() {
	frequency = getTicksPerSecond();
	time = getTicks();
	double seconds = time.QuadPart / frequency.QuadPart;

	return seconds;
}

Timer& Timer::getInstance() {
	if (theInstance == nullptr) {
		theInstance = new Timer();
	}
	return *theInstance;
}