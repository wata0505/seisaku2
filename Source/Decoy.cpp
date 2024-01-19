#include"Decoy.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "Graphics.h"
#include "TrapManager.h"
#include "Camera.h"

Decoy::Decoy()
{

    model = std::make_unique<Model>(".\\resources\\Trap\\Decoy\\Decoy.fbx", true);
    model->ModelSerialize(".\\resources\\Trap\\Decoy\\Decoy.fbx");
    model->ModelRegister(".\\resources\\Trap\\Decoy\\Decoy.fbx", "Texture\\Albedo.png");


    scale.x = scale.y = scale.z = 3.0f;

    UpdateTransform(0, 0);
    model->UpdateBufferDara(transform);
    renderdata = model->GetBufferData();
    
    height = 5;
    maxHealth = 100;
    health = maxHealth;
    territoryRange = 10.0f;
    radius = 1.5f;
    type = Trap::TrapType::TrapDecoy;

    // ホログラムシェーダー情報初期化
    HologramShaderDataInitialize(-1.2f, 1.4f);
}
Decoy::~Decoy()
{

}

void Decoy::Update(float elapsedTime)
{
    timer += elapsedTime;

    model->ShaderAdjustment(adjustMetalness, adjustSmoothness, glitchScale, timer, maxHeight);
    
    // ホログラムシェーダー更新処理
    UpdateHologramShader(elapsedTime, activateFlag);

    if (health <= 0)
    {
        Destroy();
    }
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
    hpRenderFlag = Collision::IntersectFanVsSphere(
        Camera::Instance().GetEye(),
        Camera::Instance().GetFront(),
        Camera::Instance().GetFovY(),
        Camera::Instance().GetFarZ(),
        position,
        radius);
}

void Decoy::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
    shader->Draw(dc, model.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { hologramColor.x, hologramColor.y, hologramColor.z, 0.0f });
}
void Decoy::Afterimagerender(Microsoft::WRL::ComPtr<ID3D11DeviceContext> immediate_context, ModelShader* shader)
{

}
void Decoy::DrawDebugPrimitive()
{
    // 基底クラスのデバッグプリミティブ描画
    Trap::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //衝突判定用のデバック円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));


    // 縄張り範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
}
// デバッグ情報表示
void Decoy::DrawDebugGUI()
{
    //トランスフォーム
    if (ImGui::CollapsingHeader("Decoy", ImGuiTreeNodeFlags_DefaultOpen))
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
void Decoy::CollisionVsEnemies()
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


        }
    }
}
