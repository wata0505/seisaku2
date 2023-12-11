#include"Totem.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "Graphics.h"
#include "TrapManager.h"
#include "Camera.h"
Totem::Totem()
{

    model = std::make_unique<Model>(".\\resources\\Trap\\AttacheCase\\AttacheCase.fbx", true);
    model->ModelSerialize(".\\resources\\Trap\\AttacheCase\\AttacheCase.fbx");
    model->ModelRegister(".\\resources\\Trap\\AttacheCase\\AttacheCase.fbx", "Texture\\AO.png");

    scale.x = scale.y = scale.z = 3.0f;


    UpdateTransform(0, 0);
    model->UpdateBufferDara(transform);
    renderdata = model->GetBufferData();
    type = Trap::TrapType::TrapTotem;
}
Totem::~Totem()
{

}

void Totem::Update(float elapsedTime)
{
    CollisionVsEnemies();
    UpdateTransform(0, 0);
    model->UpdateBufferDara(transform);
    //モデル描画情報受け渡し
    renderdata = model->GetBufferData();
    //hpRenderFlag = Collision::IntersectFanVsSphere(
    //    Camera::Instance().GetEye(),
    //    Camera::Instance().GetFront(),
    //    Camera::Instance().GetFovY(),
    //    Camera::Instance().GetFarZ(),
    //    position,
    //    radius);
}

void Totem::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
    shader->Draw(dc, model.get());
}
void Totem::Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader)
{
}
void Totem::DrawDebugPrimitive()
{
    // 基底クラスのデバッグプリミティブ描画
    Trap::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //衝突判定用のデバック円柱を描画
    //debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));


    // 縄張り範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(position, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
}

//敵との衝突処理
void Totem::CollisionVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //全ての敵と総当たりで衝突処理
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);
        //衝突処理
        DirectX::XMFLOAT3 outPosition;

        if (Collision::IntersectCylinderVsCylinder(
            enemy->GetPosition(), enemy->GetRadius(), enemy->GetHeight(),
            position, territoryRange, height,
            outPosition))
        {
            if (enemy->GetHealth() <= 0)
            {
                //ポイント追加加算
                TrapManager::Instance().SetTrapPoint(TrapManager::Instance().GetTrapPoint() + 1);

                //仮のデバック用で消してみる
                Destroy();
            }

        }
    }
}