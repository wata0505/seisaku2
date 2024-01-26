#include "Base.h"
//#include "Camera.h"
#include "Graphics.h"
//#include "Collision.h"

static Base* instance = nullptr;

// インスタンス取得
Base& Base::Instance() 
{
	return *instance;
};

// コンストラクタ
Base::Base(DirectX::XMFLOAT3 position)
{
	instance = this;

	obj = std::make_unique<Model>(".\\resources\\SecurityGate\\SecurityGate.fbx", true, false);
	obj->ModelSerialize(".\\resources\\SecurityGate\\SecurityGate.fbx");
	obj->ModelRegister(".\\resources\\SecurityGate\\SecurityGate.fbx","Texture\\Albedo.png");
	
	this->position = position;					// 位置設定
	scale.x = scale.y = scale.z = 10.0f;		// サイズ設定
	UpdateTransform();							// 行列更新処理
	obj->UpdateBufferData(transform);			// 描画情報更新処理
	//transform = obj->GetBufferTransform();	// インスタンシング化の描画情報行列取得

	hpUISprite = std::make_unique<Sprite>(L".\\resources\\UI\\TowerHP.png");	// HPUIテクスチャ読み込み
	lossHPUISprite = std::make_unique<Sprite>(L".\\resources\\UI\\TowerLossHP.png");	// 減少HPUIテクスチャ読み込み

	// ホログラムシェーダー情報初期化
	minHeight = -4.0f;	// 最低高さ
	maxHeight = 4.0f;	// 最高高さ
}

// 更新処理
void Base::Update(float elapseTime)
{
	// 経過時間加算
	progressTimer += elapseTime;
	// シェーダー情報調整
	obj->ShaderAdjustment(50.0f, progressTimer, maxHeight);
#if 0
	isRender = Collision::IntersectFanVsSphere(
		Camera::Instance().GetEye(),
		Camera::Instance().GetFront(),
		Camera::Instance().GetFovY(),
		Camera::Instance().GetFarZ(),
		position,
		radius);

	// 行列更新処理
	UpdateTransform();
#endif
}
// 描画処理
void Base::Render(ID3D11DeviceContext* deviceContext, ModelShader* shader)
{
	// 表示用のためワールド行列に更新する
	//obj->UpdateTransform();
	//obj->UpdateBufferData(transform);

	shader->Draw(deviceContext, obj.get(), { 0.0f, minHeight, minHeight, maxHeight }, {});
	skyboxColor = 0.0f;
}

// 行列更新処理
void Base::UpdateTransform()
{
	const DirectX::XMFLOAT4X4 coordinate_system_transforms[]
	{
		{ -1,0,0,0, 0,1, 0,0, 0,0,1,0, 0,0,0,1 },		//0:RHS Y-UP　右手系
		{  1,0,0,0, 0,1, 0,0, 0,0,1,0, 0,0,0,1 },		//1:LHS Y-UP　左手系
		{ -1,0,0,0, 0,0,-1,0, 0,1,0,0, 0,0,0,1 },		//2:RHS Z-UP　右手系
		{  1,0,0,0, 0,0, 1,0, 0,1,0,0, 0,0,0,1 },		//3:LHS Z-UP　左手系
	};

	const float scaleFactor = 0.01f;

	DirectX::XMMATRIX C{ DirectX::XMLoadFloat4x4(&coordinate_system_transforms[0]) * DirectX::XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor) };
	// スケール行列作成
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	// 回転行列作成
	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;
	// 位置行列作成
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	// 3つの行列を組み合わせ、ワールド行列作成
	DirectX::XMMATRIX W = C * S * R * T;
	// 計算したワールド行列取得
	DirectX::XMStoreFloat4x4(&transform, W);
}

// UI表示処理
void Base::HpDisplay(RenderContext& renderContext, SpriteShader* shader)
{
#if 0
	if (!isRender) return;

	// 体力残量割合
	float gaugeThrate = this->hp / static_cast<float>(this->hpMax);
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	renderContext.deviceContext->RSGetViewports(&numViewports, &viewport);

	// 変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&renderContext.view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&renderContext.projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();
	
	// 体力が残っていればHPゲージ表示
	if (hp > 0)
	{
		DirectX::XMFLOAT3 sPosition = { position.x, position.y + 5.0f, position.z };
		DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&sPosition);
		// ワールド座標からスクリーン座標へ
		DirectX::XMVECTOR ScreenPosition = DirectX::XMVector3Project(
			WorldPosition,
			viewport.TopLeftX,
			viewport.TopLeftY,
			Graphics::Instance().GetScreenWidth(),
			Graphics::Instance().GetScreenHeight(),
			viewport.MinDepth,
			viewport.MaxDepth,
			Projection,
			View,
			World);
		// スクリーン座標
		DirectX::XMFLOAT2 screenPosition;
		DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);

		// 幅・高さ
		static constexpr float width = 200.0f;
		static constexpr float height = 20.0f;
		uiSprite->Render(renderContext.deviceContext,
			screenPosition.x - width * 0.5f,  screenPosition.y - height * 0.5f,
			width * gaugeThrate, height,
			0.0f, 0.0f,
			static_cast<float>(uiSprite->GetTextureWidth() * gaugeThrate), static_cast<float>(uiSprite->GetTextureHeight()),
			0.0f,
			3.0f, 1.0f, 1.0f, 1.0f);
		shader->Draw(renderContext, uiSprite.get());
	}
#else
	Graphics& graphics = Graphics::Instance();
	static constexpr float scale = 0.025f;
	static constexpr float size = 0.5f;
	// スクリーン幅・高さ
	float screenWidth = graphics.GetScreenWidth() * scale;
	float screenHeight = graphics.GetScreenHeight() * scale;
	float textureWidth = static_cast<float>(hpUISprite->GetTextureWidth());
	float textureHeight = static_cast<float>(hpUISprite->GetTextureHeight());
	// 体力残量割合
	float gaugeThrate = this->hp / static_cast<float>(this->hpMax);
	lossHPUISprite->Render(renderContext.deviceContext,
		screenHeight, screenHeight,
		textureWidth * size, textureHeight * size,
		0.0f, 0.0f,
		textureWidth, textureHeight,
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f);
	shader->Draw(renderContext, lossHPUISprite.get());
	hpUISprite->Render(renderContext.deviceContext,
		screenHeight, screenHeight + (textureHeight * (1.0f - gaugeThrate)) * 0.5f,
		textureWidth * size, textureHeight * size - (textureHeight * (1.0f - gaugeThrate)) * 0.5f,
		0.0f, (textureHeight * (1.0f - gaugeThrate)),
		textureWidth, (textureHeight * gaugeThrate),
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f);
	shader->Draw(renderContext, hpUISprite.get());
#endif
}

// ダメージ処理
void Base::InputDamage(int damage) 
{
	// 体力が残っていれば
	if (hp > 0) 
	{
		hp -= damage;			// 体力減少
		skyboxColor = 1.0f;		// スカイボックス色(強さ)設定
		// 体力が残っていなければ
		if (hp < 0) 
		{
			hp = 0;	// 体力を固定
		}
	}
}