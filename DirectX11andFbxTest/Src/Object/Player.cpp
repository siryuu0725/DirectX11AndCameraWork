#include "Player.h"
#include "../Utility/Calculation.h"
#include "../System/DirectInput.h"

//初期化関数
void Player::Init()
{
	player_info.m_pos = Vector3(0.0f,0.0f, 0.0f);      //座標
	player_info.m_degree = Vector3(0.0f, 0.0f, 0.0f);  //回転角度
	player_info.m_scale = Vector3(0.1f, 0.1f, 0.1f);   //サイズ

	player_info.world_matrix = Calculation::Matrix(player_info.m_pos, player_info.m_scale, player_info.m_degree);

	player_info.m_key = "Player";  //描画用キー

	player_info.m_direction = Vector3(0.0f, 0.0f, 1.0f); //方向ベクトル
	player_info.m_old_pos = player_info.m_pos;           //1フレーム前座標
	player_info.m_speed = 1.5f;                          //移動スピード

	//Meshファイル読み込み
	FbxController::Instance()->LoadFbxMesh(player_info.m_key, "Res/FBX/unitychan_ch_change.fbx",VertexShaderType::SkinVertex);

	//Animation読み込み
	FbxController::Instance()->LoadAnimation(player_info.m_key,"Run", "Res/FBX/unitychan_RUN00_F.FBX");
	FbxController::Instance()->LoadAnimation(player_info.m_key,"Stay", "Res/FBX/unitychan_WAIT00.FBX");
	//動かしたいAnimationのキー設定
	FbxController::Instance()->SetAnimationName(player_info.m_key, "Run");
}

//更新関数
void Player::Update()
{	
	Move(); //移動

	//ワールド行列更新
	player_info.world_matrix = Calculation::Matrix(player_info.m_pos, player_info.m_scale, player_info.m_degree);
}

//描画関数
void Player::Draw()
{
	FbxController::Instance()->DrawFbx(player_info.m_key, player_info.world_matrix, DrawType::MainDraw);
}

//FBXモデル解放関数
void Player::ReleaseModel()
{
	FbxController::Instance()->ReleaseModel(player_info.m_key);
}

//影描関数
void Player::ShadowDraw()
{
	FbxController::Instance()->DrawFbx(player_info.m_key, player_info.world_matrix, DrawType::ShadowDraw);
}


//移動関数
void Player::Move()
{
	//更新前座標保存
	player_info.m_old_pos = player_info.m_pos;

	player_info.m_speed = 0.0f;

	//カメラ情報取得
	Camera::CameraInfo camera_info;
	mp_camera->GetCameraInfo(camera_info);
	//カメラの方向ベクトル保存
	player_info.m_camera_dir = camera_info.m_direction;

	//前移動
	if (Inputter::Instance()->GetKey(Inputter::WKey))
	{
		Vector3 m_top_vec = Vector3(player_info.m_camera_dir.x, 0.0f, player_info.m_camera_dir.z); //!左方向ベクトル
		Calculation::ThreeNormalization(m_top_vec);

		RoteDirection(m_top_vec);
	}
	//後ろ移動
	if (Inputter::Instance()->GetKey(Inputter::SKey))
	{
		Vector3 m_under_vec = Vector3(-player_info.m_camera_dir.x, 0.0f, -player_info.m_camera_dir.z); //!左方向ベクトル
		Calculation::ThreeNormalization(m_under_vec);

		RoteDirection(m_under_vec);
	}
	//左移動
	if (Inputter::Instance()->GetKey(Inputter::AKey))
	{
		Vector3 m_left_vec = Vector3(-player_info.m_camera_dir.z, 0.0f, player_info.m_camera_dir.x); //!左方向ベクトル
		Calculation::ThreeNormalization(m_left_vec);

		RoteDirection(m_left_vec);
	}
	//右移動
	if (Inputter::Instance()->GetKey(Inputter::DKey))
	{
		Vector3 m_right_vec = Vector3(player_info.m_camera_dir.z, 0.0f, -player_info.m_camera_dir.x); //!左方向ベクトル
		Calculation::ThreeNormalization(m_right_vec);

		RoteDirection(m_right_vec);
	}

	//動いているとき
	if (player_info.m_speed > StopSpeed)
	{
		//動かしたいAnimationのキー設定
		FbxController::Instance()->SetAnimationName(player_info.m_key, "Run");
	}
	else
	{
		FbxController::Instance()->SetAnimationName(player_info.m_key, "Stay");
	}

	Calculation::ThreeNormalization(player_info.m_direction);
	
	//移動
	player_info.m_pos += player_info.m_direction * player_info.m_speed;

	//アニメーション
	Animation();

	//カメラに移動量送信
	mp_camera->SetMoveVec(player_info.m_pos - player_info.m_old_pos);
	mp_camera->SetPlayerPos(player_info.m_pos);

}

//移動関数
void Player::RoteDirection(Vector3 m_end_vec_)
{
	//球面線形補間
	player_info.m_direction = Calculation::SphericalInterpolation(player_info.m_direction, m_end_vec_, PlayerRoteTime);

	//移動スピード設定
	player_info.m_speed = 1.5f;

	//モデル回転角度
	player_info.m_degree.y = Degree(atan2f(player_info.m_direction.x, player_info.m_direction.z));
}

//アニメーション関数
void Player::Animation()
{
	FbxController::Instance()->PlayAnimation(player_info.m_key);
}


