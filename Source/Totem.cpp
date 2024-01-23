#include "Totem.h"
#include "TrapManager.h"
#include "Graphics.h"
#include "Collision.h"
#include "EnemyManager.h"

// �R���X�g���N�^
Totem::Totem()
{
    model = std::make_unique<Model>(".\\resources\\Trap\\AttacheCase\\AttacheCase.fbx", true);
    model->ModelSerialize(".\\resources\\Trap\\AttacheCase\\AttacheCase.fbx");
    model->ModelRegister(".\\resources\\Trap\\AttacheCase\\AttacheCase.fbx", "Texture\\Albedo.png");

    scale.x = scale.y = scale.z = 3.0f;     // �T�C�Y�ݒ�
    radius = 2.0f;                          // ���a�ݒ�
    height = 5.0f;                          // �����ݒ�
    territoryRange = 10.0f;                 // �e���g���[�͈͐ݒ�
    attack = 1;                             // �_���[�W�ʐݒ�
    type = Trap::TrapType::TrapTotem;       // �g���b�v��ސݒ�
    survivalTime = 10.0f;                   // �������Ԑݒ�
    yUp = 0;

    // �z���O�����V�F�[�_�[��񏉊���
    HologramShaderDataInitialize(0.0f, 15.0f);

    UpdateTransform(0, 0);                      // �s��X�V����
    model->UpdateBufferData(transform);         // �`����X�V����
    transform = model->GetBufferTransform();	// �C���X�^���V���O���̕`����s��擾
    instancing = true;						    // �C���X�^���V���O��
}

// �X�V����
void Totem::Update(float elapsedTime)
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
        hologramColor = 2.0f;                                       // �z���O�����F(��)�ݒ�
        // �z���O�������I��������
        if (GetHologramTimer() >= 1.0f)
        {
            survivalTime -= elapsedTime;    // �������Ԍ���
            // �������Ԃ��c���Ă��Ȃ����
            if (survivalTime <= 0.0f)
            {
                glitchIntensity = 1.0f;     // �O���b�`�ݒ�
                brokenTimer -= elapsedTime; // �j�󎞂̔j���܂ł̎��Ԍ���
                // �j�󎞂̔j���܂ł̎��Ԃ��c���Ă��Ȃ����
                if (brokenTimer <= 0.0f)
                {
                    Destroy();  // �j��
                }
            }
            // �G�ƏՓ�
            CollisionVsEnemies();
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
void Totem::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
    shader->Draw(deviceContext, model.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { hologramColor, 0.0f, 0.0f, 0.0f });
}
// �f�o�b�O�v���~�e�B�u�`�揈��
void Totem::DrawDebugPrimitive()
{
    // ���N���X�̃f�o�b�O�v���~�e�B�u�`��
    Trap::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    //�Փ˔���p�̃f�o�b�N�~����`��
    //debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    // �꒣��͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(position, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
}
// �f�o�b�O���\��
void Totem::DrawDebugGUI()
{
    // �g�����X�t�H�[��
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

// �G�Ƃ̏Փˏ���
void Totem::CollisionVsEnemies()
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
            position, territoryRange, height,
            outPosition))
        {
            // �G�̗̑͂��c���Ă��Ȃ��A�|�C���g���Z�O�Ȃ�
            if (enemy->GetHealth() <= 0 && !enemy->GetAddPointFlag())
            {                
                TrapManager::Instance().SetTrapPoint(TrapManager::Instance().GetTrapPoint() + 1);   // �|�C���g���Z
                enemy->SetAddPointFlag(true);                                                       // �|�C���g���Z����
            }
        }
    }
}
