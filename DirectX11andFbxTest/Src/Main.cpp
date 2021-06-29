#include <Windows.h>
#include <time.h>
#include <mmsystem.h>
#include "System/Window.h"
#include "System/DirectGraphics.h"
#include "System/DirectInput.h"
#include "System/Fbx/FbxMeshFile.h"
#include "Object/Camera.h"
#include "Object/Player.h"
#include "Object/Floor.h"
#include "Object/SkyDome.h"
#include "Object/Block/BlockController.h"

#pragma comment(lib, "winmm.lib")

constexpr __int16 ObjectNum = 4;   //!使用しているオブジェクトの数


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

	//FbxMeshFile* model;
	BlockController* block = new BlockController;
	Camera* camera = new Camera(block);
	Player* player = new Player(camera);
	Floor* floor = new Floor;
	SkyDome* skydome = new SkyDome;

	//各オブジェクトを配列でまとめる
	ObjectBase* obj[ObjectNum] =
	{ camera ,player ,floor,skydome };

	for (int i = 0; i < ObjectNum; i++)
	{
		obj[i]->Init();
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
			//入力クラス更新
			Inputter::Instance()->UpdateInput();

			player->Update();
			block->Update();
			camera->Update();

			DirectGraphics::Instance()->ShadowStartRendering();
			DirectGraphics::Instance()->SetUpShadowRnderTaget();

			block->ShadowDraw();
			player->ShadowDraw();

			DirectGraphics::Instance()->StartRendering();
			DirectGraphics::Instance()->SetUpRnderTaget();

			floor->Draw();
			skydome->Draw();
			block->Draw();

			player->Draw();

			DirectGraphics::Instance()->FinishRendering();

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

	//オブジェクト解放
	for (int i = 0; i < ObjectNum; i++)
	{
		obj[i]->ReleaseModel();
		delete obj[i];
	}

	DirectGraphics::Instance()->Release();

	return 0;


}