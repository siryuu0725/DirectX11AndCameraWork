/**
 * @file LoadUI.h
 * @brief ゲームシーン中移行中のロード画面を扱う
 * @details 中身：ロード用UIクラス
**/
#ifndef LOADUI_H_
#define LOADUI_H_

#include "UICategory.h"
#include "../System/DirectGraphics.h"
#include "../System/Texture/TextureManager.h"
#include "../Utility/Utility.h"

constexpr float IconRoteSpeed = 2.0f; //アイコンの回転速度

/**
* ロード用UIクラス
*/
class LoadUI
{
public:
	//!ロードUI情報
	struct LoadUIInfo
	{
		LoadUIInfo() :
			m_pos{ Vector3(0.0f,0.0f,0.0f) },
			m_scale{ Vector3(0.0f,0.0f,0.0f) },
			m_rot{ Vector3(0.0f,0.0f,0.0f) }
		{}

		Vector3 m_pos[(__int8)LoadUICategory::CategoryMax];   //Texture座標
		Vector3 m_scale[(__int8)LoadUICategory::CategoryMax]; //Textureサイズ
		Vector3 m_rot[(__int8)LoadUICategory::CategoryMax];   //Texture回転角度

	};

public:
	LoadUI() {}
	~LoadUI() {}

	/**
	 * @brief  初期化関数
	 * @details オブジェクト情報初期化
	 */
	void Init();

	/**
	 * @brief  描画関数
	 */
	void Draw();

	/**
	 * @brief  更新関数
	 * @details ロードアイコンの回転処理
	 */
	void Update();

private:
	LoadUIInfo m_ui_info;  //!ロードUI情報
};

#endif