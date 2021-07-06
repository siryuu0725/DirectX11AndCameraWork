﻿#include "TitleScene.h"
#include "SceneController.h"
#include "../System/DirectInput.h"
#include "../System/Texture/TextureManager.h"

TitleScene::TitleScene()
{
	m_is_change_scene = false;
	m_cur_step = SceneStep::InitStep;
}

//初期化ステップ関数
void TitleScene::InitStep()
{
	if (mp_ui == nullptr) { mp_ui = new TitleUI; }

	mp_ui->Init(); //UI初期化

	TextureManager::Instance()->Load();

	m_cur_step = SceneStep::MainStep;
}

//更新ステップ関数
void TitleScene::MainStep()
{

	if (Inputter::Instance()->GetKeyDown(Inputter::ReturnKey))
	{
		m_cur_step = SceneStep::EndStep;
	}
}

//終了ステップ関数
void TitleScene::EndStep()
{
	delete mp_ui;
	mp_ui = nullptr;

	//初期化ステップに変更
	m_cur_step = SceneStep::InitStep;

	SceneController::Instance()->SetSceneId(SceneId::Game);
	m_is_change_scene = true;
}

//描画情報送信まとめ関数
void TitleScene::Draw()
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

//インスタンス返還関数
SceneBase* TitleScene::Instance()
{
	return static_cast<SceneBase*>(new TitleScene);
}


