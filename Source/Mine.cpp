#include "Mine.h"
#include "TrapManager.h"
#include "Graphics.h"
#include "Collision.h"
#include "EnemyManager.h"
#include "ParticleSystem.h"

// �R���X�g���N�^
Mine::Mine()
{
    model = std::make_unique<Model>(".\\resources\\Trap\\Mine\\Mine.fbx", true);
    model->ModelSerialize(".\\resources\\Trap\\Mine\\Mine.fbx");
    model->ModelRegister(".\\resources\\Trap\\Mine\\Mine.fbx");

    radius = 0.7f;                          // ���a�ݒ�
    height = 5.0f;                          // �����ݒ�
    territoryRange = 5.0f;                  // �e���g���[�͈͐ݒ�
    attack = 1;                             // �_���[�W�ʐݒ�
    type = Trap::TrapType::TrapMine;        // �g���b�v��ސݒ�

    // �z���O�����V�F�[�_�[��񏉊���
    HologramShaderDataInitialize(0.0f, 0.6f);

    UpdateTransform(0, 0);                      // �s��X�V����
    model->UpdateBufferData(transform);         // �`����X�V����
    transform = model->GetBufferTransform();	// �C���X�^���V���O���̕`����s��擾
    instancing = true;						    // �C���X�^���V���O��
}

// �X�V����
void Mine::Update(float elapsedTime)
{
    // �o�ߎ��ԉ��Z
    progressTimer += elapsedTime;
    // �V�F�[�_�[��񒲐�
    model->ShaderAdjustment(glitchScale, progressTimer, maxHeight);
    
    // �z���O�����V�F�[�_�[�X�V����
    UpdateHologramShader(elapsedTime, isActivate);

    // �ݒu��
    if (isActivate)
    {
        hologramColor = 2.0f;   // �z���O�����F(��)�ݒ�
        // �z���O�������I��������
        if (GetHologramTimer() >= 1.0f)
        {
            // �G�ƏՓ�
            if (CollisionVsEnemies())
            {
                ParticleSystem::Instance().BoomEffect(position, 3, int(EffectTexAll::EfTexAll::Flame), 20.0f, 0.5f, { NULL, NULL, 2, 1 });
                Destroy();  // �j��
            }
        }
    }
    // �ݒu�O
    else
    {
        hologramColor = TrapManager::Instance().GetHologramColor(); // �ݒu�s�F�ݒ�
    }

    UpdateTransform(0, 0);                  // �s��X�V����
    model->UpdateBufferData(transform);     // �`����X�V����
}

// �`�揈��
void Mine::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
    shader->Draw(deviceContext, model.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { hologramColor, 0.0f, 0.0f, 1.0f });
}
// �f�o�b�O�v���~�e�B�u�`�揈��
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
    // �g�����X�t�H�[��
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

// �G�Ƃ̏Փˏ���
bool Mine::CollisionVsEnemies()
{
    // �S�Ă̓G�Ƒ�������ŏՓˏ���
    EnemyManager& enemyManager = EnemyManager::Instance();
    int enemyCount = enemyManager.GetEnemyCount();
    for (int i = 0; i < enemyCount; i++)
    {
        // �Փˏ���
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
