#pragma once
#include "ID2DGraph.h"
#include "IUpdateWithTime.h"
#include "constants.h"

namespace LGG
{

class FlappyBird : public ID2DGraph, public IUpdateWithTime
{
public:
    enum class BirdState {
        HOVER, FLY, DIE
    };
private:
    //ͼƬ��Դ
    CComPtr<ID2D1Bitmap> mBitmapSource;
    //ԭͼ��Ԫ�طֲ���Ϣ
    std::unordered_map<std::string, D2D1_RECT_F> mBitmapLayout;

    //��λ��
    D2D1_POINT_2F mBirdCenter;
    //��ķ�����ͼ��[0, 3] 4����ͼ
    int mBirdImage = 0;
    //�ٶ� ����Ϊ��
    float mVerticalSpeed = 0;
    //����״̬
    BirdState mState = BirdState::HOVER;

    //������Ⱦλ��
    D2D1_POINT_2F mLandLocation;
    float mLandLocationLoopDeltaX;

    //�ܵ�
    std::default_random_engine mRandomEngine;
    std::uniform_real_distribution<float> mPipeheightGen{ Pipe::HEIGHT_LOW, Pipe::HEIGHT_HIGH };
    struct Pipe {
        static constexpr float GAP = GAME_SCREEN_SIZE_Y / 8 * 2;
        static constexpr float DISTANCE = 192;

        static constexpr float IMAGE_WIDTH = 52;
        static constexpr float IMAGE_HEIGHT = 320;

        static constexpr float HEIGHT_HIGH = GAME_SCREEN_SIZE_Y / 8 * 3;
        static constexpr float HEIGHT_LOW = GAME_SCREEN_SIZE_Y / 8 * 1;

        static constexpr float PIP_WIDTH = 52;

        float x;
        float downHeight;
    };
    std::list<Pipe> mPipes;

    static constexpr float ONCE_FLY_SPEED = 400.0f;
    static constexpr float FALL_ACCELERATION = 1000.0f;
    static constexpr float LAND_SPEED = 150.0f;
    static constexpr float PIPE_SPEED = LAND_SPEED;
public:
    FlappyBird();

    void fly();

    void setState(BirdState newState);

    void reset();

    void startFly();

    virtual void update(const GameTimer& timer) override;

    virtual void render(CComPtr<ID2D1HwndRenderTarget> renderTarget) override;

    virtual void createDeviceIndependentResources(CComPtr<ID2D1Factory> factory) override;

    virtual void discardDeviceIndependentResources() override;

    virtual void createDeviceDependentResources(CComPtr<ID2D1HwndRenderTarget> renderTarget) override;

    virtual void discardDeviceDependentResources() override;
private:
    void impactChecking();

    void renderBackground(CComPtr<ID2D1HwndRenderTarget> renderTarget);

    void renderBird(CComPtr<ID2D1HwndRenderTarget> renderTarget);

    void renderLand(CComPtr<ID2D1HwndRenderTarget> renderTarget);

    void renderTutorial(CComPtr<ID2D1HwndRenderTarget> renderTarget);

    void renderPipe(CComPtr<ID2D1HwndRenderTarget> renderTarget);
};

}
