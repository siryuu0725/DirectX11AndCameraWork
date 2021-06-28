/**
 * @file Window.h
 * @brief Window作成を行う
**/
#ifndef WINDOW_H_
#define WINDOW_H_

#include<Windows.h>
#include "../Utility/Utility.h"

Vector2 GetWindowSize();

class Window
{
public:
	static const char* ClassName;	//!< ウィンドウクラス登録時のクラス名

public:
	Window() {}
	~Window() {}

public:
	/**
	* @brief ウィンドウプロシージャ
	* @param[in] window_handle イベントが発生したウィンドウのハンドル
	* @param[in] message_id イベントメッセージ
	* @param[in] wparam イベント情報
	* @param[in] lparam イベント情報
	* @return メッセージ処理結果
	*/
	static LRESULT CALLBACK WindowProc(HWND window_handle, UINT message_id, WPARAM wparam, LPARAM lparam);

	/**
	* @brief ウィンドウ生成関数@n
	* @details ウィンドウを生成して成功したらtrue、失敗したらfalseを返す
	* @return 生成の成否 成功(true)
	*/
	bool MakeWindow(const char* title, unsigned int width, unsigned int height);
};




#endif