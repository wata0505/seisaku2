#include "Graphics.h"
#include "Effect.h"
#include "EffectManager.h"


Effect::Effect(const char* filename)
{
    //エフェクトを読み込みする前にロックする
    //＊マルチスレッドでEffectを作成するとDeviceContextを同時アクセスして
    //　フリーズする可能性があるので排他制御する
    std::lock_guard<std::mutex>lock(Graphics::Instance().GetMutex());

    //Effekseerのリソースを読み込む
    //EffekseerはUTF-16のファイルパス以外は対応していないため文字コード変換が必要
    char16_t utf16Filename[256];
    Effekseer::ConvertUtf8ToUtf16(utf16Filename, 256, filename);

    //Effekseer::Managerを取得
    Effekseer::Manager* effekseerManager = EffectManager::instance().GetEffekseerManager();

    //Effekseerエフェクトの読み込み
    effekseerEffect = Effekseer::Effect::Create(effekseerManager, (EFK_CHAR*)utf16Filename);
}
Effect::~Effect()
{
    if (effekseerEffect != nullptr)
    {
        effekseerEffect->Release();
        effekseerEffect = nullptr;
    }
}

//再生
Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, float scale, int32_t timer)
{
    Effekseer::Manager* effekseerManager = EffectManager::instance().GetEffekseerManager();

    Effekseer::Handle handle = effekseerManager->Play(effekseerEffect,{ position.x, position.y, position.z},timer);
    effekseerManager->SetScale(handle, scale, scale, scale);
    return handle;
}

//停止
void Effect::Stop(Effekseer::Handle handle)
{
    Effekseer::Manager* effekseerManager = EffectManager::instance().GetEffekseerManager();

    effekseerManager->StopEffect(handle);
}
void Effect::SetSpeed(Effekseer::Handle handle, float speed)
{
    Effekseer::Manager* effekseerManager = EffectManager::instance().GetEffekseerManager();

    effekseerManager->SetSpeed(handle,speed);
}

//座標設定
void Effect::SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
    Effekseer::Manager* effekseerManager = EffectManager::instance().GetEffekseerManager();

    effekseerManager->SetLocation(handle, position.x, position.y, position.z);
}

//スケール設定
void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
    Effekseer::Manager* effekseerManager = EffectManager::instance().GetEffekseerManager();

    effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
}

void Effect::SetRotation(Effekseer::Handle handle, const DirectX::XMFLOAT3& a)
{
    Effekseer::Manager* effekseerManager = EffectManager::instance().GetEffekseerManager();

    effekseerManager->SetRotation(handle, a.x, a.y, a.z);
}