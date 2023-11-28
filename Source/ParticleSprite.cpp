#include"ParticleSprite.h"
#include"ParticleSpriteManager.h"
#include"Graphics.h"
#include"Calculation.h"
#include"misc.h"
#include"Player.h"

// �R���X�g���N�^
ParticleSprite::ParticleSprite(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir, int spritetype, int movetype, int textype, int Max, float lifetimer,float lenght,bool lifeflag,float speed,float size, DirectX::XMFLOAT4 materialColor)//���N���X�̃R���X�g���N�^�Ăяo��
{
	this->materialColor = materialColor;
	this->lifeflag = lifeflag;
	max = Max;
	ID3D11Device* device = Graphics::Instance().GetDevice();
	lifeTimer = lifetimer;
	spriteType = spritetype;
	texType = textype;
	moveType = movetype;
	this->lenght = lenght;
	D3D11_BUFFER_DESC desc;
	::memset(&desc, 0, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.ByteWidth = sizeof(PVConstants) * Max;
	desc.StructureByteStride = 0;
	depth = DepthStencilState::ONOFF;
	struct PVConstants* posVertex = new PVConstants[Max];
	
	// ���_�o�b�t�@�̃T�u���\�[�X�̒�`(�o�b�t�@�[���\�[�X�ł���������Ă�j
	D3D11_SUBRESOURCE_DATA vSubData;//�������p�f�[�^���쐬
	vSubData.pSysMem = posVertex;  // �o�b�t�@�E�f�[�^�̏����l
	vSubData.SysMemPitch = 0;
	vSubData.SysMemSlicePitch = 0;

	// �`��p���_���̓o�b�t�@�̍쐬�@�@�������p�f�[�^�𑗂��Ă���
	HRESULT hr = device->CreateBuffer(&desc, &vSubData, verBufferDrawFrom.GetAddressOf());
	//device->CreateBuffer(&desc, 0, sceneBuffers.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	//�����^�C�v���Ƃɏ����l�ݒ�
	switch (movetype)
	{
	case Slashing:
		CreateSlashingResource(pos, dir);
		break;
	case Diffusion:
		CreateDiffusionResource(pos, dir,speed,lenght,size);
		break;
	case Convergence:
		CreateConvergenceResource(pos);
		break;
	case Slash:
		CreateSlashResource(pos, dir);
		break;
	case Expansion:
		CreateExpansionResource(pos, dir,lenght);
		break;
	case WeponConvergence:
		CreateWeponConvergenceResource(pos, dir);
		break;
	case Corruption:
		CreateCorruptionResource(pos, dir);
		break;
	case Chile:
		CreateChileResource(pos);
		break;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC DescSRV;
	ZeroMemory(&DescSRV, sizeof(DescSRV));
	DescSRV.Format = DXGI_FORMAT_UNKNOWN;
	DescSRV.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;

	DescSRV.Buffer.ElementWidth = max; // �f�[�^��

										// �V�F�[�_ ���\�[�X �r���[�̍쐬
	hr = device->CreateShaderResourceView(gPBuffer[0].Get(), &DescSRV, gPSRV[0].GetAddressOf());

	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));


	hr = device->CreateShaderResourceView(gPBuffer[1].Get(), &DescSRV, gPSRV[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// �A���I�[�_�[�h�E�A�N�Z�X�E�r���[
	D3D11_UNORDERED_ACCESS_VIEW_DESC descUAV;
	ZeroMemory(&descUAV, sizeof(descUAV));
	descUAV.Format = DXGI_FORMAT_UNKNOWN;
	descUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descUAV.Buffer.NumElements = Max; // �f�[�^��

											// �A���I�[�_�[�h�E�A�N�Z�X�E�r���[�̍쐬
	hr = device->CreateUnorderedAccessView(gPBuffer[INPUT].Get(), &descUAV, gPUAV[INPUT].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	hr = device->CreateUnorderedAccessView(gPBuffer[OUTPUT].Get(), &descUAV, gPUAV[OUTPUT].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	ParticleSpriteManager::Instance().Register(this);
}

// �f�X�g���N�^
ParticleSprite::~ParticleSprite()
{

	
}
HRESULT ParticleSprite::CreateDiffusionResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir,float speed, float lenght,float size) {
	HRESULT hr;

	//�������p�f�[�^
	ID3D11Device* device = Graphics::Instance().GetDevice();
	struct PVConstants* posVertex = new PVConstants[max];
	DirectX::XMFLOAT3 Vec;
	Vec.x = dir.x - pos.x;
	Vec.y = dir.y - pos.y;
	Vec.z = dir.z - pos.z;
	float dist = sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z);
	Vec.x /= dist;
	Vec.y /= dist;
	Vec.z /= dist;
	float len = dist / max;
	for (int i = 0; i < max; ++i) {
		if (dir.x == 0 && dir.y == 0 && dir.z == 0) {//������̒l�������ĂȂ���Όv�Z���Ȃ�
			posVertex[i].Position = pos;
		}
		else {
			posVertex[i].Position.x = pos.x + Vec.x * (len * i);
			posVertex[i].Position.y = pos.y + Vec.y * (len * i);
			posVertex[i].Position.z = pos.z + Vec.z * (len * i);
		}
		//�����_���̕����ɔ�юU��
		float pitch = DirectX::XMConvertToRadians(rand() % 360);
		float yaw = DirectX::XMConvertToRadians(rand() % 180);
		DirectX::XMFLOAT3 dir;
		dir.x = cosf(pitch) * sinf(yaw);
		dir.y = sin(pitch);
		dir.z = cosf(pitch) * cosf(yaw);
		posVertex[i].Position = Vector3::PosDir(posVertex[i].Position, dir, lenght);
		posVertex[i].Velocity = dir;//���x
		if (i % 2) {
			posVertex[i].Speed = (40) * speed;//speed;//���x
		}
		else
		{
			posVertex[i].Speed = (rand() % 50 + 20) * speed;//
		}
		//�T�C�Y
		posVertex[i].ParticleSize.x = posVertex[i].ParticleSize.y = posVertex[i].ParticleSize.z = posVertex[i].ParticleSize.w = size;
		posVertex[i].type = Diffusion;
		posVertex[i].Timer = rand() % 50 + i;
	}
	uvStatus.x = 1.5;//�f�B�]���u�̂�����O�`�R
	// ���\�[�X�̐ݒ�
	hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;
}

HRESULT ParticleSprite::CreateSlashingResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir) {
	HRESULT hr;

	//�������p�f�[�^
	ID3D11Device* device = Graphics::Instance().GetDevice();
	struct PVConstants* posVertex = new PVConstants[max];
	for (int i = 0; i < max; ++i) {
		//���������g�U����悤�ɐݒ�
		posVertex[i].Position = pos;
		float angleX = DirectX::XMConvertToRadians(rand() % 5 - 2.5);
		float angleY = DirectX::XMConvertToRadians(rand() % 5 - 2.5);
		DirectX::XMFLOAT3 Vec;
		Vec.y = sin(NULL) - sin(angleY);
		Vec.x = cosf(NULL) * sinf(NULL) - cosf(angleY) * sinf(angleX);
		Vec.z = cosf(NULL) * cosf(NULL) - cosf(angleY) * cosf(angleX);
		Vec = Vector3::Add(dir, Vec);
		float speed = (rand() % 100 + 1)*0.001;
		posVertex[i].Velocity = Vec;//���x
		posVertex[i].Speed = speed;//���x
		posVertex[i].ParticleSize.x = posVertex[i].ParticleSize.y = posVertex[i].ParticleSize.z = posVertex[i].ParticleSize.w = 0.05;
		posVertex[i].type = Slashing;

	}
	// ���\�[�X�̐ݒ�
	hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;
}
HRESULT ParticleSprite::CreateWeponConvergenceResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 endPos) {
	HRESULT hr;
	//�������p�f�[�^
	ID3D11Device* device = Graphics::Instance().GetDevice();
	struct PVConstants* posVertex = new PVConstants[max];
	for (int i = 0; i < max; ++i) {
		 //�����_���Ń|�W�V����������
		float pitch = DirectX::XMConvertToRadians(rand() % 360);
		float yaw = DirectX::XMConvertToRadians(rand() % 180);
		DirectX::XMFLOAT3 dir;
		dir.x = cosf(pitch) * sinf(yaw);
		dir.y = sin(pitch);
		dir.z = cosf(pitch) * cosf(yaw);
		float lenght = this->lenght;
		posVertex[i].Position.x =pos.x + dir.x * lenght;
		posVertex[i].Position.y =pos.y + dir.y * lenght;
		posVertex[i].Position.z =pos.z + dir.z * lenght;
		float speed = 0.05;
		posVertex[i].Velocity = dir;//���x
		posVertex[i].Speed = speed;//���x
		posVertex[i].ParticleSize.x = posVertex[i].ParticleSize.y = posVertex[i].ParticleSize.z = posVertex[i].ParticleSize.w = 0.02;
		posVertex[i].type = WeponConvergence;
		posVertex[i].Timer = 1;

	}
	uvStatus.x = 1.5;//�f�B�]���u�̂�����O�`�R
	// ���\�[�X�̐ݒ�
	hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;
}

HRESULT ParticleSprite::CreateConvergenceResource(DirectX::XMFLOAT3 pos) {
	HRESULT hr;
	//�������p�f�[�^
	ID3D11Device* device = Graphics::Instance().GetDevice();
	struct PVConstants* posVertex = new PVConstants[max];
	for (int i = 0; i < max; ++i) {
		posVertex[i].Position = pos;
		float pitch = DirectX::XMConvertToRadians(rand() % 360);
		float yaw = DirectX::XMConvertToRadians(rand() % 180);
		DirectX::XMFLOAT3 dir;
		dir.x = cosf(pitch) * sinf(yaw);
		dir.y = sin(pitch);
		dir.z = cosf(pitch) * cosf(yaw);
		int len = lenght;
		posVertex[i].Position.x -= dir.x * len;
		posVertex[i].Position.y -= dir.y * len;
		posVertex[i].Position.z -= dir.z * len;
		float speed = 0.12;
		posVertex[i].Velocity = pos;//���x
		posVertex[i].Speed = speed;//���x
		posVertex[i].ParticleSize.x = posVertex[i].ParticleSize.y = posVertex[i].ParticleSize.z = posVertex[i].ParticleSize.w = 0.02;
		posVertex[i].type = Convergence;
		posVertex[i].Timer = 1;
	}
	// ���\�[�X�̐ݒ�
	hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;
}
HRESULT ParticleSprite::CreateSlashResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir) {
	//materialColor = { 1,1,1,1 };
	
	//�������p�f�[�^
	ID3D11Device* device = Graphics::Instance().GetDevice();
	struct PVConstants* posVertex = new PVConstants[max];
	for (int i = 0; i < max; ++i) {
		posVertex[i].Position = pos;
		float speed = 0.15;
		posVertex[i].Velocity = dir;//���x
		posVertex[i].Speed = speed;//���x
		posVertex[i].ParticleSize.x = 0.8;
		posVertex[i].ParticleSize.w  = 5.5;
		posVertex[i].ParticleSize.y = -5.5;
		posVertex[i].type = Slash;
	}
	uvStatus.x = 1.5;//�f�B�]���u�̂�����O�`�R
	depth = DepthStencilState::OFFOFF;//�[�x�e�X�g
	// ���\�[�X�̐ݒ�
	HRESULT hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;
}
HRESULT ParticleSprite::CreateExpansionResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir,float leght) {
	materialColor = { 1,1,1,0.4};
	//�������p�f�[�^
	ID3D11Device* device = Graphics::Instance().GetDevice();
	struct PVConstants* posVertex = new PVConstants[max];
	for (int i = 0; i < max; ++i) {
		posVertex[i].Position = pos;
		float speed = 0.1 * i + leght;
		posVertex[i].Velocity = dir;//���x
		posVertex[i].Speed = speed;//���x
		posVertex[i].ParticleSize.x = 0;
		posVertex[i].ParticleSize.w = 0;
		posVertex[i].ParticleSize.y = 0;
		posVertex[i].type = Expansion;
		posVertex[i].Timer = 1;
	}
	// ���\�[�X�̐ݒ�
	HRESULT hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;
}
HRESULT ParticleSprite::CreateCorruptionResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 endPos) {
	HRESULT hr;
	//�������p�f�[�^
	ID3D11Device* device = Graphics::Instance().GetDevice();
	struct PVConstants* posVertex = new PVConstants[max];
	//�ǂ�����ǂ��܂ł̒��A�����_���ňʒu������
	DirectX::XMFLOAT3 Vec;
	Vec.x = endPos.x - pos.x;
	Vec.y = endPos.y - pos.y;
	Vec.z = endPos.z - pos.z;
	float dist = sqrtf(Vec.x * Vec.x + Vec.y * Vec.y + Vec.z * Vec.z);
	Vec.x /= dist;
	Vec.y /= dist;
	Vec.z /= dist;
	float len = dist / max;
	len = len * (rand() % max);
	for (int i = 0; i < max; ++i) {
		posVertex[i].Position.x = 0;
		//������Ƀ����_���Ŕ��
		float pitch = DirectX::XMConvertToRadians(90 + (rand() % 45 - 22));
		float yaw = DirectX::XMConvertToRadians(rand() % 360);
		posVertex[i].Position.y = 0;
		posVertex[i].Position.z = 0;
		DirectX::XMFLOAT3 dir;
		dir.x = cosf(pitch) * sinf(yaw);
		dir.y = sin(pitch);
		dir.z = cosf(pitch) * cosf(yaw);
		int Length = rand() % 10 + 5;
		float speed = (rand() % 100 + 1) * 0.001;
		posVertex[i].Velocity = dir;//���x
		posVertex[i].Speed = speed;//���x
		posVertex[i].ParticleSize.x = posVertex[i].ParticleSize.y = posVertex[i].ParticleSize.w = 0.01;
		posVertex[i].ParticleSize.z = len;//�ʒu����
		posVertex[i].type = Corruption;
		posVertex[i].Timer = 0;
	}
	// ���\�[�X�̐ݒ�
	hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;

}

HRESULT ParticleSprite::CreateChileResource(DirectX::XMFLOAT3 pos) {
	HRESULT hr;
	//�������p�f�[�^
	ID3D11Device* device = Graphics::Instance().GetDevice();
	struct PVConstants* posVertex = new PVConstants[max];
	DirectX::XMFLOAT3 Vec;
	for (int i = 0; i < max; ++i) {
		int length = rand() % 1000;
		float angleX = DirectX::XMConvertToRadians(rand() % 360);
		float angleY = DirectX::XMConvertToRadians(90 + (rand() % 180 - 90));
		Vec.x = sinf(angleX);
		Vec.z = cosf(angleX);
		posVertex[i].Position.x = pos.x + Vec.x * length;
		posVertex[i].Position.y = pos.y + rand() % 20;
		posVertex[i].Position.z = pos.z + Vec.z * length;
		posVertex[i].Velocity = Vec;//���x
		posVertex[i].Speed = -0.5f;//���x
		posVertex[i].ParticleSize.x = posVertex[i].ParticleSize.y = posVertex[i].ParticleSize.w = 0.02;
		posVertex[i].ParticleSize.z = 0;
		posVertex[i].type = Chile;
		posVertex[i].Timer = 1;
	}

	// ���\�[�X�̐ݒ�
	hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;

}
HRESULT ParticleSprite::CreateResource(PVConstants* posVertex) {
	
	ID3D11Device* device = Graphics::Instance().GetDevice();
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = max * sizeof(PVConstants); // �o�b�t�@ �T�C�Y
	desc.Usage = D3D11_USAGE_DEFAULT;//�X�e�[�W�̓��o�͂�OK�BGPU�̓��o��OK�B
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // �\�����o�b�t�@
	desc.StructureByteStride = sizeof(PVConstants);

	D3D11_SUBRESOURCE_DATA SubResource;//�T�u���\�[�X�̏������p�f�[�^���`
	SubResource.pSysMem = posVertex;
	SubResource.SysMemPitch = 0;
	SubResource.SysMemSlicePitch = 0;

	// �ŏ��̓��̓��\�[�X(�f�[�^������������)
	HRESULT hr = device->CreateBuffer(&desc, &SubResource, gPBuffer[0].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// �ŏ��̏o�̓��\�[�X�i�������p�f�[�^�͕K�v�Ȃ��j
	device->CreateBuffer(&desc, &SubResource, gPBuffer[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	return S_OK;

}
//�X�V����
void ParticleSprite::Update(float elapsedTime)
{
	Player& player = Player::Instance();
	//��������
	switch (moveType)
	{
	case Slashing:
		
		break;
	case Diffusion:
		
		break;
	case Convergence:
	
		break;
	case Slash:		
		break;
	case Expansion:
		break;
	case WeponConvergence:
		up =   player.GetTailPos();
		down = player.GetPatternPos();
		break;
	case Corruption:
		up =  player.GetTailPos();
		down  = player.GetPatternPos();
		//if (player.GetWeponChange() || player.GetUseWeponBreak() || player.GetShiftAttackflag()) {
		//	Destroy();
		//}
		break;
	case Chile:
		
		break;
	}

	lifeTimer -= elapsedTime;
	if (lifeTimer < 0 && lifeflag)
	{
		//�������폜
		materialColor.w -= elapsedTime * 2;
		if (materialColor.w < 0) {
			Destroy();
		}
	}
}

void ParticleSprite::Destroy()
{
	ParticleSpriteManager::Instance().Remove(this);
}



