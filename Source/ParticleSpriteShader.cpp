#include "ParticleSpriteShader.h"
#include "misc.h"
#include "Graphics.h"
#include "Camera.h"
#include "Player.h"

using namespace DirectX;

ParticleSpriteShader::ParticleSpriteShader(ID3D11Device* device)
{
	
	// 頂点シェーダー
	{
		// 入力レイアウト
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[]
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "PARTICLESIZE", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "SPEED",    0, DXGI_FORMAT_R32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "PARTICLETYPE",    0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TIMER",    0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		create_vs_from_cso(device, "Shader\\ParticleSpriteVS.cso", vertexShader.ReleaseAndGetAddressOf(),
			inputLayout.ReleaseAndGetAddressOf(), inputElementDesc, ARRAYSIZE(inputElementDesc));
	}

	// ピクセルシェーダー
	{
		create_ps_from_cso(device, "Shader\\ParticleSpritePS.cso", pixelShader.GetAddressOf());
	}
	// ジオメトリシェーダー
	{
		create_gs_from_cso(device, "Shader\\ParticleSpriteGS.cso", geometryShader.GetAddressOf());
	}
	// 定数バッファ
	{
		//// シーン用バッファ
		
		D3D11_BUFFER_DESC cBufferDesc;
		cBufferDesc.Usage = D3D11_USAGE_DEFAULT;    // 動的(ダイナミック)使用法
		cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // 定数バッファ
		cBufferDesc.CPUAccessFlags = 0;     // CPUから書き込む
		cBufferDesc.MiscFlags = 0;
		cBufferDesc.StructureByteStride = 0;
		cBufferDesc.ByteWidth = sizeof(SceneConstants);

		HRESULT hr = device->CreateBuffer(&cBufferDesc, 0, sceneBuffers.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		cBufferDesc.ByteWidth = sizeof(ColorConstants);

		device->CreateBuffer(&cBufferDesc, 0, colorBuffers.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

		cBufferDesc.ByteWidth = sizeof(CalculationConstants);

		device->CreateBuffer(&cBufferDesc, 0, calculationBuffers.GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
	{//コンピュートシェーダー
		create_cs_from_cso(device, "Shader\\ParticleSpriteCS.cso", computeShader.GetAddressOf());
	}
	

	//パーティクルの形
	moveSprite[ParticleSprite::ParticleSoft] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Particle_Soft.png");
	moveSprite[ParticleSprite::ParticleLine] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Line9.png");
	moveSprite[ParticleSprite::ParticleImpact] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Shock_wave1.png");
	moveSprite[ParticleSprite::ParticleWind] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\wind04.png");
	moveSprite[ParticleSprite::ParticleSquare] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\hisigata1.png");
	moveSprite[ParticleSprite::ParticleBurst] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\Burst_2.png");
	moveSprite[ParticleSprite::ParticleTriangle] = std::make_unique<Sprite>(L".\\resources\\Effect\\Texture\\sannkaku3.png");

	

	
}

// 描画開始
void ParticleSpriteShader::Begin(const RenderContext& rc)
{
	rc.deviceContext->VSSetShader(vertexShader.Get(), nullptr, 0);
	rc.deviceContext->GSSetShader(geometryShader.Get(), nullptr, 0);
	rc.deviceContext->PSSetShader(pixelShader.Get(), nullptr, 0);
	rc.deviceContext->CSSetShader(computeShader.Get(), nullptr, 0);
	Graphics& graphics = Graphics::Instance();
	DirectX::XMMATRIX V = DirectX::XMLoadFloat4x4(&rc.view);

	SceneConstants scene{};
	DirectX::XMMATRIX P =DirectX::XMLoadFloat4x4(&Camera::Instance().GetProjection());
	P = XMMatrixTranspose(P);//転地行列へ
	scene.view = Camera::Instance().GetView();
	DirectX::XMStoreFloat4x4(&scene.projection, P);

	// ビュー・プロジェクション行列と光の当たる方向をシーン定数バッファとして設定
	rc.deviceContext->UpdateSubresource(sceneBuffers.Get(), 0, 0, &scene, 0, 0);
	rc.deviceContext->VSSetConstantBuffers(0, 1, sceneBuffers.GetAddressOf());
	rc.deviceContext->PSSetConstantBuffers(0, 1, sceneBuffers.GetAddressOf());
	rc.deviceContext->GSSetConstantBuffers(0, 1, sceneBuffers.GetAddressOf());
	rc.deviceContext->CSSetConstantBuffers(0, 1, sceneBuffers.GetAddressOf());
	

	const float blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	rc.deviceContext->OMSetBlendState(graphics.GetBlendState(BlendState::Alpha), blend_factor, 0xFFFFFFFF);
	rc.deviceContext->OMSetDepthStencilState(graphics.GetDepthStencilState(DepthStencilState::ONOFF), 0);
	rc.deviceContext->RSSetState(graphics.GetRasterizerState(RasterizerState::Solid));

	

}
HRESULT ParticleSpriteShader::CreateResource() {
	

	return S_OK;
}


HRESULT ParticleSpriteShader::ComputeShader(const RenderContext& rc, ParticleSprite* particleSprite)
{
	
	// **********************************************************
	// コンピュート・シェーダを使った演算
	// **********************************************************
	HRESULT hr = S_OK;
	// シェーダを設定
	Graphics::Instance();
	
	ID3D11ShaderResourceView* pViewNULL = NULL;

	CalculationConstants calculation{};
	//計算用バファ入力
	calculation.tail = Player::Instance().GetTailPos();
	calculation.pattern = Player::Instance().GetPatternPos();
	calculation.max = particleSprite->max;
	calculation.uvStatus.x = particleSprite->uvStatus.x;
	rc.deviceContext->UpdateSubresource(calculationBuffers.Get(), 0, 0, &calculation, 0, 0);
	rc.deviceContext->CSSetConstantBuffers(1, 1, calculationBuffers.GetAddressOf());
	rc.deviceContext->GSSetConstantBuffers(1, 1, calculationBuffers.GetAddressOf());
	rc.deviceContext->PSSetConstantBuffers(1, 1, calculationBuffers.GetAddressOf());
	// 現在のシェーダ リソース ビューを解除
	rc.deviceContext->CSSetShaderResources(0, 1, &pViewNULL);
	// アンオーダード・アクセス・ビューの設定
	rc.deviceContext->CSSetUnorderedAccessViews(0, 1, particleSprite->gPUAV[particleSprite->chainB].GetAddressOf(), NULL);

	// シェーダ リソース ビューの設定
	rc.deviceContext->CSSetShaderResources(0, 1, particleSprite->gPSRV[particleSprite->chainA].GetAddressOf());

	// コンピュート・シェーダの実行

	rc.deviceContext->Dispatch(100, 1, 1);//グループの数
	rc.deviceContext->CopyResource(particleSprite->verBufferDrawFrom.Get(), particleSprite->gPBuffer[0].Get());


	
	return hr;
}
void ParticleSpriteShader::Update(const RenderContext& rc ,ParticleSprite* particleSprite) {
	ComputeShader(rc,particleSprite);
	particleSprite->chainA = particleSprite->chainA ? 0 : 1;//バッファーの切り替え
	particleSprite->chainB = particleSprite->chainB ? 0 : 1;//バッファーの切り替え
}
// 描画
void ParticleSpriteShader::Draw(const RenderContext& rc, const Sprite* sprite)
{
	Graphics& graphics = Graphics::Instance();
	int max = ParticleSpriteManager::Instance().GetParticleCount();
	for (int i = 0; i < max;i++) {
		ParticleSprite* particle = ParticleSpriteManager::Instance().GetParticleSprite(i);
		
		rc.deviceContext->OMSetDepthStencilState(graphics.GetDepthStencilState(particle->depth), 0);
		
		Update(rc, particle);

		UINT strides[1] = { sizeof(PVConstants) };
		UINT offsets[1] = { 0 };
		rc.deviceContext->IASetVertexBuffers(0, 1, particle->verBufferDrawFrom.GetAddressOf(), strides, offsets);
		// IAに入力レイアウト・オブジェクトを設定
		rc.deviceContext->IASetInputLayout(inputLayout.Get());
		// IAにプリミティブの種類を設定
		rc.deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

		ColorConstants color{};
		color.materialColor = particle->materialColor;
		rc.deviceContext->UpdateSubresource(colorBuffers.Get(), 0, 0, &color, 0, 0);
		rc.deviceContext->PSSetConstantBuffers(2, 1, colorBuffers.GetAddressOf());
		//パーティクル形
		rc.deviceContext->PSSetShaderResources(0, 1, moveSprite[particle->spriteType]->GetShaderResourceView().GetAddressOf());
		//テクスチャ
		rc.deviceContext->PSSetShaderResources(1, 1, EffectTexAll::Instance().GetSprite(particle->texType).get()->GetShaderResourceView().GetAddressOf());
		//ディゾルブ
		rc.deviceContext->PSSetShaderResources(5, 1, graphics.GetDissolveSprite(Graphics::DissolveSpriteId::Dissolve)->GetShaderResourceView().GetAddressOf());

		rc.deviceContext->Draw(particle->max, 0);
	}
}


// 描画終了
void ParticleSpriteShader::End(const RenderContext& rc)
{
	rc.deviceContext->VSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->PSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->GSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->CSSetShader(nullptr, nullptr, 0);
	rc.deviceContext->IASetInputLayout(nullptr);
	rc.deviceContext->RSSetState(Graphics::Instance().GetRasterizerState(RasterizerState::Solid));
}