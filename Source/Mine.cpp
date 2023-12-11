#include"Mine.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "Graphics.h"
#include "Camera.h"
Mine::Mine()
{

    model = std::make_unique<Model>(".\\resources\\Trap\\Mine\\Mine.fbx", true);
    model->ModelSerialize(".\\resources\\Trap\\Mine\\Mine.fbx");
    model->ModelRegister(".\\resources\\Trap\\Mine\\Mine.fbx");


    UpdateTransform(0, 0);
    model->UpdateBufferDara(transform);
    renderdata = model->GetBufferData();

    attack = 1.0f;
    height = 5.0f;
    type = Trap::TrapType::TrapMine;
}
Mine::~Mine()
{

}

void Mine::Update(float elapsedTime)
{
    CollisionVsEnemies();
    UpdateTransform(0, 0);
    model->UpdateBufferDara(transform);
    //モデル描画情報受け渡し
    renderdata = model->GetBufferData();
    objectManager.Update(elapsedTime * 1.5);

    //hpRenderFlag = Collision::IntersectFanVsSphere(
    //    Camera::Instance().GetEye(),
    //    Camera::Instance().GetFront(),
    //    Camera::Instance().GetFovY(),
    //    Camera::Instance().GetFarZ(),
    //    position,
    //    radius);
}

void Mine::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
    shader->Draw(dc, model.get());
}
void Mine::Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader)
{
    objectManager.Render(immediate_context.Get(), shader);
}

void Mine::DrawDebugPrimitive()
{
    // 基底クラスのデバッグプリミティブ描画
    Trap::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    // 当たり判定
    debugRenderer->DrawCylinder(position, radius, 1.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    // 攻撃範囲
    debugRenderer->DrawCylinder(position, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
}

//敵との衝突処理
void Mine::CollisionVsEnemies()
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
            Destroy();
            enemy->ApplyDamage(attack, 0.0f, false);
        }
    }
}