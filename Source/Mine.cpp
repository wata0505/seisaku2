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

    // �z���O�����V�F�[�_�[��񏉊���
    HologramShaderDataInitialize(0.0f, 0.6f);
}
Mine::~Mine()
{

}

void Mine::Update(float elapsedTime)
{
    timer += elapsedTime;
    model->ShaderAdjustment(adjustMetalness, adjustSmoothness, glitchScale, timer, maxHeight);
    
    // �z���O�����V�F�[�_�[�X�V����
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
    //���f���`����󂯓n��
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
    // ���N���X�̃f�o�b�O�v���~�e�B�u�`��
    Trap::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    // �����蔻��
    debugRenderer->DrawCylinder(position, radius, 1.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
    // �U���͈�
    debugRenderer->DrawCylinder(position, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
}
// �f�o�b�O���\��
void Mine::DrawDebugGUI()
{
    //�g�����X�t�H�[��
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

//�G�Ƃ̏Փˏ���
void Mine::CollisionVsEnemies()
{
    EnemyManager& enemyManager = EnemyManager::Instance();

    //�S�Ă̓G�Ƒ�������ŏՓˏ���
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = enemyManager.GetEnemy(i);
        //�Փˏ���
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
