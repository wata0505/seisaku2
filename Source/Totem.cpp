#include "Totem.h"
#include "TrapManager.h"
#include "Graphics.h"
#include "Collision.h"
#include "EnemyManager.h"

// コンストラクタ
Totem::Totem()
{
    model = std::make_unique<Model>(".\\resources\\Trap\\AttacheCase\\AttacheCase.fbx", true);
    model->ModelSerialize(".\\resources\\Trap\\AttacheCase\\AttacheCase.fbx");
    model->ModelRegister(".\\resources\\Trap\\AttacheCase\\AttacheCase.fbx", "Texture\\Albedo.png");

    scale.x = scale.y = scale.z = 3.0f;     // サイズ設定
    radius = 2.0f;                          // 半径設定
    height = 5.0f;                          // 高さ設定
    territoryRange = 10.0f;                 // テリトリー範囲設定
    attack = 1;                             // ダメージ量設定
    type = Trap::TrapType::TrapTotem;       // トラップ種類設定
    survivalTime = 10.0f;                   // 生存時間設定
    yUp = 0;

    // ホログラムシェーダー情報初期化
    HologramShaderDataInitialize(0.0f, 15.0f);

    UpdateTransform(0, 0);                      // 行列更新処理
    model->UpdateBufferData(transform);         // 描画情報更新処理
    transform = model->GetBufferTransform();	// インスタンシング化の描画情報行列取得
    instancing = true;						    // インスタンシング化
}

// 更新処理
void Totem::Update(float elapsedTime)
{
    // 経過時間加算
    progressTimer += elapsedTime;
    // シェーダー情報調整
    model->ShaderAdjustment(glitchScale, progressTimer, maxHeight);
    
    // ホログラムシェーダー更新処理
    UpdateHologramShader(elapsedTime, isActivate);

    // 設置後
    if (isActivate)
    {
        hologramColor = 2.0f;                                       // ホログラム色(緑)設定
        // ホログラムが終了したら
        if (GetHologramTimer() >= 1.0f)
        {
            survivalTime -= elapsedTime;    // 生存時間減少
            // 生存時間が残っていなければ
            if (survivalTime <= 0.0f)
            {
                glitchIntensity = 1.0f;     // グリッチ設定
                brokenTimer -= elapsedTime; // 破壊時の破棄までの時間減少
                // 破壊時の破棄までの時間が残っていなければ
                if (brokenTimer <= 0.0f)
                {
                    Destroy();  // 破棄
                }
            }
            // 敵と衝突
            CollisionVsEnemies();
        }
    }
    // 設置前
    else
    {
        hologramColor = TrapManager::Instance().GetHologramColor(); // 設置可不可色設定
    }
    
    UpdateTransform(0, 0);                  // 行列更新処理
    model->UpdateBufferData(transform);     // 描画情報更新処理
}

// 描画処理
void Totem::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
    shader->Draw(deviceContext, model.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { hologramColor, 0.0f, 0.0f, 0.0f });
}
// デバッグプリミティブ描画処理
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
// デバッグ情報表示
void Totem::DrawDebugGUI()
{
    // トランスフォーム
    if (ImGui::CollapsingHeader("Totem", ImGuiTreeNodeFlags_DefaultOpen))
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

// 敵との衝突処理
void Totem::CollisionVsEnemies()
{
    // 全ての敵と総当たりで衝突処理
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        // 衝突処理
        Enemy* enemy = enemyManager.GetEnemy(i);
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersectCylinderVsCylinder(
            enemy->GetPosition(), enemy->GetRadius(), enemy->GetHeight(),
            position, territoryRange, height,
            outPosition))
        {
            // 敵の体力が残っていなく、ポイント加算前なら
            if (enemy->GetHealth() <= 0 && !enemy->GetAddPointFlag())
            {                
                TrapManager::Instance().SetTrapPoint(TrapManager::Instance().GetTrapPoint() + 1);   // ポイント加算
                enemy->SetAddPointFlag(true);                                                       // ポイント加算完了
            }
        }
    }
}
