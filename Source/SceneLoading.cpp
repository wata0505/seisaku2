#include "Graphics.h"
#include "Input.h"
#include "SceneLoading.h"
#include "SceneManager.h"
#include <thread>

// ������
void SceneLoading::Initialize()
{
    //�X�v���C�g������
    sprite[0] = std::make_unique<Sprite>(L".\\resources\\LoadingIcon.png");

    sprite[1] = EffectTexAll::Instance().GetSprite(int(EffectTexAll::EfTexAll::Bock));

    //sousasetumei[0] = std::make_unique<Sprite>(L".\\resources\\UI\\setumei2.png");
    sousasetumei[1] = std::make_unique<Sprite>(L".\\resources\\UI\\Button.png");
    //tips[0] = std::make_unique<Sprite>(L".\\resources\\UI\\tips1.png");
    //tips[1] = std::make_unique<Sprite>(L".\\resources\\UI\\tips2.png");
   
    //�X���b�h�J�n
    std::thread thread(LoadingThread, this);
    tipsNo = rand() % tipsMax;
    //�X���b�h�̊Ǘ������
    thread.detach();
}

// �I����
void SceneLoading::Finalize()
{
    
}

// �X�V����
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
    //���̃V�[���̏���������������V�[����؂�ւ���
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

// �`�揈��
void SceneLoading::Render()
{
    Graphics& graphics = Graphics::Instance();
    ID3D11DeviceContext* dc = graphics.GetDeviceContext();
    ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
    ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();
    RenderContext rc;
    //��ʃN���A�������_�[�^�[�Q�b�g�ݒ�
    FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
    dc->ClearRenderTargetView(rtv, color);
    dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dc->OMSetRenderTargets(1, &rtv, dsv);
    rc.deviceContext = dc;
    //2D�X�v���C�g�`��
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

//���[�f�B���O�X���b�h
void SceneLoading::LoadingThread(SceneLoading* scene)
{
    //COM�֌W�̏������ŃX���b�h���ɌĂԕK�v������
    CoInitialize(nullptr);

    //���̃V�[���̏��������s��
    scene->nextScene->Initialize();
    //�X���b�h���I���O��COM�֌W�̏I����
    CoUninitialize();

    //���̃V�[���̏��������ݒ�
    scene->nextScene->SetReady();
}