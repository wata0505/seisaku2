#pragma once
#pragma once
#include "Model.h"
#include "Object.h"
#include "EffectAll.h"
#include "LightManager.h"
//�^�C�v�擾
// 0   ���i
// 1   ��]
// 2   ��
enum  FireBallType
{
	FirBallStraight,
	FirBallNULLType,
};

//���i�e��
class FireBall :public Object
{
public:
	FireBall(ObjectManager* manager,float size = 0.3f);
	~FireBall() override;

	//�X�V����
	void Update(float elapsedTime)override;


	//�`�揈��
	void Render(ID3D11DeviceContext* dc, ModelShader* shader)override;

	void TrailRender(RenderContext& rc, SwordTrailShader* shader)override {};

	//����
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, float angle);
private:
	void StraightUpdate(float elapsedTime);

	
private:
	Effekseer::Handle handle;
	float speed = 20.0f;
	float lifeTimer = 4.0f;
	float angle = 0;
	Light* light;
};