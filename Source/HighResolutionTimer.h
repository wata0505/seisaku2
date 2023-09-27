#pragma once

#include <windows.h>

class HighResolutionTimer
{
public:
	HighResolutionTimer()
	{
		LONGLONG countsPerSec;
		QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&countsPerSec));
		secondsPerCount = 1.0 / static_cast<double>(countsPerSec);

		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&thisTime));
		baseTime = thisTime;
		lastTime = thisTime;
	}
	~HighResolutionTimer() = default;
	HighResolutionTimer(const HighResolutionTimer&) = delete;
	HighResolutionTimer& operator=(const HighResolutionTimer&) = delete;
	HighResolutionTimer(HighResolutionTimer&&) noexcept = delete;
	HighResolutionTimer& operator=(HighResolutionTimer&&) noexcept = delete;

	// Returns the total time elapsed since Reset() was called, NOT counting any
	// time when the clock is stopped.
	float TimeStamp() const  // in seconds
	{
		// If we are stopped, do not count the time that has passed since we stopped.
		// Moreover, if we previously already had a pause, the distance 
		// stop_time - base_time includes paused time, which we do not want to count.
		// To correct this, we can subtract the paused time from mStopTime:  
		//
		//                     |<--paused_time-->|
		// ----*---------------*-----------------*------------*------------*------> time
		//  base_time       stop_time        start_time     stop_time    this_time

		if (stopped)
		{
			return static_cast<float>(((stopTime - pausedTime) - baseTime) * secondsPerCount);
		}

		// The distance this_time - mBaseTime includes paused time,
		// which we do not want to count.  To correct this, we can subtract 
		// the paused time from this_time:  
		//
		//  (this_time - paused_time) - base_time 
		//
		//                     |<--paused_time-->|
		// ----*---------------*-----------------*------------*------> time
		//  base_time       stop_time        start_time     this_time
		else
		{
			return static_cast<float>(((thisTime - pausedTime) - baseTime) * secondsPerCount);
		}
	}

	float TimeInterval() const  // in seconds
	{
		return static_cast<float>(deltaTime);
	}

	void Reset() // Call before message loop.
	{
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&thisTime));
		baseTime = thisTime;
		lastTime = thisTime;

		stopTime = 0;
		stopped = false;
	}

	void Start() // Call when unpaused.
	{
		LONGLONG startTime;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startTime));

		// Accumulate the time elapsed between stop and start pairs.
		//
		//                     |<-------d------->|
		// ----*---------------*-----------------*------------> time
		//  base_time       stop_time        start_time     
		if (stopped)
		{
			pausedTime += (startTime - stopTime);
			lastTime = startTime;
			stopTime = 0;
			stopped = false;
		}
	}

	void Stop() // Call when paused.
	{
		if (!stopped)
		{
			QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&stopTime));
			stopped = true;
		}
	}

	void Tick() // Call every frame.
	{
		if (stopped)
		{
			deltaTime = 0.0;
			return;
		}

		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&thisTime));
		// Time difference between this frame and the previous.
		deltaTime = (thisTime - lastTime) * secondsPerCount;

		// Prepare for next frame.
		lastTime = thisTime;

		// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
		// processor goes into a power save mode or we get shuffled to another
		// processor, then mDeltaTime can be negative.
		if (deltaTime < 0.0)
		{
			deltaTime = 0.0;
		}
	}

private:
	double secondsPerCount{ 0.0 };
	double deltaTime{ 0.0 };

	LONGLONG baseTime{ 0LL };
	LONGLONG pausedTime{ 0LL };
	LONGLONG stopTime{ 0LL };
	LONGLONG lastTime{ 0LL };
	LONGLONG thisTime{ 0LL };

	bool stopped{ false };
};
