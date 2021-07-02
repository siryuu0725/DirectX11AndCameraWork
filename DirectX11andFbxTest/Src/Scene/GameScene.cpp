﻿#include "GameScene.h"
#include "../Utility/Collision/ObjectCollision.h"
#include "../System/DirectInput.h"

//コンストラクタ
GameScene::GameScene() :
	mp_camera(nullptr),
	mp_block(nullptr), 
	mp_player(nullptr), 
	mp_sky_dome(nullptr),
	mp_floor(nullptr)
{
	m_cur_step = SceneStep::InitStep;
}

//描画情報送信まとめ関数
void GameScene::Draw()
{
	//オブジェクト
	DrawObject();
}

//初期化ステップ関数
void GameScene::InitStep()
{
	//各読み込み関数をマルチ化
	thread_h = CreateThread(
		nullptr,
		0,
		this->LoadResorse,
		nullptr,
		0,
		&thread_id);

	//オブジェクトインスタンス化
	if (mp_block == nullptr) { mp_block = new BlockController; }
	if (mp_sky_dome == nullptr) { mp_sky_dome = new SkyDome; }
	if (mp_floor == nullptr) { mp_floor = new Floor; }
	if (mp_camera == nullptr) { mp_camera = new Camera(mp_block); }
	if (mp_player == nullptr) { mp_player = new Player(mp_camera); }

	//オブジェクト初期化
	InitObject();

	//スレッドステップへ
	m_cur_step = SceneStep::ThreadStep;
}

//ロード画面更新ステップ関数
void GameScene::UpdateThreadStep()
{
	//ロード画面演出用
	if (WaitForSingleObject(thread_h, 0) != WAIT_OBJECT_0)
	{
		//mp_load_ui->Update();
		return;
	}

	m_cur_step = SceneStep::MainStep;
}


//更新ステップ関数
void GameScene::MainStep()
{
	//入力クラス更新
	Inputter::Instance()->UpdateInput();

	//オブジェクト更新
	mp_player->Update();
	mp_block->Update();
	mp_camera->Update();

	
	//ポーズ画面切り替え
	if(Inputter::Instance()->GetKeyDown(Inputter::ESCKey))
	{
		m_cur_step = SceneStep::EndStep;
	}
}

//終了ステップ関数
void GameScene::EndStep()
{
	//各オブジェクト解放
	DeleteObject();

	//当たり判定用情報リセット
	ObjectCollision::Instance()->ResetObjectInfo();

	//タイトルに戻る場合
	if (Inputter::Instance()->GetKeyDown(Inputter::ReturnKey))
	{
		m_cur_step = SceneStep::EndStep;
	}
	
	m_is_change_scene = true;

#ifdef _DEBUG
	ObjectCollision::Instance()->ReleaseInstance();
	FbxController::Instance()->ReleaseInstance();
#endif
}

//オブジェクト初期化関数
void GameScene::InitObject()
{
	//各オブジェクトを配列でまとめる
	ObjectBase* obj[ObjectNum] =
	{ mp_block,mp_camera ,mp_player ,mp_floor,mp_sky_dome };

	for (int i = 0; i < ObjectNum; i++)
	{
		obj[i]->Init();
	}
}

//オブジェクト更新関数
void GameScene::UpdateObject()
{
	mp_player->Update();
	mp_camera->Update();
}

//オブジェクト描画情報送信関数
void GameScene::DrawObject()
{
	//影描画開始
	DirectGraphics::Instance()->ShadowStartRendering();
	DirectGraphics::Instance()->SetUpShadowRnderTaget();

	mp_block->ShadowDraw();
	mp_player->ShadowDraw();

	//メイン描画開始
	DirectGraphics::Instance()->StartRendering();
	DirectGraphics::Instance()->SetUpRnderTaget();

	mp_floor->Draw();
	mp_sky_dome->Draw();
	mp_block->Draw();

	mp_player->Draw();

	//描画終了
	DirectGraphics::Instance()->FinishRendering();
}

//オブジェクト解放関数
void GameScene::DeleteObject()
{
	//各オブジェクトを配列でまとめる
	ObjectBase* obj[ObjectNum] =
	{ mp_block,mp_camera ,mp_player ,mp_floor,mp_sky_dome };

	for (int i = 0; i < ObjectNum; i++)
	{
		obj[i]->ReleaseModel();
		delete obj[i];
	}
	
	mp_camera = nullptr; //カメラ	
	mp_block = nullptr; //ブロック	
	mp_player = nullptr; //プレイヤー	
	mp_sky_dome = nullptr;//背景	
	mp_floor = nullptr; //ステージ床
}

//インスタンス返還関数
SceneBase* GameScene::Instance()
{
	return static_cast<SceneBase*>(new GameScene);
}

DWORD WINAPI GameScene::LoadResorse(LPVOID lpparm)
{


	return 0;
}