/**
 * @file TitleUI.h
 * @brief タイトルシーン画面でのUIを扱う
 * @details 中身：タイトル用UIクラス
**/
#ifndef TITLEUI_H_
#define TITLEUI_H_

#include "UICategory.h"
#include "../System/DirectGraphics.h"
#include "../System/Texture/TextureManager.h"
#include "../Utility/Utility.h"

/**
* タイトル用UIクラス
*/

class TitleUI
{
public:
	//!タイトルUI情報
	struct TitleUIInfo
	{
		TitleUIInfo() :
			m_pos{ Vector3(0.0f,0.0f,0.0f) },
			m_scale { Vector3(0.0f,0.0f,0.0f) },
			m_rot{ Vector3(0.0f,0.0f,0.0f) }
		{}

		Vector3 m_pos[(__int8)TitleUICategory::CategoryMax];   //Texture座標
		Vector3 m_scale[(__int8)TitleUICategory::CategoryMax]; //Textureサイズ
		Vector3 m_rot[(__int8)TitleUICategory::CategoryMax];   //Texture回転角度

	};

public:
	TitleUI() {}
	~TitleUI() {}

	/**
	 * @brief  初期化関数
	 * @details オブジェクト情報初期化
	 */
	void Init();

	/**
	 * @brief  描画関数
	 */
	void Draw();

private:
	TitleUIInfo m_ui_info;  //!タイトルUI情報
};
#endif