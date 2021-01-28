#pragma once

namespace LGG
{

class GameTimer
{
private:
	__int64 mCountsPerSec;
	double mSecondsPerCount;

	//上一次reset()的时间
	__int64 mBaseCount;
	//上一帧tick()的时间
	__int64	mPrevCount_;
	//前两次tick()的时间差
	double mDeltaTime;

	//总共暂停的时间
	__int64 mPausedCounts;
	//上一次暂停的时间点
	__int64	mStopCount;

	bool mStopped;
public:
	GameTimer();

	void tick();

	//前两次tick()的时间差
	float deltaTime() const;

	void reset();

	bool isStopped() const;

	void stop();

	void start();

	float totalTime() const;

};

}