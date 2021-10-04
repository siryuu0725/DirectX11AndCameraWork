#include "TitleScene.h"
#include "SceneController.h"
#include "../System/DirectInput.h"
#include "../System/Texture/TextureManager.h"

//コンストラクタ
TitleScene::TitleScene():
	mp_ui(nullptr)
{
	m_is_change_scene = false;
	m_cur_step = SceneStep::InitStep;
}

//初期化ステップ関数
void TitleScene::InitStep()
{
	if (mp_ui == nullptr) { mp_ui = std::make_unique<TitleUI>(); }

	mp_ui->Init(); //UI初期化

	TextureManager* tex_instance = TextureManager::Instance();

	tex_instance->Load();

	m_cur_step = SceneStep::MainStep;
}

//更新ステップ関数
void TitleScene::MainStep()
{
	//Enterキーが押されたら次のステップへ
	if (input_instance->GetKeyDown(Inputter::ReturnKey))
	{
		m_cur_step = SceneStep::EndStep;
	}
	//ESCキーが押されたら次のステップへ(ゲーム終了フラグtrue)
	if (input_instance->GetKeyDown(Inputter::ESCKey))
	{
		m_cur_step = SceneStep::EndStep;
		is_game_end = true;
	}
}

//終了ステップ関数
void TitleScene::EndStep()
{
	//初期化ステップに変更
	m_cur_step = SceneStep::InitStep;

	//ゲームシーンへ変更
	scene_instance->SetSceneId(SceneId::Game);
	m_is_change_scene = true;
}

//描画情報送信まとめ関数
void TitleScene::Draw()
{
	//メイン描画開始
	graphics_instance->StartRendering();
	graphics_instance->SetUpRnderTaget();

	if (mp_ui != nullptr)
	{
		mp_ui->Draw();
	}

	//描画終了
	graphics_instance->FinishRendering();
}

//インスタンス返還関数
std::unique_ptr<SceneBase> TitleScene::Instance()
{
	return static_cast<std::unique_ptr<SceneBase>>(std::make_unique<TitleScene>());
}


