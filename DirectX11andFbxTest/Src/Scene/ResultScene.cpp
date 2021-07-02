#include "ResultScene.h"
#include"../System/DirectInput.h"
#include "SceneController.h"

//コンストラクタ
ResultScene::ResultScene()
	/*mp_ui(nullptr)*/
{
	m_cur_step = SceneStep::InitStep;	
}

//描画情報送信まとめ関数
void ResultScene::Draw()
{
	//mp_ui->Draw();
}

//初期化ステップ関数
void ResultScene::InitStep()
{
	////UIインスタンス化
	//if (mp_ui == nullptr) { mp_ui = new ResultUI(); }

	////UI初期化
	//mp_ui->Init();

	//次のステップへ
	m_cur_step = SceneStep::MainStep;
}

//更新ステップ関数
void ResultScene::MainStep()
{
	if (Inputter::Instance()->OnMouseDown(Inputter::Left))
	{
		//次のステップへ
		m_cur_step = SceneStep::EndStep;
	}
}

//終了ステップ関数
void ResultScene::EndStep()
{
	//UI解放
	/*mp_ui->ReleaseTex();
	delete mp_ui;
	mp_ui = nullptr;*/

	SceneController::Instance()->SetSceneId(SceneId::Title);

	//シーン移行フラグtrue
	m_is_change_scene = true;
}

//インスタンス返還関数
SceneBase* ResultScene::Instance()
{
	return static_cast<SceneBase*>(new ResultScene);
}


