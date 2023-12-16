#include"Mine.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "Graphics.h"
#include "Camera.h"
#include "TrapManager.h"

Mine::Mine()
{

    model = std::make_unique<Model>(".\\resources\\Trap\\Mine\\Mine.fbx", true);
    model->ModelSerialize(".\\resources\\Trap\\Mine\\Mine.fbx");
    model->ModelRegister(".\\resources\\Trap\\Mine\\Mine.fbx");


    UpdateTransform(0, 0);
    model->UpdateBufferDara(transform);
    renderdata = model->GetBufferData();

    attack = 1;
    height = 5.0f;
    type = Trap::TrapType::TrapMine;

    // ホログラムシェーダー情報初期化
    HologramShaderDataInitialize(0.0f, 0.6f);
}
Mine::~Mine()
{

}

void Mine::Update(float elapsedTime)
{
    timer += elapsedTime;
    model->ShaderAdjustment(adjustMetalness, adjustSmoothness, glitchScale, timer, maxHeight);
    
    // ホログラムシェーダー更新処理
    UpdateHologramShader(elapsedTime, activateFlag);

    if (activateFlag)
    {
        hologramColor = { 0.0f, 1.0f, 0.0f };
        CollisionVsEnemies();
    }
    else
    {
        hologramColor = TrapManager::Instance().GetHologramColor();
    }
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
    shader->Draw(dc, model.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { hologramColor.x, hologramColor.y, hologramColor.z, 1.0f });
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
// デバッグ情報表示
void Mine::DrawDebugGUI()
{
    //トランスフォーム
    if (ImGui::CollapsingHeader("Mine", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::TreeNode("PBR"))
        {
            ImGui::SliderFloat("adjustMetalness", &adjustMetalness, -1.0f, 1.0f);
            ImGui::SliderFloat("adjustSmoothness", &adjustSmoothness, -1.0f, 1.0f);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Hologram"))
        {
            ImGui::SliderFloat("scanBorder", &scanBorder, minHeight, maxHeight);
            ImGui::SliderFloat("glowBorder", &glowBorder, minHeight, maxHeight);
            ImGui::SliderFloat("hologramBorder", &hologramBorder, minHeight, maxHeight);
            ImGui::SliderFloat("scanTimer", &scanTimer, -1.0f, 2.0f);
            ImGui::SliderFloat("glowTimer", &glowTimer, -1.0f, 2.0f);
            ImGui::SliderFloat("hologramTimer", &hologramTimer, -1.0f, 2.0f);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Glitch"))
        {
            //ImGui::SliderFloat("glitchSpeed", &glitchSpeed, 0.0f, 10.0f);
            ImGui::SliderFloat("glitchSpeed", &glitchSpeed, 1.0f, 50.0f);
            ImGui::SliderFloat("glitchIntensity", &glitchIntensity, 0.0f, 1.0f);
            ImGui::SliderFloat("glitchScale", &glitchScale, 1.0f, 50.0f);
            ImGui::TreePop();
        }
    }
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
