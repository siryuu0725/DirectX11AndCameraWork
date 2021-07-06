#include "ResultScene.h"
#include "SceneController.h"
#include "../System/DirectInput.h"
#include "../System/Texture/TextureManager.h"

//コンストラクタ
ResultScene::ResultScene()
	/*mp_ui(nullptr)*/
{
	m_is_change_scene = false;

	m_cur_step = SceneStep::InitStep;	
}

//描画情報送信まとめ関数
void ResultScene::Draw()
{
	//メイン描画開始
	DirectGraphics::Instance()->StartRendering();
	DirectGraphics::Instance()->SetUpRnderTaget();

	if (mp_ui != nullptr)
	{
		mp_ui->Draw();
	}

	//描画終了
	DirectGraphics::Instance()->FinishRendering();
}

//初期化ステップ関数
void ResultScene::InitStep()
{
	if (mp_ui == nullptr) { mp_ui = new ResultUI; }

	mp_ui->Init(); //UI初期化

	//次のステップへ
	m_cur_step = SceneStep::MainStep;
}

//更新ステップ関数
void ResultScene::MainStep()
{
	if (Inputter::Instance()->GetKeyDown(Inputter::ReturnKey))
	{
		m_cur_step = SceneStep::EndStep;
	}
}

//終了ステップ関数
void ResultScene::EndStep()
{
	//UI解放
	delete mp_ui;
	mp_ui = nullptr;

	SceneController::Instance()->SetSceneId(SceneId::Title);

	//シーン移行フラグtrue
	m_is_change_scene = true;
}

//インスタンス返還関数
SceneBase* ResultScene::Instance()
{
	return static_cast<SceneBase*>(new ResultScene);
}


