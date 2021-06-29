/**
 * @file Player.h
 * @brief プレイヤーオブジェクト
 * @details　中身：プレイヤークラス
**/
#ifndef PLAYER_H_
#define PLAYER_H_
#include "ObjectBase.h"
#include "Camera.h"

constexpr float PlayerRoteTime = 0.3f; //球面線形補間に使う媒介変数

class Player :public ObjectBase
{
public:
	struct PlayerInfo :public ObjectInfo
	{
		//!初期化
		PlayerInfo() :
			m_old_pos(Vector3(0.0f, 0.0f, 0.0f)),
			m_camera_dir(Vector3(0.0f, 0.0f, 0.0f)),
			m_speed(0.0f)
		{}

		Vector3 m_camera_dir;   //!カメラ方向ベクトル
	
		Vector3 m_old_pos; //!更新前座標
		float m_speed;     //!移動スピード

	};

private:
	PlayerInfo player_info;

public:

	Player(Camera* camera_) :mp_camera(camera_) {}
	~Player() {}

	 /**
	 * @brief  初期化関数
	 * @details オブジェクト情報初期化
	 */
	 virtual void Init()override;

	 /**
	 * @brief  更新関数
	 * @details 更新処理まとめ関数
	 */
	 virtual void Update()override;

	 /**
	 * @brief   描画関数
	 */
	 virtual void Draw()override;

	 /**
     * @brief   FBXモデル解放関数
     */
	 virtual void ReleaseModel()override;

	 /**
 　　* @brief  移動関数
 　　* @details プレイヤーの移動処理行う
 　　*/
	 void Move();

	 /**
 　　* @brief  移動関数
 　　* @details プレイヤーの移動処理行う
 　　*/
	 void RoteDirection(Vector3 m_end_vec_);

	 /**
 　　* @brief  移動関数
 　　* @details プレイヤーの移動処理行う
 　　*/
	 Vector3 SphericalInterpolation(Vector3 start_, Vector3 end_, float t_);

	 /**
　　 * @brief  アニメーション関数
　　 * @details プレイヤーのアニメーションを進める処理行う
　　 */
	 void Animation();

	 /**
     * @brief   影描関数
     */
	 virtual void ShadowDraw()override;

	 /**
　　 * @brief  オブジェクト情報Getter
	 * @param[out] copy_info_ 保存用オブジェクト構造体
	 * @details 引数にオブジェクトの情報を渡す
　　 */
	 const void GetPlayerInfo(PlayerInfo& copy_info_) { copy_info_ = player_info; }

private:
	Camera* mp_camera; //カメラ
};

#endif