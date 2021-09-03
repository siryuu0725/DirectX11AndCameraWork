#include "ResultScene.h"
#include "SceneController.h"
#include "../System/DirectInput.h"
#include "../System/Texture/TextureManager.h"

//コンストラクタ
ResultScene::ResultScene():
	mp_ui(nullptr)
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
	if (mp_ui == nullptr) { mp_ui = std::make_unique<ResultUI>(); }


	mp_ui->Init(); //UI初期化

	//次のステップへ
	m_cur_step = SceneStep::MainStep;
}

//更新ステップ関数
void ResultScene::MainStep()
{
	//Enterキーが押されたら次のステップへ
	if (Inputter::Instance()->GetKeyDown(Inputter::ReturnKey))
	{
		m_cur_step = SceneStep::EndStep;
	}
	//ESCキーが押されたら次のステップへ(ゲーム終了フラグtrue)
	if (Inputter::Instance()->GetKeyDown(Inputter::ESCKey))
	{
		m_cur_step = SceneStep::EndStep;
		is_game_end = true;
	}
}

//終了ステップ関数
void ResultScene::EndStep()
{
	//初期化ステップに変更
	m_cur_step = SceneStep::InitStep;

	//タイトルシーンへ変更
	SceneController::Instance()->SetSceneId(SceneId::Title);

	//シーン移行フラグtrue
	m_is_change_scene = true;
}

//インスタンス返還関数
std::unique_ptr<SceneBase> ResultScene::Instance()
{
	return static_cast<std::unique_ptr<SceneBase>>(std::make_unique<ResultScene>());
}


