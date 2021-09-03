/**
 * @file TitleScene.h
 * @brief タイトルシーンを管理する
 * @details 中身：タイトルシーンクラス
**/
#ifndef TITLESCENE_H_
#define TITLESCENE_H_
#include"SceneBase.h"
#include "../System/Texture/Texture.h"
#include "../UI/TitleUI.h"

/**
* タイトルシーンクラス
*/

class TitleScene :public SceneBase
{
public:
	TitleScene();
	~TitleScene() {}


	/**
	 * @brief  描画情報送信まとめ関数
	 * @details UIの描画情報送信処理を行う
	 */
	virtual void Draw()override;

#pragma region 各ステップ処理
	/**
 　　* @brief  初期化ステップ関数
 　　* @details UI、サウンドの初期化を行う
 　　*/
	virtual void InitStep()override;

	/**
 　　* @brief  更新ステップ関数
 　　* @details 本編の更新処理を行う
 　　*/
	virtual void MainStep()override;

	/**
 　　* @brief  終了ステップ関数
 　　* @details 次のシーン移行前のUIやサウンドの解放を行う
 　　*/
	virtual void EndStep()override;

#pragma endregion


	/**
     * @brief  インスタンス返還関数
     * @details 管理クラスのポインタ配列に返す
     */
	static std::unique_ptr<SceneBase> Instance();
	
private:
	std::unique_ptr<TitleUI> mp_ui;
};

#endif