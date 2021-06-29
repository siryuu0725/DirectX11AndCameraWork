#include "Window.h"

const char* Window::ClassName = "DX11PracticeWindow";
Vector2 window_size;
Vector2 GetWindowSize() { return window_size; }

//ウィンドウプロシージャ
LRESULT CALLBACK Window::WindowProc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam)
{
	switch (message_id)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(window_handle, message_id, wparam, lparam);
		break;
	}

	return 0;
}

//ウィンドウ生成関数
bool Window::MakeWindow(const char* title_, unsigned int width_, unsigned int height_)
{
	WNDCLASSEX window_class = {
		sizeof(WNDCLASSEXA),			//構造体のサイズ
		CS_HREDRAW | CS_VREDRAW,		//クラスのスタイル
		WindowProc,						//ウィンドウプロシージャ
		0,								//補助メモリ
		0,								//補助メモリ
		GetModuleHandle(nullptr),		//このプログラムのインスタンスハンドル
		LoadIcon(nullptr, IDI_APPLICATION),//アイコン画像
		LoadCursor(nullptr, IDC_ARROW),	//カーソル画像
		nullptr,						//背景ブラシ(背景色)
		nullptr,						//メニュー名
		Window::ClassName,				//クラス名									
		nullptr							//小さいアイコン
	};

	//構造体の登録
	if (RegisterClassEx(&window_class) == 0)
	{
		return 0;
	}

	//ウィンドウ作成
	HWND window_handle = CreateWindow(
		Window::ClassName,
		title_,
		//WS_VISIBLE | WS_POPUP,
		(WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME) | WS_VISIBLE,
		CW_USEDEFAULT,
		0,
		width_,
		height_,
		nullptr,
		nullptr,
		GetModuleHandle(nullptr),
		nullptr);

	if (window_handle == nullptr)
	{
		return 0;
	}

	RECT window_rect;
	RECT client_rect;

	GetWindowRect(window_handle, &window_rect);
	GetClientRect(window_handle, &client_rect);

	int frame_size_x = (window_rect.right - window_rect.left) - (client_rect.right - client_rect.left);
	int frame_size_y = (window_rect.bottom - window_rect.top) - (client_rect.bottom - client_rect.top);

	SetWindowPos(window_handle, nullptr, CW_USEDEFAULT, CW_USEDEFAULT, frame_size_x + width_, frame_size_y + height_, SWP_NOMOVE);

	ShowWindow(window_handle, SW_SHOW);
	UpdateWindow(window_handle);

	window_size.x = (float)width_;
	window_size.y = (float)height_;

	return window_handle;
}
