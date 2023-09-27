#include "Framework.h"
#include "shader.h"
#include "texture.h"
#include "SceneManager.h"
#include "Camera.h"
#include "StageMain.h"
#include "StageManager.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "EffectManager.h"

CONST BOOL FULLSCREEN{ TRUE };
CONST LPCWSTR APPLICATION_NAME{ L"X3DGP" };

// ���������Ԋu�ݒ�
static const int syncInterval = 0;
//UINT sync_interval{ 1 };
Framework::Framework(HWND hwnd) : hwnd(hwnd), graphics(hwnd), input(hwnd)
{
	//�G�t�F�N�g�}�l�[�W���[������
	EffectManager::instance().Initialize();
	SceneManager::Instance().ChangeScene(new SceneTitle);
}

bool Framework::initialize()
{
	

	return true;
}

void Framework::Update(float elapsed_time/*Elapsed seconds from last frame*/)
{
	
	input.Update();

	SceneManager::Instance().Update(elapsed_time);

}

void Framework::Render(float elapsed_time/*Elapsed seconds from last frame*/)
{
	std::lock_guard<std::mutex> lock(graphics.GetMutex());
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();

	// IMGUI�t���[���J�n����
	graphics.GetImGuiRenderer()->NewFrame();

	// �V�[���`�揈��
	//sceneGame.Render();
	 SceneManager::Instance().Render();

	// IMGUI�f���E�C���h�E�`��iIMGUI�@�\�e�X�g�p�j
	//ImGui::ShowDemoWindow();

	// IMGUI�`��
	graphics.GetImGuiRenderer()->Render(dc);

	// �o�b�N�o�b�t�@�ɕ`�悵�������ʂɕ\������B
	graphics.GetSwapChain()->Present(syncInterval, 0);


	//UINT sync_interval{ 0 };
}

bool Framework::uninitialize()
{
	//device->Release();
	//immediate_context->Release();
	//swap_chain->Release();
	//render_target_view->Release();
	//depth_stencil_view->Release();
	//for (int i = 0; i < 3; i++) {
	//	sampler_states[i]->Release();
	//}
	//for (int i = 0; i < 4; i++) {
	//	blend_states[i]->Release();
	//}
	//for (sprite* p : sprites)delete p;

	//for (ID3D11DepthStencilState* p : depth_stencil_states) p->Release();

	SceneManager::Instance().Clear();
	return true;
}

Framework::~Framework()
{
	
	SceneManager::Instance().Clear();
	//�G�t�F�N�g�}�l�[�W���[�I����
	EffectManager::instance().Finalize();
	
}
// �t���[�����[�g�v�Z
void Framework::CalculateFrameStats()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.
	static int frames = 0;
	static float time_tlapsed = 0.0f;

	frames++;

	// Compute averages over one second period.
	if ((timer.TimeStamp() - time_tlapsed) >= 1.0f)
	{
		float fps = static_cast<float>(frames); // fps = frameCnt / 1
		float mspf = 1000.0f / fps;
		std::ostringstream outs;
		outs.precision(6);
		outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
		SetWindowTextA(hwnd, outs.str().c_str());

		// Reset for next average.
		frames = 0;
		time_tlapsed += 1.0f;
	}
}

// �A�v���P�[�V�������[�v
int Framework::Run()
{
	MSG msg = {};

	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			timer.Tick();
			CalculateFrameStats();

			float elapsedTime = syncInterval == 0
				? timer.TimeInterval()
				: syncInterval / 60.0f
				;
			Update(elapsedTime);
			Render(elapsedTime);
		}
	}
	return static_cast<int>(msg.wParam);
}

// ���b�Z�[�W�n���h��
LRESULT CALLBACK Framework::HandleMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (Graphics::Instance().GetImGuiRenderer()->HandleMessage(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps{};
		//HDC hdc;
		BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostMessage(hWnd, WM_CLOSE, 0, 0);
		break;
	case WM_ENTERSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
		timer.Stop();
		break;
	case WM_EXITSIZEMOVE:
		// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
		// Here we reset everything based on the new window dimensions.
		timer.Start();
		break;
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}
