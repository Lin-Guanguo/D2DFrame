#pragma once

namespace LGG
{

class GameTimer
{
private:
	__int64 mCountsPerSec;
	double mSecondsPerCount;

	//��һ��reset()��ʱ��
	__int64 mBaseCount;
	//��һ֡tick()��ʱ��
	__int64	mPrevCount_;
	//ǰ����tick()��ʱ���
	double mDeltaTime;

	//�ܹ���ͣ��ʱ��
	__int64 mPausedCounts;
	//��һ����ͣ��ʱ���
	__int64	mStopCount;

	bool mStopped;
public:
	GameTimer();

	void tick();

	//ǰ����tick()��ʱ���
	float deltaTime() const;

	void reset();

	bool isStopped() const;

	void stop();

	void start();

	float totalTime() const;

};

}