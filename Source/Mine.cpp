#include "Mine.h"
#include "TrapManager.h"
#include "Graphics.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "ParticleSystem.h"

// コンストラクタ
Mine::Mine()
{
    model = std::make_unique<Model>(".\\resources\\Trap\\Mine\\Mine.fbx", true);
    model->ModelSerialize(".\\resources\\Trap\\Mine\\Mine.fbx");
    model->ModelRegister(".\\resources\\Trap\\Mine\\Mine.fbx");

    radius = 0.7f;                          // 半径設定
    height = 5.0f;                          // 高さ設定
    territoryRange = 5.0f;                  // テリトリー範囲設定
    attack = 1;                             // ダメージ量設定
    type = Trap::TrapType::TrapMine;        // トラップ種類設定

    // ホログラムシェーダー情報初期化
    HologramShaderDataInitialize(0.0f, 0.6f);

    UpdateTransform(0, 0);                      // 行列更新処理
    model->UpdateBufferData(transform);         // 描画情報更新処理
    transform = model->GetBufferTransform();	// インスタンシング化の描画情報行列取得
    instancing = true;						    // インスタンシング化
}

// 更新処理
void Mine::Update(float elapsedTime)
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
        hologramColor = 2.0f;   // ホログラム色(緑)設定
        // ホログラムが終了したら
        if (GetHologramTimer() >= 1.0f)
        {
            // 敵と衝突
            if (CollisionVsEnemies())
            {
                ParticleSystem::Instance().BoomEffect(position, 3, int(EffectTexAll::EfTexAll::Flame), 20.0f, 0.5f, { NULL, NULL, 2, 1 });
                Destroy();  // 破棄
            }
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
void Mine::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
    shader->Draw(deviceContext, model.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { hologramColor, 0.0f, 0.0f, 1.0f });
}
// デバッグプリミティブ描画処理
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
    // トランスフォーム
    if (ImGui::CollapsingHeader("Mine", ImGuiTreeNodeFlags_DefaultOpen))
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
bool Mine::CollisionVsEnemies()
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
            position, radius, height,
            outPosition))
        {
            enemy->ApplyDamage(attack, 0.0f, false);
            return true;
        }
    }

    return false;
}
