#include "SceneController.h"
#include"../Scene/TitleScene.h"
#include"../Scene/GameScene.h"
#include"../Scene/ResultScene.h"
#include"../System/DirectInput.h"
#include"../System/DirectGraphics.h"

SceneController* SceneController::mp_instance = nullptr;

//インスタンス化関数
SceneController* SceneController::Instance()
{
	if (mp_instance == nullptr) { mp_instance = new SceneController; }

	return mp_instance;
}

//更新関数
void SceneController::Update()
{
	//入力クラス更新
	Inputter::Instance()->UpdateInput();

	//シーン切り替え判定
	ChangeScene();

	if (mp_scene->GetIsGameEnd() == true)
	{
		PostQuitMessage(0);
	}

	//各シーン更新
	mp_scene->Update();
}

//描画関数
void SceneController::Draw()
{
	if (mp_scene->GetIsChangeScene() == false)
	{
		//描画情報送信関数
		mp_scene->Draw();
	}
}

//現在シーンSetter
void SceneController::SetSceneId(SceneId sceneid_)
{
	m_scene_id = sceneid_;
}

//シーン切り替え判定関数
void SceneController::ChangeScene()
{
	if (mp_scene == nullptr) { mp_scene = new TitleScene; }

	if (mp_scene->GetIsChangeScene() == true)
	{
		delete mp_scene;
		//指定のゲームの管理クラスに切り替える
		mp_scene = s_controller_array[static_cast<int>(m_scene_id)]();
	}
}

//各ゲーム管理クラスアドレス配列
SceneBase* (*SceneController::s_controller_array[static_cast<int>(SceneId::Max)])() =
{
	TitleScene::Instance,   //タイトル
	GameScene::Instance,	  //ゲーム
	ResultScene::Instance  //リザルト
};

//デストラクタ
SceneController::~SceneController()
{
	delete mp_scene;
}

//Instance解放関数(Debug用)
void SceneController::ReleaseInstance()
{
	delete mp_instance;
}

