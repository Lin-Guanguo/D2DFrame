#include "pch.h"
#include "ResourceLoader.h"
#include "FlappyBird.h"
#include "GameTimer.h"
#include "constants.h"
#include "resource.h"

LGG::FlappyBird::FlappyBird()
{
	reset();

	using namespace std;
	ifstream ifs{ "resources\\atlas.txt" };
	string name;
	int px, py;
	float left, up, ratex, ratey ;
	while (ifs >> name >> px >> py >> left >> up >> ratex >> ratey) {
		mBitmapLayout.emplace(make_pair(name,
			D2D1::RectF(
				IMAGE_SOURCE_SIZE_X * left,
				IMAGE_SOURCE_SIZE_Y * up,
				IMAGE_SOURCE_SIZE_X * left + px,
				IMAGE_SOURCE_SIZE_Y * up + py
			)));
	}
	{//初始化地面数据
		auto& landRect = mBitmapLayout[std::string{ "land" }];
		mLandLocation = D2D1::Point2F(
			0.0f, GAME_SCREEN_SIZE_Y - (landRect.bottom - landRect.top)
			);
		mLandLocationLoopDeltaX = (landRect.right - landRect.left) / 14;
	}
}

void LGG::FlappyBird::update(const GameTimer& timer)
{
	auto deltaTime = timer.deltaTime();
	//鸟的飞行状态
	if (mState == BirdState::FLY) {
		mVerticalSpeed -= deltaTime * FALL_ACCELERATION;
		mBirdCenter.y += ( - mVerticalSpeed) * deltaTime;
	}
	else if (mState == BirdState::HOVER) {
		mVerticalSpeed = 0;
	}
	if (mState != BirdState::DIE) {
		mBirdImage = (static_cast<int>(timer.totalTime()*10)) % 4;
	}

	//地面纹理
	if (mState != BirdState::DIE) {
		mLandLocation.x -= LAND_SPEED * deltaTime;
		if (mLandLocation.x < -mLandLocationLoopDeltaX) {
			mLandLocation.x += mLandLocationLoopDeltaX;
		}
	}

	//管道
	if (mState == BirdState::FLY) {
		for (auto& p : mPipes) {
			p.x -= deltaTime * PIPE_SPEED;
		}
		if (mPipes.begin()->x < -Pipe::DISTANCE) {
			auto p = *mPipes.begin();
			mPipes.erase(mPipes.begin());
			p.x = (--mPipes.end())->x + Pipe::DISTANCE;
			p.downHeight = mPipeheightGen(mRandomEngine);
			mPipes.push_back(p);
		}
	}

	//碰撞检测
	impactChecking();
}

void LGG::FlappyBird::render(CComPtr<ID2D1HwndRenderTarget> renderTarget)
{
	renderBackground(renderTarget);
	renderLand(renderTarget);
	renderTutorial(renderTarget);
	renderPipe(renderTarget);
	renderBird(renderTarget);
}

void LGG::FlappyBird::fly()
{
	if (mState == BirdState::FLY) {
		mVerticalSpeed = ONCE_FLY_SPEED;
	}
}

void LGG::FlappyBird::setState(BirdState newState)
{
	mState = newState;
}

void LGG::FlappyBird::reset()
{
	mBirdCenter = D2D1::Point2F(
		GAME_SCREEN_SIZE_X / 2,
		GAME_SCREEN_SIZE_Y / 2);
	mVerticalSpeed = 0;
	mState = BirdState::HOVER;

	mPipes.clear();
	for (int i = 1; i < 6; ++i) {
		mPipes.push_back({ GAME_SCREEN_SIZE_X + i * Pipe::DISTANCE, mPipeheightGen(mRandomEngine) });
	}
}

void LGG::FlappyBird::startFly()
{
	if (mState == BirdState::HOVER) {
		mState = BirdState::FLY;
	}
}

void LGG::FlappyBird::createDeviceIndependentResources(CComPtr<ID2D1Factory> factory)
{
}

void LGG::FlappyBird::discardDeviceIndependentResources()
{
}

void LGG::FlappyBird::createDeviceDependentResources(CComPtr<ID2D1HwndRenderTarget> renderTarget)
{
	BitmapLoader::LoadBitmapFromResource(
		renderTarget,
		IDB_PNG1,
		L"PNG",
		&mBitmapSource
	);
}

void LGG::FlappyBird::discardDeviceDependentResources()
{
	mBitmapSource.Release();
}

void LGG::FlappyBird::impactChecking()
{
	if (mState == BirdState::FLY) {
		if (mBirdCenter.y > GAME_SCREEN_SIZE_Y - LAND_HEIGHT - BIRD_IMPACT_RADUIS) {
			mState = BirdState::DIE;
		}
		for (auto& p : mPipes) {
			if (mBirdCenter.x - p.x > -BIRD_IMPACT_RADUIS &&
				mBirdCenter.x - (p.x + Pipe::PIP_WIDTH) < BIRD_IMPACT_RADUIS) 
			{
				if (mBirdCenter.y - p.downHeight < BIRD_IMPACT_RADUIS ||
					mBirdCenter.y - (p.downHeight + Pipe::GAP) > - BIRD_IMPACT_RADUIS)
				{
					mState = BirdState::DIE;
				}
			}
		}
	}
}

void LGG::FlappyBird::renderBackground(CComPtr<ID2D1HwndRenderTarget> renderTarget)
{
	std::string name = "bg_day";
	auto& srcRect = mBitmapLayout[name];
	auto destRect = D2D1::Rect(
		0.0f,
		0.0f,
		0.0f + srcRect.right - srcRect.left,
		0.0f + srcRect.bottom - srcRect.top
	);
	renderTarget->DrawBitmap(
		mBitmapSource,
		destRect,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		srcRect
	);
}

void LGG::FlappyBird::renderBird(CComPtr<ID2D1HwndRenderTarget> renderTarget)
{
	std::string name;
	switch (mBirdImage) {
	case 0: name = "bird0_0"; break;
	case 1: name = "bird0_1"; break;
	case 2: name = "bird0_2"; break;
	case 3: name = "bird0_1"; break;
	}
	auto& srcRect = mBitmapLayout[name];
	auto destRect = D2D1::Rect(
		mBirdCenter.x - BIRD_SIZE_X / 2,
		mBirdCenter.y - BIRD_SIZE_X / 2,
		mBirdCenter.x - BIRD_SIZE_X / 2 + srcRect.right - srcRect.left,
		mBirdCenter.y - BIRD_SIZE_X / 2 + srcRect.bottom - srcRect.top
	);

	renderTarget->SetTransform(
		D2D1::Matrix3x2F::Rotation(-0.07f * mVerticalSpeed, mBirdCenter)
	);

	renderTarget->DrawBitmap(
		mBitmapSource,
		destRect,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		srcRect
	);

	renderTarget->SetTransform(
		D2D1::Matrix3x2F::Identity()
	);
}

void LGG::FlappyBird::renderLand(CComPtr<ID2D1HwndRenderTarget> renderTarget)
{
	auto& srcRect = mBitmapLayout[std::string{ "land" }];
	auto width = srcRect.right - srcRect.left;
	auto height = srcRect.bottom - srcRect.top;
	
	auto destRect = D2D1::RectF(
		mLandLocation.x,
		mLandLocation.y,
		mLandLocation.x + width,
		mLandLocation.y + height
		);

	renderTarget->DrawBitmap(
		mBitmapSource,
		destRect,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		srcRect
	);
}

void LGG::FlappyBird::renderTutorial(CComPtr<ID2D1HwndRenderTarget> renderTarget)
{
	if (mState == BirdState::HOVER) {
		auto& srcRect = mBitmapLayout[std::string{ "tutorial" }];
		auto width = srcRect.right - srcRect.left;
		auto height = srcRect.bottom - srcRect.top;

		auto destRect = D2D1::RectF(
			(GAME_SCREEN_SIZE_X - width) / 2,
			(GAME_SCREEN_SIZE_Y - height) / 2,
			(GAME_SCREEN_SIZE_X + width) / 2,
			(GAME_SCREEN_SIZE_Y + height) / 2
		);

		renderTarget->DrawBitmap(
			mBitmapSource,
			destRect,
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			srcRect
		);
	}
}

void LGG::FlappyBird::renderPipe(CComPtr<ID2D1HwndRenderTarget> renderTarget)
{
	auto& srcRectDown = mBitmapLayout[std::string{ "pipe_down" }];
	auto& srcRectUp = mBitmapLayout[std::string{ "pipe_up" }];
	auto width = srcRectDown.right - srcRectDown.left;
	auto height = srcRectDown.bottom - srcRectDown.top;
	for (auto& p : mPipes) {
		auto destRectDown = D2D1::RectF(
			p.x,
			p.downHeight - height,
			p.x + width,
			p.downHeight
		);
		auto destRectUp = D2D1::RectF(
			p.x,
			p.downHeight + Pipe::GAP,
			p.x + width,
			p.downHeight + Pipe::GAP + height
		);

		renderTarget->DrawBitmap(
			mBitmapSource,
			destRectDown,
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			srcRectDown
		);
		
		renderTarget->DrawBitmap(
			mBitmapSource,
			destRectUp,
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
			srcRectUp
		);

	}

}



