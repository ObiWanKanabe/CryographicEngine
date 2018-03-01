#ifndef TIMER_H
#define TIMER_H

#include <Windows.h>

class Timer {

private:

	// Private constructor because Timer class is a singleton
	Timer();

	// Variables to store ticks per second and current number of ticks respectively 
	LARGE_INTEGER frequency, prevTime, currtime;

	// Single instance of the Timer
	static Timer *theInstance;

public:

	~Timer();

	// Method that gives access to the single instance of the timer
	static Timer& GetInstance();


	// Returns the current number of CPU ticks 
	static inline LARGE_INTEGER GetTicks() {
		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);
		return t;
	}

	// Returns the number of CPU ticks per seconds
	static inline LARGE_INTEGER GetTicksPerSecond() {
		LARGE_INTEGER f;
		QueryPerformanceFrequency(&f);
		return f;
	}

	// Returns the number of seconds since the computer has been turned on
	double GetSeconds();

	void Start();
	void Update();

	LARGE_INTEGER GetDeltaTime();


};
#endif

