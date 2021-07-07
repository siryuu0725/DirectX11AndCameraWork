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

constexpr __int16 ObjectNum = 5;   //!使用しているオブジェクトの数


int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmpLine,
	INT       nCmdShow)
{

	//Window作成
	Window window;

	// ウィンドウを作成
	if (window.MakeWindow("FbxRender", 1920, 1080) == false)
	{
		return 0;
	}

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

			if (Inputter::Instance()->GetKeyDown(Inputter::ESCKey))
			{
				PostQuitMessage(0);
			}

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

	return 0;


}