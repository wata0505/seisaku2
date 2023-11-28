#include"ParticleSprite.h"
#include"ParticleSpriteManager.h"
#include"Graphics.h"
#include"Calculation.h"
#include"misc.h"
#include"Player.h"

// コンストラクタ
ParticleSprite::ParticleSprite(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir, int spritetype, int movetype, int textype, int Max, float lifetimer,float lenght,bool lifeflag,float speed,float size, DirectX::XMFLOAT4 materialColor)//基底クラスのコンストラクタ呼び出し
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
	
	// 頂点バッファのサブリソースの定義(バッファーリソースでも一つだけ持てる）
	D3D11_SUBRESOURCE_DATA vSubData;//初期化用データを作成
	vSubData.pSysMem = posVertex;  // バッファ・データの初期値
	vSubData.SysMemPitch = 0;
	vSubData.SysMemSlicePitch = 0;

	// 描画用頂点入力バッファの作成　　初期化用データを送っている
	HRESULT hr = device->CreateBuffer(&desc, &vSubData, verBufferDrawFrom.GetAddressOf());
	//device->CreateBuffer(&desc, 0, sceneBuffers.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	//動くタイプごとに初期値設定
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

	DescSRV.Buffer.ElementWidth = max; // データ数

										// シェーダ リソース ビューの作成
	hr = device->CreateShaderResourceView(gPBuffer[0].Get(), &DescSRV, gPSRV[0].GetAddressOf());

	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));


	hr = device->CreateShaderResourceView(gPBuffer[1].Get(), &DescSRV, gPSRV[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// アンオーダード・アクセス・ビュー
	D3D11_UNORDERED_ACCESS_VIEW_DESC descUAV;
	ZeroMemory(&descUAV, sizeof(descUAV));
	descUAV.Format = DXGI_FORMAT_UNKNOWN;
	descUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descUAV.Buffer.NumElements = Max; // データ数

											// アンオーダード・アクセス・ビューの作成
	hr = device->CreateUnorderedAccessView(gPBuffer[INPUT].Get(), &descUAV, gPUAV[INPUT].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	hr = device->CreateUnorderedAccessView(gPBuffer[OUTPUT].Get(), &descUAV, gPUAV[OUTPUT].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	ParticleSpriteManager::Instance().Register(this);
}

// デストラクタ
ParticleSprite::~ParticleSprite()
{

	
}
HRESULT ParticleSprite::CreateDiffusionResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir,float speed, float lenght,float size) {
	HRESULT hr;

	//初期化用データ
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
		if (dir.x == 0 && dir.y == 0 && dir.z == 0) {//もう一つの値が入ってなければ計算しない
			posVertex[i].Position = pos;
		}
		else {
			posVertex[i].Position.x = pos.x + Vec.x * (len * i);
			posVertex[i].Position.y = pos.y + Vec.y * (len * i);
			posVertex[i].Position.z = pos.z + Vec.z * (len * i);
		}
		//ランダムの方向に飛び散る
		float pitch = DirectX::XMConvertToRadians(rand() % 360);
		float yaw = DirectX::XMConvertToRadians(rand() % 180);
		DirectX::XMFLOAT3 dir;
		dir.x = cosf(pitch) * sinf(yaw);
		dir.y = sin(pitch);
		dir.z = cosf(pitch) * cosf(yaw);
		posVertex[i].Position = Vector3::PosDir(posVertex[i].Position, dir, lenght);
		posVertex[i].Velocity = dir;//速度
		if (i % 2) {
			posVertex[i].Speed = (40) * speed;//speed;//速度
		}
		else
		{
			posVertex[i].Speed = (rand() % 50 + 20) * speed;//
		}
		//サイズ
		posVertex[i].ParticleSize.x = posVertex[i].ParticleSize.y = posVertex[i].ParticleSize.z = posVertex[i].ParticleSize.w = size;
		posVertex[i].type = Diffusion;
		posVertex[i].Timer = rand() % 50 + i;
	}
	uvStatus.x = 1.5;//ディゾルブのかけ具合０～３
	// リソースの設定
	hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;
}

HRESULT ParticleSprite::CreateSlashingResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir) {
	HRESULT hr;

	//初期化用データ
	ID3D11Device* device = Graphics::Instance().GetDevice();
	struct PVConstants* posVertex = new PVConstants[max];
	for (int i = 0; i < max; ++i) {
		//一方向から拡散するように設定
		posVertex[i].Position = pos;
		float angleX = DirectX::XMConvertToRadians(rand() % 5 - 2.5);
		float angleY = DirectX::XMConvertToRadians(rand() % 5 - 2.5);
		DirectX::XMFLOAT3 Vec;
		Vec.y = sin(NULL) - sin(angleY);
		Vec.x = cosf(NULL) * sinf(NULL) - cosf(angleY) * sinf(angleX);
		Vec.z = cosf(NULL) * cosf(NULL) - cosf(angleY) * cosf(angleX);
		Vec = Vector3::Add(dir, Vec);
		float speed = (rand() % 100 + 1)*0.001;
		posVertex[i].Velocity = Vec;//速度
		posVertex[i].Speed = speed;//速度
		posVertex[i].ParticleSize.x = posVertex[i].ParticleSize.y = posVertex[i].ParticleSize.z = posVertex[i].ParticleSize.w = 0.05;
		posVertex[i].type = Slashing;

	}
	// リソースの設定
	hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;
}
HRESULT ParticleSprite::CreateWeponConvergenceResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 endPos) {
	HRESULT hr;
	//初期化用データ
	ID3D11Device* device = Graphics::Instance().GetDevice();
	struct PVConstants* posVertex = new PVConstants[max];
	for (int i = 0; i < max; ++i) {
		 //ランダムでポジションを決定
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
		posVertex[i].Velocity = dir;//速度
		posVertex[i].Speed = speed;//速度
		posVertex[i].ParticleSize.x = posVertex[i].ParticleSize.y = posVertex[i].ParticleSize.z = posVertex[i].ParticleSize.w = 0.02;
		posVertex[i].type = WeponConvergence;
		posVertex[i].Timer = 1;

	}
	uvStatus.x = 1.5;//ディゾルブのかけ具合０～３
	// リソースの設定
	hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;
}

HRESULT ParticleSprite::CreateConvergenceResource(DirectX::XMFLOAT3 pos) {
	HRESULT hr;
	//初期化用データ
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
		posVertex[i].Velocity = pos;//速度
		posVertex[i].Speed = speed;//速度
		posVertex[i].ParticleSize.x = posVertex[i].ParticleSize.y = posVertex[i].ParticleSize.z = posVertex[i].ParticleSize.w = 0.02;
		posVertex[i].type = Convergence;
		posVertex[i].Timer = 1;
	}
	// リソースの設定
	hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;
}
HRESULT ParticleSprite::CreateSlashResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir) {
	//materialColor = { 1,1,1,1 };
	
	//初期化用データ
	ID3D11Device* device = Graphics::Instance().GetDevice();
	struct PVConstants* posVertex = new PVConstants[max];
	for (int i = 0; i < max; ++i) {
		posVertex[i].Position = pos;
		float speed = 0.15;
		posVertex[i].Velocity = dir;//速度
		posVertex[i].Speed = speed;//速度
		posVertex[i].ParticleSize.x = 0.8;
		posVertex[i].ParticleSize.w  = 5.5;
		posVertex[i].ParticleSize.y = -5.5;
		posVertex[i].type = Slash;
	}
	uvStatus.x = 1.5;//ディゾルブのかけ具合０～３
	depth = DepthStencilState::OFFOFF;//深度テスト
	// リソースの設定
	HRESULT hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;
}
HRESULT ParticleSprite::CreateExpansionResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 dir,float leght) {
	materialColor = { 1,1,1,0.4};
	//初期化用データ
	ID3D11Device* device = Graphics::Instance().GetDevice();
	struct PVConstants* posVertex = new PVConstants[max];
	for (int i = 0; i < max; ++i) {
		posVertex[i].Position = pos;
		float speed = 0.1 * i + leght;
		posVertex[i].Velocity = dir;//速度
		posVertex[i].Speed = speed;//速度
		posVertex[i].ParticleSize.x = 0;
		posVertex[i].ParticleSize.w = 0;
		posVertex[i].ParticleSize.y = 0;
		posVertex[i].type = Expansion;
		posVertex[i].Timer = 1;
	}
	// リソースの設定
	HRESULT hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;
}
HRESULT ParticleSprite::CreateCorruptionResource(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 endPos) {
	HRESULT hr;
	//初期化用データ
	ID3D11Device* device = Graphics::Instance().GetDevice();
	struct PVConstants* posVertex = new PVConstants[max];
	//どこからどこまでの中、ランダムで位置を決定
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
		//上方向にランダムで飛ぶ
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
		posVertex[i].Velocity = dir;//速度
		posVertex[i].Speed = speed;//速度
		posVertex[i].ParticleSize.x = posVertex[i].ParticleSize.y = posVertex[i].ParticleSize.w = 0.01;
		posVertex[i].ParticleSize.z = len;//位置決定
		posVertex[i].type = Corruption;
		posVertex[i].Timer = 0;
	}
	// リソースの設定
	hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;

}

HRESULT ParticleSprite::CreateChileResource(DirectX::XMFLOAT3 pos) {
	HRESULT hr;
	//初期化用データ
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
		posVertex[i].Velocity = Vec;//速度
		posVertex[i].Speed = -0.5f;//速度
		posVertex[i].ParticleSize.x = posVertex[i].ParticleSize.y = posVertex[i].ParticleSize.w = 0.02;
		posVertex[i].ParticleSize.z = 0;
		posVertex[i].type = Chile;
		posVertex[i].Timer = 1;
	}

	// リソースの設定
	hr = CreateResource(posVertex);
	delete[] posVertex;
	return S_OK;

}
HRESULT ParticleSprite::CreateResource(PVConstants* posVertex) {
	
	ID3D11Device* device = Graphics::Instance().GetDevice();
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = max * sizeof(PVConstants); // バッファ サイズ
	desc.Usage = D3D11_USAGE_DEFAULT;//ステージの入出力はOK。GPUの入出力OK。
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // 構造化バッファ
	desc.StructureByteStride = sizeof(PVConstants);

	D3D11_SUBRESOURCE_DATA SubResource;//サブリソースの初期化用データを定義
	SubResource.pSysMem = posVertex;
	SubResource.SysMemPitch = 0;
	SubResource.SysMemSlicePitch = 0;

	// 最初の入力リソース(データを初期化する)
	HRESULT hr = device->CreateBuffer(&desc, &SubResource, gPBuffer[0].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// 最初の出力リソース（初期化用データは必要ない）
	device->CreateBuffer(&desc, &SubResource, gPBuffer[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	return S_OK;

}
//更新処理
void ParticleSprite::Update(float elapsedTime)
{
	Player& player = Player::Instance();
	//寿命処理
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
		//自分を削除
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



