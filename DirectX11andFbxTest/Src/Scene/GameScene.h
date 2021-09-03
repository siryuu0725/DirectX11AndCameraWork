/**
 * @file GameScene.h
 * @brief ゲームシーンを管理する
 * @details 中身：ゲームシーンクラス
**/

#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "SceneBase.h"
#include "../Scene/SceneController.h"

#include "../Object/Block/BlockController.h"
#include "../Object/Camera.h"
#include "../Object/Player.h"
#include "../Object/Floor.h"
#include "../Object/SkyDome.h"

#include "../UI/LoadUI.h"

#include <Windows.h>

constexpr __int16 ObjectNum = 5;   //!使用しているオブジェクトの数

/**
* ゲームシーンクラス
*/
class GameScene :public SceneBase
{
public:
	GameScene();
	~GameScene() {}

	/**
	 * @brief  描画まとめ関数
	 * @details 各オブジェクト、UIなどの描画処理を行う
	 */
	virtual void Draw()override;

#pragma region 各ステップ処理

	/**
 　　* @brief  初期化ステップ関数
 　　* @details 各オブジェクト、UIの初期化を行う
 　　*/
	virtual void InitStep()override;

	/**
 　　* @brief  更新ステップ関数
 　　* @details 本編の更新処理を行う
 　　*/
	virtual void MainStep()override;

	/**
 　　* @brief  終了ステップ関数
 　　* @details 次のシーン移行前のオブジェクトやUIの解放を行う
 　　*/
	virtual void EndStep()override;

	/**
　　* @brief  スレッド更新ステップ関数
　　* @details 各Fbxオブジェクトやテクスチャ読み込み時のロード画面更新ステップ、
　　*/
	virtual void UpdateThreadStep()override;

#pragma endregion

#pragma region 各オブジェクトまとめ関数
	/**
 　　* @brief  オブジェクト初期化関数
 　　* @details 各オブジェクトの初期化を行う
 　　*/
	void InitObject();

	/**
 　　* @brief  オブジェクト更新関数
 　　* @details  各オブジェクトの更新処理を行う
 　　*/
	void UpdateObject();

	/**
 　　* @brief  オブジェクト描画情報送信関数
 　　* @details 各オブジェクトの描画情報送信を行う
 　　*/
	void DrawObject();

#pragma endregion

	/**
     * @brief  インスタンス返還関数
     * @details 管理クラスのポインタ配列に返す
     */
	static std::unique_ptr<SceneBase> Instance();


	/**
	 * @brief  読み込み用スレッド関数
	 * @details 各Fbxオブジェクトの読み込みを行う
	 */
	static DWORD WINAPI LoadResorse(LPVOID lpparm_);
private:
	std::unique_ptr<Camera> mp_camera;          //!カメラオブジェクト
	std::unique_ptr<BlockController> mp_block;  //!ブロック管理オブジェクト
	std::unique_ptr<Player> mp_player;          //!プレイヤーオブジェクト
	std::unique_ptr<SkyDome> mp_sky_dome;       //!背景オブジェクト
	std::unique_ptr<Floor> mp_floor;            //!ステージ床オブジェクト

	std::unique_ptr<LoadUI> mp_ui; 

	HANDLE thread_h;
	DWORD thread_id;
};

#endif