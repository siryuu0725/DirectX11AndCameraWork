/**
 * @file SceneBase.h
 * @brief 各シーンの元となるクラス
 * @details 中身：シーンベースクラス、シーンステップ列挙体
**/
#ifndef SCENEBASE_H_
#define SCENEBASE_H_

//!シーンステップ
enum class SceneStep
{
	InitStep,	 //!初期化ステップ
	ThreadStep,	 //!ロードステップ
	MainStep,	 //!メインステップ
	EndStep,	 //!エンドステップ
};

/**
* シーンベースクラス
*/
class SceneBase
{
public:
	SceneBase() :m_cur_step(SceneStep::InitStep) {}
	virtual ~SceneBase() {}

	/**
　　* @brief  初期化ステップ関数
　　* @details 各シーン内の初期化を行う
　　*/
	virtual void InitStep() = 0;

	/**
　　* @brief  更新ステップ関数
　　* @details 本編の更新処理を行う
　　*/
	virtual void MainStep() = 0;

	/**
　　* @brief  終了ステップ関数
　　* @details 次のシーン移行前の解放処理を行う
　　*/
	virtual void EndStep() = 0;

	/**
　　* @brief  スレッド更新ステップ関数
　　* @details 各Fbxオブジェクトやテクスチャ読み込み時のロード画面更新ステップ、
　　*/
	virtual void UpdateThreadStep() {}

	/**
	 * @brief  描画情報送信まとめ関数
	 * @details UIの描画情報送信処理を行う
	 */
	virtual void Draw() = 0;

	/**
　　* @brief  ステップ処理関数
　　* @details 各ステップ処理を管理する
　　*/
	virtual void Update();

	/**
　　* @brief  シーン移行フラグGetter
　　*/
	const bool GetIsChangeScene()const {return m_is_change_scene; }

	/**
　　* @brief  ゲーム修了フラグGetter
　　*/
	const bool GetIsGameEnd()const { return is_game_end; }

protected:
	SceneStep m_cur_step;    //!現在のシーンステップ
	bool m_is_change_scene;  //!シーン移行フラグ
	bool is_game_end;        //!ゲーム修了フラグ

};


#endif