/**
 * @file ResultUI.h
 * @brief リザルトシーン画面でのUIを扱う
 * @details 中身：リザルト用UIクラス
**/
#ifndef RESULTUI_H_
#define RESULTUI_H_

#include "UICategory.h"
#include "../System/DirectGraphics.h"
#include "../System/Texture/TextureManager.h"
#include "../Utility/Utility.h"

/**
* リザルト用UIクラス
*/
class ResultUI
{
public:
	//!リザルトUI情報
	struct ResultUIInfo
	{
		ResultUIInfo() :
			m_pos{ Vector3(0.0f,0.0f,0.0f) },
			m_scale{ Vector3(0.0f,0.0f,0.0f) },
			m_rot{ Vector3(0.0f,0.0f,0.0f) }
		{}

		Vector3 m_pos[(__int8)ResultUICategory::CategoryMax];   //Texture座標
		Vector3 m_scale[(__int8)ResultUICategory::CategoryMax]; //Textureサイズ
		Vector3 m_rot[(__int8)ResultUICategory::CategoryMax];   //Texture回転角度

	};

public:
	ResultUI() {}
	~ResultUI() {}

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
	ResultUIInfo m_ui_info;  //!リザルトUI情報
};
#endif