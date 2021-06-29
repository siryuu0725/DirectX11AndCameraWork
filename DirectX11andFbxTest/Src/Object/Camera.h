/**
 * @file Camera.h
 * @brief カメラオブジェクト
 * @details　中身：カメラクラス
**/
#ifndef CAMERA_H_
#define CAMERA_H_
#include "../System/DirectGraphics.h"
#include "ObjectBase.h"
#include "Block/BlockController.h"

constexpr __int8 AngleofView = 60;       //!画角
constexpr float Near = 1.1f;
constexpr float Far = 20000000.0f;
constexpr unsigned __int8 CameraSensitivity = 50;  //!カメラ感度
constexpr float CameraRoteTime = 0.2f; //球面線形補間に使う媒介変数
constexpr float MaxCameraRote_Y = 80.0f; //縦回転の限界値
constexpr float MinCameraPos_Y = 0.0f;   //カメラが地面から押し出される位置
constexpr float CameraUpPos = 1.0f;   //地面の裏側が見えないように余分に押し出す値

class Camera :public ObjectBase
{
public:
	//!カメラ情報
	struct CameraInfo :public ObjectInfo
	{
		CameraInfo() :
			m_eye_vec(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
			m_camera_up_vec(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
			m_forward_vec(DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f)),
			mat_view(DirectX::XMMatrixIdentity()),
			m_camera_up(Vector3(0.0f, 0.0f, 0.0f)),
			m_forward(Vector3(0.0f, 0.0f, 0.0f)),
			m_move_vec(Vector3(0.0f, 0.0f, 0.0f)),
			m_player_pos(Vector3(0.0f, 0.0f, 0.0f)),
			m_zik_vec(Vector3(0.0f, 0.0f, 0.0f)),
			m_intersection(Vector3(0.0f, 0.0f, 0.0f)),
			m_p1(Vector3(0.0f, 0.0f, 0.0f)),
			m_p2(Vector3(0.0f, 0.0f, 0.0f)),
			leap_vec(Vector3(0.0f, 0.0f, 0.0f)),
			m_yaw(0.0f),
			m_rote_speed(0.0f),
			m_rote_angle(0.0f)
		{
		}
		DirectX::XMMATRIX world_matrix;  //!ワールド行列

		DirectX::XMVECTOR m_eye_vec;       //!注視点
		DirectX::XMVECTOR m_camera_up_vec; //!カメラの上向きのベクトル
		DirectX::XMVECTOR m_forward_vec;   //!カメラの前向きのベクトル
		DirectX::XMMATRIX mat_view;        //!カメラのビュー情報を保存する変数

		//値を変更しやすいようにVector型で用意する
		Vector3 m_camera_up; //!カメラの上向きのベクトル
		Vector3 m_forward;   //!カメラの前向きのベクトル

		Vector3 m_move_vec;   //!移動量
		Vector3 m_player_pos; //!プレイヤー座標(カメラの回転の軸にする)
		Vector3 m_zik_vec; //!プレイヤー座標(カメラの回転の軸にする)
		Vector3 m_intersection; //!地面とカメラベクトルの交点

		Vector3 m_p1; //円柱との交点
		Vector3 m_p2; //円柱との交点2

		Vector3 leap_vec; //!線形補間ベクトル

		float m_yaw;        //!X軸回転
		float m_rote_speed; //回転スピード

		float m_rote_angle; //現在の回転角度(上下)

	};

private:
	CameraInfo m_camerainfo;

public:
	Camera(BlockController* block_) :m_block(block_) {}
	~Camera() {}

	/**
	* @brief  初期化関数
	* @details オブジェクト情報初期化
	*/
	virtual void Init()override;

	/**
	* @brief  視錐台作成関数
	* @details 視錐台作成
	*/
	void CreateFrustum();

	/**
	* @brief  更新関数
	* @param[in] player_pos_ プレイヤー座標
	* @details 更新処理まとめ関数
	*/
	virtual void Update()override;

	/**
　　* @brief  移動関数
　　* @details プレイヤーの移動処理行う
　　*/
	void Move();

	/**
 　 * @brief 矩形ブロック当たり判定関数
 　 * @details ステージにある矩形ブロックとカメラのレイとの当たり判定を行う
 　 */
	void HitRectBlock();

	/**
 　 * @brief 円形ブロック当たり判定関数
 　 * @details ステージにある円形ブロックとカメラのレイとの当たり判定を行う
 　 */
	void HitCircleBlock();

	/**
	* @brief  カメラ回転関数
	* @details カメラの回転処理関数
	*/
	void RotEyePos();

	/**
	* @brief  めり込み差分算出関数
	* @details カメラが床や壁に埋まらないように差分を計算する
	*/
	void Difference();

	/**
	* @brief  カメラ移動量Set関数
	* @param[in] move_vec_ 移動量
	* @details カメラの移動量を代入する(Playerから受け取る)
	*/
	void SetMoveVec(Vector3 move_vec_) { m_camerainfo.m_move_vec = move_vec_; }

	/**
	* @brief  プレイヤー座標取得関数
	* @param[in] player_pos_ プレイヤー座標
	* @details 相互参照にならないようにSetterを用意する
	*/
	void SetPlayerPos(Vector3 player_pos_) { m_camerainfo.m_player_pos = player_pos_; }

	/**
　　* @brief  オブジェクト情報Getter
	* @param[out] copy_info_ 保存用オブジェクト構造体
	* @details 引数にオブジェクトの情報を渡す
　　*/
	const void GetCameraInfo(CameraInfo& copy_info_) { copy_info_ = m_camerainfo; }

private:
	BlockController* m_block;
};

#endif