#include "Decoy.h"
#include "TrapManager.h"
#include "Graphics.h"
//#include "Collision.h"
//#include "EnemyManager.h"
//#include "Camera.h"

// �R���X�g���N�^
Decoy::Decoy()
{
    model = std::make_unique<Model>(".\\resources\\Trap\\Decoy\\Decoy.fbx", true);
    model->ModelSerialize(".\\resources\\Trap\\Decoy\\Decoy.fbx");
    model->ModelRegister(".\\resources\\Trap\\Decoy\\Decoy.fbx", "Texture\\Albedo.png");
    
    scale.x = scale.y = scale.z = 3.0f;     // �T�C�Y�ݒ�
    radius = 1.5f;                          // ���a�ݒ�
    height = 5.0f;                          // �����ݒ�
    maxHealth = 100;                        // �ő�̗͐ݒ�
    health = maxHealth;                     // �̗͐ݒ�
    territoryRange = 10.0f;                 // �e���g���[�͈͐ݒ�
    type = Trap::TrapType::TrapDecoy;       // �g���b�v��ސݒ�
    yUp = 0;

    // �z���O�����V�F�[�_�[��񏉊���
    HologramShaderDataInitialize(-1.2f, 1.4f);

    UpdateTransform(0, 0);                      // �s��X�V����
    model->UpdateBufferData(transform);         // �`����X�V����
    transform = model->GetBufferTransform();	// �C���X�^���V���O���̕`����s��擾
    instancing = true;					        // �C���X�^���V���O��
}

// �X�V����
void Decoy::Update(float elapsedTime)
{
    // �o�ߎ���
    progressTimer += elapsedTime;
    // �V�F�[�_�[��񒲐�
    model->ShaderAdjustment(glitchScale, progressTimer, maxHeight);
    
    // �z���O�����V�F�[�_�[�X�V����
    UpdateHologramShader(elapsedTime, isActivate);

    // �̗͂��c���Ă��Ȃ����
    if (health <= 0)
    {
        glitchIntensity = 1.0f;     // �O���b�`�ݒ�        
        brokenTimer -= elapsedTime; // �j�󎞂̔j���܂ł̎��Ԍ���
        // �j�󎞂̔j���܂ł̎��Ԃ��c���Ă��Ȃ����
        if (brokenTimer <= 0.0f)
        {
            Destroy();  // �j��
        }
    }

    // �ݒu��
    if (isActivate)
    {
        hologramColor = 2.0f;                                       // �z���O�����F(��)�ݒ�
    }
    // �ݒu�O
    else
    {
        hologramColor = TrapManager::Instance().GetHologramColor(); // �ݒu�s�F�ݒ�
    }
    
    UpdateTransform(0, 0);                  // �s��X�V����
    model->UpdateBufferData(transform);     // �`����X�V����

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

// �`�揈��
void Decoy::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
    shader->Draw(deviceContext, model.get(), { glitchIntensity, scanBorder, glowBorder, hologramBorder }, { hologramColor, 0.0f, 0.0f, 0.0f });
}
// �f�o�b�O�v���~�e�B�u�`�揈��
void Decoy::DrawDebugPrimitive()
{
    // ���N���X�̃f�o�b�O�v���~�e�B�u�`��
    Trap::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();

    // �Փ˔���p�̃f�o�b�N�~����`��
    debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));

    // �꒣��͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
}
// �f�o�b�O���\��
void Decoy::DrawDebugGUI()
{
    // �g�����X�t�H�[��
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