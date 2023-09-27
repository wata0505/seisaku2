#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>
#include <d3d11.h>


#include "misc.h"
#include "Graphics.h"
#include "HighResolutionTimer.h"
//#include "high_resolution_timer.h"
#include "CameraController.h"
#include "Input.h"
#include "Audio.h"
#include <wrl.h>


class Framework
{
public:
	
	

	CONST HWND hwnd;

	

	Framework(HWND hwnd);
	~Framework();

	Framework(const Framework&) = delete;
	Framework& operator=(const Framework&) = delete;
	Framework(Framework&&) noexcept = delete;
	Framework& operator=(Framework&&) noexcept = delete;

	int Run();
	

	LRESULT CALLBACK HandleMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
	bool initialize();
	void Update(float elapsed_time/*Elapsed seconds from last frame*/);
	void Render(float elapsed_time/*Elapsed seconds from last frame*/);
	bool uninitialize();

private:
	Input					input;
	HighResolutionTimer	    timer;
	uint32_t frames{ 0 };
	float elapsed_time{ 0.0f };
	Graphics	graphics;
	Audio		audio;
	void CalculateFrameStats();
	
};

