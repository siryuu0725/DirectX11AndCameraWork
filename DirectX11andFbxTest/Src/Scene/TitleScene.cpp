#include "TitleScene.h"
#include "SceneController.h"

//初期化ステップ関数
void TitleScene::InitStep()
{
	//if (mp_ui == nullptr) { mp_ui = new TitleUI; }

	//mp_ui->Init(); //UI初期化

	m_cur_step = SceneStep::MainStep;
}

//更新ステップ関数
void TitleScene::MainStep()
{
	//TitleUI::TitleUIInfo ui_info;

	////UI更新
	//mp_ui->Update();

	m_cur_step = SceneStep::EndStep;

}

//終了ステップ関数
void TitleScene::EndStep()
{
	//UI解放
	/*mp_ui->ReleaseTex();
	delete mp_ui;
	mp_ui = nullptr;*/

	//初期化ステップに変更
	m_cur_step = SceneStep::InitStep;

	SceneController::Instance()->SetSceneId(SceneId::Game);
	m_is_change_scene = true;
}

//描画情報送信まとめ関数
void TitleScene::Draw()
{
	//if (mp_ui != nullptr)
	//{
	//	mp_ui->Draw();
	//}
}

//インスタンス返還関数
SceneBase* TitleScene::Instance()
{
	return static_cast<SceneBase*>(new TitleScene);
}


