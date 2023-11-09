#include"Mine.h"
#include "EnemyManager.h"
#include "Collision.h"
#include "Graphics.h"
Mine::Mine()
{
    //model = std::make_unique<Model>(".\\resources\\Bug Creature\\Bug9.fbx", true);
    //model->ModelSerialize(".\\resources\\Bug Creature\\Bug9.fbx");
    //model->ModelRegister(".\\resources\\Bug Creature\\Bug9.fbx");

    model = std::make_unique<Model>(".\\resources\\Sentorygun\\gunmotiongun1.fbx", true);
    model->ModelSerialize(".\\resources\\Sentorygun\\gunmotiongun1.fbx");
    model->ModelRegister(".\\resources\\Sentorygun\\gunmotiongun1.fbx");


    UpdateTransform(0, 0);
    model->UpdateBufferDara(transform);
    renderdata = model->GetBufferData();

    attack = 100.0f;

}
Mine::~Mine()
{

}

void Mine::Update(float elapsedTime)
{
    CollisionVsEnemies();
    UpdateTransform(0, 0);
    model->UpdateBufferDara(transform);
    //���f���`����󂯓n��
    renderdata = model->GetBufferData();
}

void Mine::Render(ID3D11DeviceContext* dc, ModelShader* shader)
{
    shader->Draw(dc, model.get(), renderdata);
}

void Mine::DrawDebugPrimitive()
{
    // ���N���X�̃f�o�b�O�v���~�e�B�u�`��
    Trap::DrawDebugPrimitive();

    DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
    // �꒣��͈͂��f�o�b�O�~���`��
    debugRenderer->DrawCylinder(territoryOrigin, territoryRange, 1.0f, DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));
}

//�v���C���[�ƓG�Ƃ̏Փˏ���
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
            position, radius, height,
            outPosition)) 
        {
            Destroy();
            enemy->ApplyDamage(attack, 0.0f, false);
        }
    }
}