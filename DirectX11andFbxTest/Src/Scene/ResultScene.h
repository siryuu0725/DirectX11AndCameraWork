/**
 * @file GameScene.h
 * @brief リザルトシーン(エンドシーン)を管理する
 * @details 中身：リザルトシーンクラス
**/

#ifndef RESULTSCENE_H_
#define RESULTSCENE_H_
#include"SceneBase.h"
#include "../Scene/SceneController.h"
#include "../UI/ResultUI.h"


/**
* リザルトシーンクラス
*/
class ResultScene :public SceneBase
{
public:
	ResultScene();
	~ResultScene() {}

#pragma region 各ステップ処理
	/**
 　　* @brief  初期化ステップ関数
 　　* @details UIの初期化を行う
 　　*/
	virtual void InitStep()override;

	/**
 　　* @brief  更新ステップ関数
 　　* @details 本編の更新処理を行う
 　　*/
	virtual void MainStep()override;

	/**
 　　* @brief  終了ステップ関数
 　　* @details 次のシーン移行前のUIの解放を行う
 　　*/
	virtual void EndStep()override;

#pragma endregion

	/**
	 * @brief  描画まとめ関数
	 * @details UIの描画処理を行う
	 */
	virtual void Draw()override;

	/**
	 * @brief  インスタンス返還関数
	 * @details 管理クラスのポインタ配列に返す
	 */
	static SceneBase* Instance();

private:
	ResultUI* mp_ui; //!リザルト画面用UI
};

#endif