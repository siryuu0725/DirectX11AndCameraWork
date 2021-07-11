#include <Windows.h>
#include <time.h>
#include <mmsystem.h>
#include "System/Window.h"
#include "System/DirectGraphics.h"
#include "System/DirectInput.h"
#include "System/Fbx/FbxMeshFile.h"
#include "System/Texture/TextureManager.h"
#include "Object/Camera.h"
#include "Object/Player.h"
#include "Object/Floor.h"
#include "Object/SkyDome.h"
#include "Object/Block/BlockController.h"
#include "Scene/SceneController.h"

#pragma comment(lib, "winmm.lib")

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmpLine,
	INT       nCmdShow)
{
#ifdef _DEBUG

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

#endif

	// ウィンドウを作成
	if (Window::MakeWindow("FbxRender", 1920, 1080) == false)
	{
		return 0;
	}

	//描画情報初期化
	if (DirectGraphics::Instance()->Init() == false)
	{
		return 0;
	}
	//入力情報初期化
	if (Inputter::Instance()->InitInput() == false)
	{
		return 0;
	}

	timeBeginPeriod(1);

	DWORD current = timeGetTime();
	DWORD before = current;

	while (true)
	{
		bool message_ret = false;
		MSG msg;

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else if (msg.message == WM_CLOSE)
			{
				break;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//更新処理
			SceneController::Instance()->Update();

			//描画
			SceneController::Instance()->Draw();

			while (current - before < 13)
			{
				Sleep(current - before);

				current = timeGetTime();
			}

			before = current;
		}
	}

	timeEndPeriod(1);

	//DirectX11の解放関数
	DirectGraphics::Instance()->Release();
	//入力情報解放
	Inputter::Instance()->EndInput();
	//テクスチャ解放関数
	TextureManager::Instance()->Release();

#ifdef _DEBUG
	TextureManager::Instance()->ReleaseInstance();
	DirectGraphics::Instance()->ReleaseInstance();
	ShaderManager::Instance()->ReleaseInstance();
	Inputter::Instance()->ReleaseInstance();
	SceneController::Instance()->ReleaseInstance();
#endif

	return 0;


}