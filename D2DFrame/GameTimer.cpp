#include "pch.h"
#include "GameTimer.h"


LGG::GameTimer::GameTimer()
{
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&mCountsPerSec));
	mSecondsPerCount = 1.0 / mCountsPerSec;
	reset();
}

void LGG::GameTimer::tick()
{
	if (mStopped) {
		mDeltaTime = 0.0;
		return;
	}

	__int64 currCount;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currCount));

	mDeltaTime = (currCount - mPrevCount_) * mSecondsPerCount;
	mPrevCount_ = currCount;

	if (mDeltaTime < 0.0) {
		mDeltaTime = 0.0;
	}
}

float LGG::GameTimer::deltaTime() const 
{
	return mDeltaTime;
}

void LGG::GameTimer::reset()
{
	__int64 currCount;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currCount));
	mBaseCount = currCount;
	mPrevCount_ = currCount;
	mStopCount = 0;
	mStopped = false;
}

bool LGG::GameTimer::isStopped() const
{
	return mStopped;
}

void LGG::GameTimer::stop()
{
	if (!mStopped) {
		__int64 currCounts;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currCounts));

		mStopCount = currCounts;
		mStopped = true;
	}
}

void LGG::GameTimer::start()
{
	if (mStopped) {
		__int64 startCounts;
		QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&startCounts));

		mPausedCounts += startCounts - mStopCount;
		mPrevCount_ = startCounts;
		mStopped = false;
	}
}

float LGG::GameTimer::totalTime() const
{
	if (mStopped) {
		return static_cast<float>(
			(mStopCount - mBaseCount - mPausedCounts) * mSecondsPerCount
			);
	}
	else {
		return static_cast<float>(
			(mPrevCount_ - mBaseCount - mPausedCounts) * mSecondsPerCount
			);
	}
}