#include "Misc.h"
#include "Graphics.h"
#include "DefaultShader.h"
#include "ShadowMap.h"
#include "SkyboxShader.h"
#include "ToonShader.h"
#include "InstancingSpriteShader.h"

Graphics* Graphics::instance = nullptr;

Graphics::Graphics(HWND hWnd)
{
	// インスタンス設定
	_ASSERT_EXPR(instance == nullptr, "already instantiated");
	instance = this;

	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT screenWidth = rc.right - rc.left;
	UINT screenHeight = rc.bottom - rc.top;

	this->screenWidth = static_cast<float>(screenWidth);
	this->screenHeight = static_cast<float>(screenHeight);

	HRESULT hr{ S_OK };

	UINT create_device_flags{ 0 };
#if defined(DEBUG) || defined(_DEBUG)
	//create_device_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	//Device,Contextの能力レベルの設定
	//DirectX11の基準を完全に満たしたGPU上で動作するレベルに設定
	//D3D_FEATURE_LEVEL feature_levels{ D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	//IDXGISwapChainの設定を行う為の構造体。SwapChain作成するために必要な設定を行う
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	//スワップチェインのバッファ数。交換するバッファの枚数
	swapChainDesc.BufferCount = 1;
	//解像度幅
	swapChainDesc.BufferDesc.Width = screenWidth;
	//解像度高さ
	swapChainDesc.BufferDesc.Height = screenHeight;
	//描画先の形式
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//リフレッシュレート。Denominstorが分母
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	//バッファの使用方法の指定
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//ウィンドウハンドル
	swapChainDesc.OutputWindow = hWnd;
	//バックバッファのマルチサンプリングに関する設定
	//マルチサンプリングを使用しない場合はCountに１，Qualityに０を指定。
	//１ピクセルの色を決めるサンプリング数
	swapChainDesc.SampleDesc.Count = 1;
	//サンプリングの品質、制度
	swapChainDesc.SampleDesc.Quality = 0;
	//true…　ウィンドウモード
	swapChainDesc.Windowed = true;

	//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;

	swapChainDesc.Flags = 0;

	//swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//デバイス、デバイスコンテキスト、スワップチェ―ン

	D3D_FEATURE_LEVEL featureLevel;

	hr = D3D11CreateDeviceAndSwapChain(
		NULL,                             //ビデオアダプタを指定（NULLは既存）
		D3D_DRIVER_TYPE_HARDWARE,         //ドライバのタイプ
		NULL,                             //D3D_DRIVER_TYPE_SOFTWRE指定時に使用
		create_device_flags,              //フラグ指定
		featureLevels,                  //デバイスコンテキストの能力レベルを配列で指定
		ARRAYSIZE(featureLevels),                                //上記配列の要素数
		D3D11_SDK_VERSION,                //SDKバージョン
		&swapChainDesc,                 //スワップチェーンを詳細情報
		swapchain.GetAddressOf(),                      //関数成功時のスワップチェーンの出力先
		device.GetAddressOf(),                          //関数成功時のデバイスの出力先
		&featureLevel,                             //成功したD3D＿FEATURE＿LEVELの出力先
		immediateContext.GetAddressOf());              //関数成功時のContextの出力先
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer{};
	hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(back_buffer.GetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//バックバッファを指定してレンダーターゲットビューを作成
	hr = device->CreateRenderTargetView(back_buffer.Get(), NULL, renderTargetView.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));



	//ID3D11Texture２Dの設定を行うための構造体。ID3D11Texture２Dに深度ステンシル用の設定を行う
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer{};
	D3D11_TEXTURE2D_DESC texture2dDesc{};
	//テクスチャの幅（スクリーンの幅いっぱい）
	texture2dDesc.Width = screenWidth;
	//テクスチャの高さ（スクリーンの高さいっぱい）
	texture2dDesc.Height = screenHeight;
	texture2dDesc.MipLevels = 1;
	texture2dDesc.ArraySize = 1;
	texture2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texture2dDesc.SampleDesc.Count = 1;
	texture2dDesc.SampleDesc.Quality = 0;
	texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
	//深度ステンシルターゲットとしてバインド
	texture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//CPUからの読み書きを不可
	texture2dDesc.CPUAccessFlags = 0;
	texture2dDesc.MiscFlags = 0;
	hr = device->CreateTexture2D(&texture2dDesc, NULL, depthStencilBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//ID3D11DepthStencilView の設定を行うための構造体。深度ステンシルビューを作成するために必要な
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	//リソースデータの形式
	depthStencilViewDesc.Format = texture2dDesc.Format;
	//深度ステンシルへのアクセス方法
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	//リソースデータにアクセスするための深度ステンシルビューを作成
	hr = device->CreateDepthStencilView(depthStencilBuffer.Get(),
		&depthStencilViewDesc,
		depthStencilView.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//depthStencilBuffer->Release();

	D3D11_VIEWPORT viewport{};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(screenWidth);
	viewport.Height = static_cast<float>(screenHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	immediateContext->RSSetViewports(1, &viewport);

	//サンプラーステートを生成する為の構造体
	D3D11_SAMPLER_DESC samplerDesc;
	//テクスチャをサンプリングする時に使用するフィルタリング方法
	//縮小、拡大、およびミップマップレベルのサンプリングにはポイントサンプリングを使用します
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	//０から１の範囲外のauテクスチャ座標を解決するために使用するメゾット
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//０から１の範囲外のauテクスチャ座標を解決するために使用する方法
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//０から１の範囲外のauテクスチャ座標を解決するために使用する方法
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//計算されたミップマップレベルからのオフセット。
	samplerDesc.MipLODBias = 0;
	// クランプ値
	samplerDesc.MaxAnisotropy = 16;
	//サンプリングされたデータを既存のサンプリングされたデータと比較する関数
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	// AddressU,V,WにD3D11_TEXTURE_ADDRESS_BORDERが指定されている場合に使用する境界線の色
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	//アクセスをクランプするミップマップの範囲の下限
	samplerDesc.MinLOD = 0;
	//アクセスをクランプするミップマップの範囲の上限
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	//ポイントフィルタとしてサンプラーステートを生成
	hr = device->CreateSamplerState(&samplerDesc, samplerStates[0].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//リニアフィルタとしてサンプラーステートを生成
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hr = device->CreateSamplerState(&samplerDesc, samplerStates[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//アニソトロピックとしてサンプラーステートを生成
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	hr = device->CreateSamplerState(&samplerDesc, samplerStates[2].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// 深度テストを　ON、深度への書き込み　ON
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilStates[0].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//深度テストを　ON、深度への書き込み　OFF
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilStates[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//深度テストを　OFF、深度への書き込み　ON
	depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS; // 手前の物体を描画
	hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilStates[2].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//深度テストを　OFF、深度への書き込み　OFF
	depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // 書き込む
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS; // 手前の物体を描画
	depthStencilDesc.StencilEnable = FALSE; // ステンシル・テストなし
	depthStencilDesc.StencilReadMask = 0;     // ステンシル読み込みマスク。
	depthStencilDesc.StencilWriteMask = 0;     // ステンシル書き込みマスク。
	hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilStates[3].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//ブレントステートを生成するための構造体
	D3D11_BLEND_DESC blendDesc{};
	//無効
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&blendDesc, blendStates[0].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	//アルファ
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&blendDesc, blendStates[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//加算
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&blendDesc, blendStates[2].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//乗算
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_DEST_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = device->CreateBlendState(&blendDesc, blendStates[3].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// ラスタライザステートを作成するための構造体
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	// ソリッド描画
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	// 裏面非表示
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	// false...時計回りを表面とする、反時計回りを裏面
	rasterizerDesc.FrontCounterClockwise = TRUE;
	rasterizerDesc.DepthBias = 0;
	rasterizerDesc.DepthBiasClamp = 0;
	rasterizerDesc.SlopeScaledDepthBias = 0;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.ScissorEnable = FALSE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerStates[0].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// ワイヤーフレーム描画
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	// 裏面非表示
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.AntialiasedLineEnable = TRUE;
	hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerStates[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// ソリッド描画
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	// 両面表示
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerStates[2].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	{
		//定数バッファオブジェクトの生成
		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.ByteWidth = sizeof(scene_constants);
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		bufferDesc.StructureByteStride = 0;
		hr = device->CreateBuffer(&bufferDesc, nullptr, constantBuffers[0].GetAddressOf());
		_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	}
	// シェーダー
	{
		shader[static_cast<int>(ModelShaderId::ModelShaderDefault)] = std::make_unique<DefaultShader>(device.Get());
		shader[static_cast<int>(ModelShaderId::Toon)] = std::make_unique<ToonShader>(device.Get());
		//shader[static_cast<int>(ModelShaderId::Stealth)]=std::make_unique<
		shader[static_cast<int>(ModelShaderId::ShadowmapCaster)] = std::make_unique<ShadowMapShader>(device.Get());
	}
	// スプライトシェーダー
	{
		spriteShaders[static_cast<int>(SpriteShaderId::SpriteShaderDefault)] = std::make_unique<DefaultSpriteShader>(device.Get());
		spriteShaders[static_cast<int>(SpriteShaderId::Skybox)] = std::make_unique<SkyboxShader>(device.Get());
		spriteShaders[static_cast<int>(SpriteShaderId::LuminanceExtraction)] = std::make_unique<LuminanceExtractionShader>(device.Get());
		spriteShaders[static_cast<int>(SpriteShaderId::ParticleSprite)] = std::make_unique<ParticleSpriteShader>(device.Get());
		spriteShaders[static_cast<int>(SpriteShaderId::InstancingSprite)] = std::make_unique<InstancingSpriteShader>(device.Get());
	
	}

	// レンダラ
	{
		debugRenderer = std::make_unique<DebugRenderer>(device.Get());
		//lineRenderer = std::make_unique<LineRenderer>(device.Get(), 1024);
		imguiRenderer = std::make_unique<ImGuiRenderer>(hWnd, device.Get());
	}
	//エフェクト&&インスタンシング
	{
		swordtrailShader = std::make_unique<SwordTrailShader>(device.Get());
		particleShader = std::make_unique<ParticleShader>(device.Get());
		//instancingModelShader = std::make_unique<InstancingModelShader>(device.Get());
	}
	//ディゾルブテクスチャ生成
	{
		dissolveSprite[static_cast<int>(DissolveSpriteId::Dissolve)] = std::make_unique<Sprite>(L".\\resources\\UI\\dissolve.jpg");
		dissolveSprite[static_cast<int>(DissolveSpriteId::DissolveCircle)] = std::make_unique<Sprite>(L".\\resources\\UI\\dissolve Circle.png");
		dissolveSprite[static_cast<int>(DissolveSpriteId::DissolveDiagonal)] = std::make_unique<Sprite>(L".\\resources\\UI\\2.png");
	}
}

// デストラクタ
Graphics::~Graphics()
{
}