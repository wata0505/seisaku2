#include "Decoy.h"
#include "TrapManager.h"
#include "Graphics.h"
//#include "Collision.h"
//#include "EnemyManager.h"
//#include "Camera.h"

// コンストラクタ
Decoy::Decoy()
{
    model = std::make_unique<Model>(".\\resources\\Trap\\Decoy\\Decoy.fbx", true);
    model->ModelSerialize(".\\resources\\Trap\\Decoy\\Decoy.fbx");
    model->ModelRegister(".\\resources\\Trap\\Decoy\\Decoy.fbx", "Texture\\Albedo.png");
    
    scale.x = scale.y = scale.z = 3.0f;     // サイズ設定
    radius = 1.5f;                          // 半径設定
    height = 5.0f;                          // 高さ設定
    maxHealth = 100;                        // 最大体力設定
    health = maxHealth;                     // 体力設定
    territoryRange = 10.0f;                 // テリトリー範囲設定
    type = Trap::TrapType::TrapDecoy;       // トラップ種類設定
    yUp = 0;

    // ホログラムシェーダー情報初期化
    HologramShaderDataInitialize(-1.2f, 1.4f);

    UpdateTransform(0, 0);                      // 行列更新処理
    model->UpdateBufferData(transform);         // 描画情報更新処理
    transform = model->GetBufferTransform();	// インスタンシング化の描画情報行列取得
    instancing = true;					        // インスタンシング化
}

// 更新処理
void Decoy::Update(float elapsedTime)
{
    // 経過時間
    progressTimer += elapsedTime;
    // シェーダー情報調整
    model->ShaderAdjustment(glitchScale, progressTimer, maxHeight);
    
    // ホログラムシェーダー更新処理
    UpdateHologramShader(elapsedTime, isActivate);

    // 体力が残っていなければ
    if (health <= 0)
    {
        glitchIntensity = 1.0f;     // グリッチ設定        
        brokenTimer -= elapsedTime; // 破壊時の破棄までの時間減少
        // 破壊時の破棄までの時間が残っていなければ
        if (brokenTimer <= 0.0f)
        {
            Destroy();  // 破棄
        }
    }

    // 設置後
    if (isActivate)
    {
        hologramColor = 2.0f;                                       // ホログラム色(緑)設定
    }
    // 設置前
    else
    {
        hologramColor = TrapManager::Instance().GetHologramColor(); // 設置可不可色設定
    }
    
    UpdateTransform(0, 0);                  // 行列更新処理
    model->UpdateBufferData(transform);     // 描画情報更新処理

#if 0
    hpRenderFlag = Collision::IntersectFanVsSphere(
        Camera::Instance().GetEye(),
        Camera::Instance().GetFront(),
        Camera::Instance().GetFovY(),
        Camera::Instance().GetFarZ(),
        position,
        radius);
#endif
}

// 描画処理
void Decoy::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
    shader->Draw(deviceContext, model.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { hologramColor, 0.0f, 0.0f, 0.0f });
}
// デバッグプリミティブ描画処理
void Decoy::DrawDebugPrimitive()
{
    // 基底クラスのデバッグプリミティブ描画
    Trap::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // 衝突判定用のデバック円柱を描画
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    // 縄張り範囲をデバッグ円柱描画
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
}
// デバッグ情報表示
void Decoy::DrawDebugGUI()
{
    // トランスフォーム
    if (ImGui::CollapsingHeader("Decoy", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::TreeNode("Hologram"))
        {
            ImGui::SliderFloat("scanTimer", &scanTimer, -1.0f, 2.0f);
            ImGui::SliderFloat("glowTimer", &glowTimer, -1.0f, 2.0f);
            ImGui::SliderFloat("hologramTimer", &hologramTimer, -1.0f, 2.0f);
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Glitch"))
        {
            ImGui::SliderFloat("glitchIntensity", &glitchIntensity, 0.0f, 1.0f);
            ImGui::SliderFloat("glitchScale", &glitchScale, 1.0f, 50.0f);
            ImGui::TreePop();
        }
    }
}