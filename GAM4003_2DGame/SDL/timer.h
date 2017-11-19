/*
File: timer.h
Author: Peter Lager
Version: 2.10
Date Last Modified: 27-12-2006

Version 2.20
Pause time functions added: startPause and endPause

Include file that provides a timer device to either
provide a delay or to measure elapsed time. The same
timer should not be used to perform both actions
simultaneously.

To use the features in this facility you need to #include
this file and add the following line
using namespace timer;

Create an instance of a timer use
Timer mytimer;

To create a half second delay in your program use
mytimer.delay(0.5f);

To measure elapsed time start or reset the timer using

mytimer.startTimer();
or
mytimer.resetTimer();

later in your program you can measure the elapsed
time since the timer was started or reset using

elapsedTime = mytimer.getElapsedTime();

where elapsedTime is of type 'double'

You can also use
lapTime = mytimer.getLapTime();

which returns the number of seconds passed since
the method was last called.

*/

#pragma once

#include <windows.h>

namespace timer
{
	/**
	This class provides a high resolution timer object.

	To use the features in this facility you need to #include this
	file and add the following line
	\code using namespace timer; \endcode

	You can then create and use your own timer objects e.g.
	\code Timer t1, t2; \endcode

	Full supporting comments are in the timer.h file
	*/

	class Timer
	{
	public:
		Timer(void)
		{
			m_TicksPerSecond.QuadPart = 1;
			m_StartTime.QuadPart = 0;
			startTimer();
		}

		virtual ~Timer(void){};

		// Start clock and initialise start time to now
		bool startTimer()
		{
			if (!QueryPerformanceFrequency(&m_TicksPerSecond))
				return false;
			else
			{
				QueryPerformanceCounter(&m_StartTime);
				return true;
			}
		}

		// Reset clock to now (Does same as startTimer()
		bool resetTimer()
		{
			if (!QueryPerformanceFrequency(&m_TicksPerSecond))
				return false;
			else
			{
				QueryPerformanceCounter(&m_StartTime);
				return true;
			}
		}

		// Get time since the last time this method was called
		double getLapTime()
		{
			static LARGE_INTEGER lastTime = m_StartTime;
			LARGE_INTEGER currentTime;
			double seconds;

			QueryPerformanceCounter(&currentTime);

			seconds = ((double)currentTime.QuadPart - (double)lastTime.QuadPart) /
				(double)m_TicksPerSecond.QuadPart;
			lastTime = currentTime;

			return seconds;
		}

		// Get total time since timer was 'started' or 'reset'
		double getElapsedTime()
		{
			LARGE_INTEGER currentTime;
			double seconds;

			QueryPerformanceCounter(&currentTime);

			seconds = ((double)currentTime.QuadPart - (double)m_StartTime.QuadPart) /
				(double)m_TicksPerSecond.QuadPart;

			return seconds;
		}

		// Pause for a period of time measured in seconds
		void delay(double pauseTime)
		{
			LARGE_INTEGER startTime, currentTime;
			double seconds;

			QueryPerformanceCounter(&startTime);
			do
			{
				QueryPerformanceCounter(&currentTime);
				seconds = ((double)currentTime.QuadPart - (double)startTime.QuadPart) /
					(double)m_TicksPerSecond.QuadPart;
			} while (seconds <= pauseTime);
		}

//******************************************	v2.20
		void startPause(){	//added v2.20
			QueryPerformanceCounter(&m_pauseTime);
		}

		void endPause(){	//added v2.20
			if (m_pauseTime.QuadPart > m_StartTime.QuadPart){
				LARGE_INTEGER currentTime;
				QueryPerformanceCounter(&currentTime);
				m_StartTime.QuadPart += currentTime.QuadPart - m_pauseTime.QuadPart;
			}
		}
//******************************************	
	protected:
		LARGE_INTEGER m_TicksPerSecond;
		LARGE_INTEGER m_StartTime;
		LARGE_INTEGER m_pauseTime;	//v2.20
	};
} // End of namespace timer
