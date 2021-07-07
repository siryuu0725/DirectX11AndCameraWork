#include "DirectInput.h"
#include <Windows.h>
#include <dinput.h>
#include "Window.h"

//ライブラリ側のキー情報配列
int g_KeyInfo[] = {
	DIK_ESCAPE,
	DIK_SPACE,
	DIK_A,
	DIK_W,
	DIK_S,
	DIK_D,
	DIK_F,
	DIK_E,
	DIK_Q,
	DIK_T,
	DIK_Y,
	DIK_J,
	DIK_L,
	DIK_I,
	DIK_K,
	DIK_RETURN,
};


Inputter* Inputter::mp_instance = nullptr;

Inputter* Inputter::Instance()
{
	if (mp_instance == nullptr) { mp_instance = new Inputter; }

	return mp_instance;
}

//入力情報初期化まとめ関数
bool Inputter::InitInput()
{
	//インターフェース取得
	if (CreateInputInterface() == false)
	{
		return false;
	}

	//マウスデバイス設定
	if (CreateMouseDevice() == false)
	{
		EndInput();
		return false;
	}

	//!キーボードデバイス設定
	if (CreateKeyboardDevice() == false) {
		EndInput();
		return false;
	}

	ZeroMemory(&m_current_mouse_state, sizeof(DIMOUSESTATE));
	ZeroMemory(&m_prev_mouse_state, sizeof(DIMOUSESTATE));

	return true;
}

//入力情報解放関数
void Inputter::EndInput()
{
	//インターフェース
	if (Ip_interface != nullptr)
	{
		Ip_interface->Release();
		Ip_interface = nullptr;
	}

	//マウス用デバイス
	if (m_mouse_device != nullptr)
	{
		m_mouse_device->Release();
		m_mouse_device = nullptr;
	}

	//キーボード用デバイス
	if (m_key_device != nullptr)
	{
		m_key_device->Release();
		m_key_device = nullptr;
	}
}

//インターフェース取得関数
bool Inputter::CreateInputInterface()
{
	//IDirectInput8インターフェイスの取得
	HRESULT hr = DirectInput8Create(GetModuleHandle(nullptr),
		0x0800,
		IID_IDirectInput8,
		(void**)&Ip_interface,
		nullptr);
	if (FAILED(hr))
	{
		return false;
	}
	return true;
}

//マウスデバイス設定関数
bool Inputter::CreateMouseDevice()
{
	//マウス用にデバイスオブジェクトを作成
	if (FAILED(Ip_interface->CreateDevice(GUID_SysMouse, &m_mouse_device, nullptr)))
	{
		//デバイスの作成に失敗
		return false;
	}

	//データフォーマットを設定
	if (FAILED(m_mouse_device->SetDataFormat(&c_dfDIMouse)))
	{
		// データフォーマットに失敗
		return false;
	}

	//モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_mouse_device->SetCooperativeLevel(
		FindWindowA(Window::ClassName, nullptr),
		DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
	{
		//モードの設定に失敗
		return false;
	}

	//デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	//相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	if (FAILED(m_mouse_device->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		//デバイスの設定に失敗
		return false;
	}

	//入力制御開始
	m_mouse_device->Acquire();

	return true;
}

//キーボードデバイス設定関数
bool Inputter::CreateKeyboardDevice()
{
	//マウス用にデバイスオブジェクトを作成
	if (FAILED(Ip_interface->CreateDevice(GUID_SysKeyboard, &m_key_device, nullptr)))
	{
		//デバイスの作成に失敗
		return false;
	}

	//データフォーマットを設定
	if (FAILED(m_key_device->SetDataFormat(&c_dfDIKeyboard)))
	{
		// データフォーマットに失敗
		return false;
	}

	//モードを設定（フォアグラウンド＆非排他モード）
	if (FAILED(m_key_device->SetCooperativeLevel(
		FindWindowA(Window::ClassName, nullptr),
		DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))
	{
		//モードの設定に失敗
		return false;
	}

	//デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL;	//相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	if (FAILED(m_key_device->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		//デバイスの設定に失敗
		return false;
	}

	//入力制御開始
	m_key_device->Acquire();

	return true;
}

//キーボードの入力情報更新関数
void Inputter::UpdateKeyState()
{
	BYTE Key[256];
	HRESULT hr = m_key_device->GetDeviceState(256, Key);

	if (FAILED(hr))
	{
		return;
	}

	for (int i = 0; i < MaxKeyInfo; i++)
	{
		if (Key[g_KeyInfo[i]] & 0x80)
		{
			if (g_InputState[i] == InputState::NotPush || g_InputState[i] == InputState::PushUp)
			{
				g_InputState[i] = InputState::PushDown;
			}
			else
			{
				g_InputState[i] = InputState::Push;
			}
		}
		else
		{
			if (g_InputState[i] == InputState::Push || g_InputState[i] == InputState::PushDown)
			{
				g_InputState[i] = InputState::PushUp;
			}
			else
			{
				g_InputState[i] = InputState::NotPush;
			}
		}
	}
}

//キーボードの入力判定関数(押されている間)
bool Inputter::GetKey(KeyInfo key_)
{
	return (g_InputState[key_] == InputState::Push);
}

//キーボードの入力判定関数(押された瞬間)
bool Inputter::GetKeyDown(KeyInfo key_)
{
	return (g_InputState[key_] == InputState::PushDown);
}

//入力情報の更新関数
void Inputter::UpdateInput()
{
	UpdateMouse();
	UpdateKeyState();

	m_prev_mouse_state = m_current_mouse_state;
	//マウスの状態を取得します
	HRESULT	hr = m_mouse_device->GetDeviceState(sizeof(DIMOUSESTATE), &m_current_mouse_state);
	if (FAILED(hr))
	{
		m_mouse_device->Acquire();
		hr = m_mouse_device->GetDeviceState(sizeof(DIMOUSESTATE), &m_current_mouse_state);
	}
}

//マウスの入力判定関数
bool Inputter::OnMouseDown(MouseButton button_type_)
{
	if (!(m_prev_mouse_state.rgbButtons[button_type_] & MOUSE_ON_VALUE) &&
		m_current_mouse_state.rgbButtons[button_type_] & MOUSE_ON_VALUE)
	{
		return true;
	}

	return false;
}

//Instance解放関数(Debug用)
void Inputter::ReleaseInstance()
{
	delete mp_instance;
	mp_instance = nullptr;
}

//マウスの入力情報更新関数
void Inputter::UpdateMouse()
{
	Vector2 prev = m_mouse_pos;
	POINT p;
	GetCursorPos(&p);

	m_mouse_pos.x = (float)p.x;
	m_mouse_pos.y = (float)p.y;
}