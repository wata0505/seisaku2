#include "Graphics.h"
#include "Input.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include <thread>

// 初期化
void SceneLoading::Initialize()
{
    //スプライト初期化
    sprite[0] = std::make_unique<Sprite>(L".\\resources\\LoadingIcon.png");

    sprite[1] = EffectTexAll::Instance().GetSprite(int(EffectTexAll::EfTexAll::Bock));

    //sousasetumei[0] = std::make_unique<Sprite>(L".\\resources\\UI\\setumei2.png");
    sousasetumei[1] = std::make_unique<Sprite>(L".\\resources\\UI\\Button.png");
    //tips[0] = std::make_unique<Sprite>(L".\\resources\\UI\\tips1.png");
    //tips[1] = std::make_unique<Sprite>(L".\\resources\\UI\\tips2.png");
   
    //スレッド開始
    std::thread thread(LoadingThread, this);
    tipsNo = rand() % tipsMax;
    //スレッドの管理を放棄
    thread.detach();
}

// 終了化
void SceneLoading::Finalize()
{
    
}

// 更新処理
void SceneLoading::Update(float elapsedTime)
{
    constexpr float speed = 180;
    angle += speed * elapsedTime;
    GamePad& gamePad = Input::Instance().GetGamePad();
    const GamePadButton anyButton =
        GamePad::BTN_A
        | GamePad::BTN_B
        | GamePad::BTN_X
        | GamePad::BTN_Y
        ;
    //次のシーンの準備が完了したらシーンを切り替える
    if (nextScene->IsReady())
    {
        if (gamePad.GetButtonDown() & anyButton) {
            SceneManager::Instance().ChangeScene(nextScene);
            nextScene = nullptr;
        }
    }
    tipsTimer++;
    //if (tipsTimer > 3) {
    //    tipsNo++;
    //    tipsTimer = 0;
    //    if (tipsNo >= tipsMax)tipsNo = 0;
    //}
}

// 描画処理
void SceneLoading::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
    RenderContext rc;
    //画面クリア＆レンダーターゲット設定
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);
    rc.deviceContext = dc;
    //2Dスプライト描画
    {
        {
            SpriteShader* shader = graphics.GetShader(Graphics::SpriteShaderId::SpriteShaderDefault);
            float screenWidth = static_cast<float>(graphics.GetScreenWidth());
            float screenHeight = static_cast<float>(graphics.GetScreenHeight());
            float textureWidth = static_cast<float>(sprite[0]->GetTextureWidth());
            float textureHeight = static_cast<float>(sprite[0]->GetTextureHeight());
            float positionX = screenWidth - 50;
            float positionY = screenHeight - 50;
            shader->Begin(rc);
            sprite[1]->Render(dc,
                0, 0, 1280, 720,
                0, tipsTimer * 10, sprite[1]->GetTextureWidth(), sprite[1]->GetTextureHeight(),
                0.0f,
                1.0f, 1.0f, 1.0f, 1.0f
            );
            shader->Draw(rc, sprite[1].get());
            sprite[0]->Render(dc,
                positionX, positionY, 50, 50,
                0, 0, textureWidth, textureHeight,
                angle,
                1, 1, 1, 1);
            //shader->Draw(rc, sprite[0].get());
            
            //sousasetumei[0]->Render(dc,
            //    0, 0, 1280, 720,
            //    0, 0, sousasetumei[0]->GetTextureWidth(), sousasetumei[0]->GetTextureHeight(),
            //    0.0f,
            //    1.0f, 1.0f, 1.0f, 1.0f
            //);
            //shader->Draw(rc, sousasetumei[0].get());
            if (nextScene->IsReady()) 
            {
                float width = static_cast<float>(sousasetumei[1]->GetTextureWidth());
                float height = static_cast<float>(sousasetumei[1]->GetTextureHeight());
                sousasetumei[1]->Render(dc,
                    screenWidth - width, screenHeight - height, width, height,
                    0, 0, width, height,
                    0.0f,
                    1.0f, 1.0f, 1.0f, 1.0f
                );
                shader->Draw(rc, sousasetumei[1].get());
            }
            //tips[tipsNo]->Render(dc,
            //    300, 0, tips[tipsNo]->GetTextureWidth(), tips[tipsNo]->GetTextureHeight(),
            //    0, 0, tips[tipsNo]->GetTextureWidth(), tips[tipsNo]->GetTextureHeight(),
            //    0.0f,
            //    1.0f, 1.0f, 1.0f, 1.0f
            //);
            //shader->Draw(rc, tips[tipsNo].get());

            shader->End(rc);
        }
    
    }
}

//ローディングスレッド
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    //COM関係の初期化でスレッド毎に呼ぶ必要がある
    CoInitialize(nullptr);

    //次のシーンの初期化を行う
    scene->nextScene->Initialize();
    //スレッドが終わる前にCOM関係の終了化
    CoUninitialize();

    //次のシーンの準備完了設定
    scene->nextScene->SetReady();
}