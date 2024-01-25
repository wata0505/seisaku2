#pragma once
//#include "Model.h"
//#include "Object.h"
//#include "ParticleSpriteShader.h"
#include "Shader.h"
struct PVConstants
{
	DirectX::XMFLOAT3 Position;     // ���W�l
	DirectX::XMFLOAT3 Velocity;     // ���x
	DirectX::XMFLOAT4 ParticleSize; // �T�C�Y
	float             Speed;        // �����x
	int               type;         // �p�[�e�B�N���^�C�v
	float             Timer;       

};

class ParticleSprite
{
public:
	ParticleSprite(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir, int spritetype, int movetype, int textype, int Max, float lifetimer, float lenght = 0,bool lifeflag = true,float speed = 0.05,float size = 0.005, DirectX::XMFLOAT4 materialColor = { 1.0,0.0,0.0,1 });
	~ParticleSprite();

	//�X�V����
	void Update(float elapsedTime);


	//����
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position, int movetype, int spritetype, float lifeTimer, float speed, DirectX::XMFLOAT4 materialColor = { 1,0,0,1 });

	//�p�[�e�B�N�������^�C�v
	enum  MoveSpriteType
	{
		Slashing,          //�a��
		Diffusion,		   //�g�U
		Convergence,	   //����
		Slash,			   //�a���q�b�g
		Expansion,		   //�g��
		WeponConvergence,  //�������
		Corruption,		   //��
		Chile			   //�o
	};
	//�p�[�e�B�N�����f���^�C�v
	enum  SpriteType
	{
		ParticleSoft,  //��
		ParticleLine,  //��
		ParticleImpact,//�Ռ�
		ParticleWind,  //��
		ParticleSquare,//�l�p
		ParticleBurst,//�o�[�X�g
		ParticleTriangle,//�l�p,
		ParticleHart,//�l�p,


		ParticleMax
	};
	
private:
	//������������ݒ�
	HRESULT CreateWeponConvergenceResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
	//�g�U�����ݒ�
	HRESULT CreateDiffusionResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir,float speed, float lenght,float size);

	HRESULT CreateSlashingResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
	//���������ݒ�
	HRESULT CreateConvergenceResource(DirectX::XMFLOAT3 pos);
	//�a���G�t�F�N�g�����ݒ�
	HRESULT CreateSlashResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
	//�g�叉���ݒ�
	HRESULT CreateExpansionResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir,float leght);
	//�������ݒ�
	HRESULT CreateCorruptionResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir);
	//�o�����ݒ�
	HRESULT CreateChileResource(DirectX::XMFLOAT3 pos);
	//���\�[�X����
	HRESULT CreateResource(PVConstants* posVertex);

	// �j��
	void Destroy();

private:
	
	float lifeTimer = { 0 };
	float speed[5] = { 15.0f,7,7,0,0 };
	bool lifeflag = false;

	int type = 0;

public:
	Microsoft::WRL::ComPtr<ID3D11Buffer> gPBuffer[2] = { NULL, NULL }; // �o�b�t�@ ���\�[�X
	// �萔�o�b�t�@�t�H�[�}�b�g
	Microsoft::WRL::ComPtr<ID3D11Buffer> verBufferDrawFrom;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  gPSRV[2] = { NULL, NULL }; // �V�F�[�_ ���\�[�X �r���[
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> gPUAV[2] = { NULL, NULL }; // �A���I�[�_�[�h �A�N�Z�X �r���[

	DirectX::XMFLOAT3 uvStatus = { 0,0,0 };
	DirectX::XMFLOAT3 up = { 0,0,0 };
	DirectX::XMFLOAT3 down = { 0,0,0 };
	DirectX::XMFLOAT4 materialColor = { 1,0,0,1 };
	int chainA = 0;//�o�b�t�@�[�̐؂�ւ�
	int chainB = 1;//�o�b�t�@�[�̐؂�ւ�

	int spriteType = 0;
	int texType = 0;
	int moveType = 0;
	int max = 0;
	float lenght = 0;
	//�[�x�X�e���V��
	int depth = 0;
};