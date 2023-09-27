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
	// �C���X�^���X�ݒ�
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
	//Device,Context�̔\�̓��x���̐ݒ�
	//DirectX11�̊�����S�ɖ�������GPU��œ��삷�郌�x���ɐݒ�
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

	//IDXGISwapChain�̐ݒ���s���ׂ̍\���́BSwapChain�쐬���邽�߂ɕK�v�Ȑݒ���s��
	DXGI_SWAP_CHAIN_DESC swapChainDesc{};
	//�X���b�v�`�F�C���̃o�b�t�@���B��������o�b�t�@�̖���
	swapChainDesc.BufferCount = 1;
	//�𑜓x��
	swapChainDesc.BufferDesc.Width = screenWidth;
	//�𑜓x����
	swapChainDesc.BufferDesc.Height = screenHeight;
	//�`���̌`��
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	//���t���b�V�����[�g�BDenominstor������
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	//�o�b�t�@�̎g�p���@�̎w��
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	//�E�B���h�E�n���h��
	swapChainDesc.OutputWindow = hWnd;
	//�o�b�N�o�b�t�@�̃}���`�T���v�����O�Ɋւ���ݒ�
	//�}���`�T���v�����O���g�p���Ȃ��ꍇ��Count�ɂP�CQuality�ɂO���w��B
	//�P�s�N�Z���̐F�����߂�T���v�����O��
	swapChainDesc.SampleDesc.Count = 1;
	//�T���v�����O�̕i���A���x
	swapChainDesc.SampleDesc.Quality = 0;
	//true�c�@�E�B���h�E���[�h
	swapChainDesc.Windowed = true;

	//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;

	swapChainDesc.Flags = 0;

	//swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	//swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	//�f�o�C�X�A�f�o�C�X�R���e�L�X�g�A�X���b�v�`�F�\��

	D3D_FEATURE_LEVEL featureLevel;

	hr = D3D11CreateDeviceAndSwapChain(
		NULL,                             //�r�f�I�A�_�v�^���w��iNULL�͊����j
		D3D_DRIVER_TYPE_HARDWARE,         //�h���C�o�̃^�C�v
		NULL,                             //D3D_DRIVER_TYPE_SOFTWRE�w�莞�Ɏg�p
		create_device_flags,              //�t���O�w��
		featureLevels,                  //�f�o�C�X�R���e�L�X�g�̔\�̓��x����z��Ŏw��
		ARRAYSIZE(featureLevels),                                //��L�z��̗v�f��
		D3D11_SDK_VERSION,                //SDK�o�[�W����
		&swapChainDesc,                 //�X���b�v�`�F�[�����ڍ׏��
		swapchain.GetAddressOf(),                      //�֐��������̃X���b�v�`�F�[���̏o�͐�
		device.GetAddressOf(),                          //�֐��������̃f�o�C�X�̏o�͐�
		&featureLevel,                             //��������D3D�QFEATURE�QLEVEL�̏o�͐�
		immediateContext.GetAddressOf());              //�֐���������Context�̏o�͐�
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	Microsoft::WRL::ComPtr<ID3D11Texture2D> back_buffer{};
	hr = swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(back_buffer.GetAddressOf()));
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//�o�b�N�o�b�t�@���w�肵�ă����_�[�^�[�Q�b�g�r���[���쐬
	hr = device->CreateRenderTargetView(back_buffer.Get(), NULL, renderTargetView.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));



	//ID3D11Texture�QD�̐ݒ���s�����߂̍\���́BID3D11Texture�QD�ɐ[�x�X�e���V���p�̐ݒ���s��
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer{};
	D3D11_TEXTURE2D_DESC texture2dDesc{};
	//�e�N�X�`���̕��i�X�N���[���̕������ς��j
	texture2dDesc.Width = screenWidth;
	//�e�N�X�`���̍����i�X�N���[���̍��������ς��j
	texture2dDesc.Height = screenHeight;
	texture2dDesc.MipLevels = 1;
	texture2dDesc.ArraySize = 1;
	texture2dDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	texture2dDesc.SampleDesc.Count = 1;
	texture2dDesc.SampleDesc.Quality = 0;
	texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
	//�[�x�X�e���V���^�[�Q�b�g�Ƃ��ăo�C���h
	texture2dDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//CPU����̓ǂݏ�����s��
	texture2dDesc.CPUAccessFlags = 0;
	texture2dDesc.MiscFlags = 0;
	hr = device->CreateTexture2D(&texture2dDesc, NULL, depthStencilBuffer.GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//ID3D11DepthStencilView �̐ݒ���s�����߂̍\���́B�[�x�X�e���V���r���[���쐬���邽�߂ɕK�v��
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
	//���\�[�X�f�[�^�̌`��
	depthStencilViewDesc.Format = texture2dDesc.Format;
	//�[�x�X�e���V���ւ̃A�N�Z�X���@
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;
	//���\�[�X�f�[�^�ɃA�N�Z�X���邽�߂̐[�x�X�e���V���r���[���쐬
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

	//�T���v���[�X�e�[�g�𐶐�����ׂ̍\����
	D3D11_SAMPLER_DESC samplerDesc;
	//�e�N�X�`�����T���v�����O���鎞�Ɏg�p����t�B���^�����O���@
	//�k���A�g��A����у~�b�v�}�b�v���x���̃T���v�����O�ɂ̓|�C���g�T���v�����O���g�p���܂�
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	//�O����P�͈̔͊O��au�e�N�X�`�����W���������邽�߂Ɏg�p���郁�]�b�g
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	//�O����P�͈̔͊O��au�e�N�X�`�����W���������邽�߂Ɏg�p������@
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	//�O����P�͈̔͊O��au�e�N�X�`�����W���������邽�߂Ɏg�p������@
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	//�v�Z���ꂽ�~�b�v�}�b�v���x������̃I�t�Z�b�g�B
	samplerDesc.MipLODBias = 0;
	// �N�����v�l
	samplerDesc.MaxAnisotropy = 16;
	//�T���v�����O���ꂽ�f�[�^�������̃T���v�����O���ꂽ�f�[�^�Ɣ�r����֐�
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	// AddressU,V,W��D3D11_TEXTURE_ADDRESS_BORDER���w�肳��Ă���ꍇ�Ɏg�p���鋫�E���̐F
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	//�A�N�Z�X���N�����v����~�b�v�}�b�v�͈̔͂̉���
	samplerDesc.MinLOD = 0;
	//�A�N�Z�X���N�����v����~�b�v�}�b�v�͈̔͂̏��
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	//�|�C���g�t�B���^�Ƃ��ăT���v���[�X�e�[�g�𐶐�
	hr = device->CreateSamplerState(&samplerDesc, samplerStates[0].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//���j�A�t�B���^�Ƃ��ăT���v���[�X�e�[�g�𐶐�
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	hr = device->CreateSamplerState(&samplerDesc, samplerStates[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//�A�j�\�g���s�b�N�Ƃ��ăT���v���[�X�e�[�g�𐶐�
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	hr = device->CreateSamplerState(&samplerDesc, samplerStates[2].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// �[�x�e�X�g���@ON�A�[�x�ւ̏������݁@ON
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilStates[0].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//�[�x�e�X�g���@ON�A�[�x�ւ̏������݁@OFF
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilStates[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//�[�x�e�X�g���@OFF�A�[�x�ւ̏������݁@ON
	depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS; // ��O�̕��̂�`��
	hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilStates[2].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//�[�x�e�X�g���@OFF�A�[�x�ւ̏������݁@OFF
	depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // ��������
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS; // ��O�̕��̂�`��
	depthStencilDesc.StencilEnable = FALSE; // �X�e���V���E�e�X�g�Ȃ�
	depthStencilDesc.StencilReadMask = 0;     // �X�e���V���ǂݍ��݃}�X�N�B
	depthStencilDesc.StencilWriteMask = 0;     // �X�e���V���������݃}�X�N�B
	hr = device->CreateDepthStencilState(&depthStencilDesc, depthStencilStates[3].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	//�u�����g�X�e�[�g�𐶐����邽�߂̍\����
	D3D11_BLEND_DESC blendDesc{};
	//����
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
	//�A���t�@
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

	//���Z
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

	//��Z
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

	// ���X�^���C�U�X�e�[�g���쐬���邽�߂̍\����
	D3D11_RASTERIZER_DESC rasterizerDesc{};
	// �\���b�h�`��
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	// ���ʔ�\��
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	// false...���v����\�ʂƂ���A�����v���𗠖�
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

	// ���C���[�t���[���`��
	rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
	// ���ʔ�\��
	rasterizerDesc.CullMode = D3D11_CULL_BACK;
	rasterizerDesc.AntialiasedLineEnable = TRUE;
	hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerStates[1].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));

	// �\���b�h�`��
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	// ���ʕ\��
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	hr = device->CreateRasterizerState(&rasterizerDesc, rasterizerStates[2].GetAddressOf());
	_ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
	{
		//�萔�o�b�t�@�I�u�W�F�N�g�̐���
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
	// �V�F�[�_�[
	{
		shader[static_cast<int>(ModelShaderId::ModelShaderDefault)] = std::make_unique<DefaultShader>(device.Get());
		shader[static_cast<int>(ModelShaderId::Toon)] = std::make_unique<ToonShader>(device.Get());
		//shader[static_cast<int>(ModelShaderId::Stealth)]=std::make_unique<
		shader[static_cast<int>(ModelShaderId::ShadowmapCaster)] = std::make_unique<ShadowMapShader>(device.Get());
	}
	// �X�v���C�g�V�F�[�_�[
	{
		spriteShaders[static_cast<int>(SpriteShaderId::SpriteShaderDefault)] = std::make_unique<DefaultSpriteShader>(device.Get());
		spriteShaders[static_cast<int>(SpriteShaderId::Skybox)] = std::make_unique<SkyboxShader>(device.Get());
		spriteShaders[static_cast<int>(SpriteShaderId::LuminanceExtraction)] = std::make_unique<LuminanceExtractionShader>(device.Get());
		spriteShaders[static_cast<int>(SpriteShaderId::ParticleSprite)] = std::make_unique<ParticleSpriteShader>(device.Get());
		spriteShaders[static_cast<int>(SpriteShaderId::InstancingSprite)] = std::make_unique<InstancingSpriteShader>(device.Get());
	
	}

	// �����_��
	{
		debugRenderer = std::make_unique<DebugRenderer>(device.Get());
		//lineRenderer = std::make_unique<LineRenderer>(device.Get(), 1024);
		imguiRenderer = std::make_unique<ImGuiRenderer>(hWnd, device.Get());
	}
	//�G�t�F�N�g&&�C���X�^���V���O
	{
		swordtrailShader = std::make_unique<SwordTrailShader>(device.Get());
		particleShader = std::make_unique<ParticleShader>(device.Get());
		//instancingModelShader = std::make_unique<InstancingModelShader>(device.Get());
	}
	//�f�B�]���u�e�N�X�`������
	{
		dissolveSprite[static_cast<int>(DissolveSpriteId::Dissolve)] = std::make_unique<Sprite>(L".\\resources\\UI\\dissolve.jpg");
		dissolveSprite[static_cast<int>(DissolveSpriteId::DissolveCircle)] = std::make_unique<Sprite>(L".\\resources\\UI\\dissolve Circle.png");
		dissolveSprite[static_cast<int>(DissolveSpriteId::DissolveDiagonal)] = std::make_unique<Sprite>(L".\\resources\\UI\\2.png");
	}
}

// �f�X�g���N�^
Graphics::~Graphics()
{
}