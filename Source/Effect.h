#pragma once

#include<DirectXMath.h>
#include<Effekseer.h>

//エフェクト
class Effect
{
public:
    Effect(const char* fliename);
    ~Effect();

    //再生
    Effekseer::Handle Play(const DirectX::XMFLOAT3& position, float scale = 1.0f, int32_t timer = 0);

    //停止
    void Stop(Effekseer::Handle handle);

    //座標設定
    void SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position);

    void SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale);

    void SetRotation(Effekseer::Handle handle, const DirectX::XMFLOAT3& a);

    void SetSpeed(Effekseer::Handle handle, float speed);

private:
    Effekseer::Effect* effekseerEffect = nullptr;
};