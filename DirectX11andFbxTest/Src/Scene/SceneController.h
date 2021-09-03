/**
 * @file SceneController.h
 * @brief 各シーン管理を管理する
 * @details 中身：シーン管理クラス、シーンID列挙体
**/
#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_
#include"SceneBase.h"
#include <string>

//!シーンID
enum class SceneId
{
	Title,  //!タイトル
	Game,	//!ゲーム
	Result,	//!リザルト
	Max		//!シーン数
};

/**
* シーン管理クラス
*/
class SceneController
{
private:
	SceneController() :mp_scene(nullptr) {}
	~SceneController();
public:

	/**
　　* @brief  インスタンス化関数
　　* @details 初回のみインスタンス化し、それ以外は実態を呼ぶ
　　*/
	static SceneController* Instance();

	/**
	 * @brief  更新関数
	 * @details 更新処理まとめ関数
	 */
	void Update();

	/**
	 * @brief   描画関数
	 */
	void Draw();

	/**
	 * @brief   現在シーンSetter
	 */
	void SetSceneId(SceneId sceneid_);

	/**
	 * @brief  シーン切り替え判定関数
	 * @details 各シーンで切り替え判定を行う
	 */
	void ChangeScene();

	/**
	*	@brief Instance解放関数(Debug用)
	*	@details 手動でインスタンスを解放する
	*/
	void ReleaseInstance();

	/**
	 * @brief  各ゲーム管理クラスアドレス配列
	 * @details 配列に指定したゲームを入れるとそのゲームの管理クラスのnewが返ってくる
	 */
	static std::unique_ptr<SceneBase>(*s_controller_array[static_cast<int>(SceneId::Max)])();

private:
	static SceneController* mp_instance;
	std::unique_ptr<SceneBase> mp_scene;


	SceneId m_scene_id;

};

#endif // !SCENEMANAGER_H_