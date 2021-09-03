/**
 * @file ObjectBase.h
 * @brief 各オブジェクトの共通部分を扱う
**/

#ifndef OBJBASE_H_
#define OBJBASE_H_
#include "../Utility/Calculation.h"
#include "../System/Fbx/FbxController.h"
#include "../Utility/Utility.h"
#include <vector>
#include <string>
#include<memory>

/**
*@class   オブジェクトベースクラス
*/
class ObjectBase
{
public:
	//!オブジェクト情報
	struct ObjectInfo
	{
		ObjectInfo() :
			m_pos(Vector3(0.0f, 0.0f, 0.0f)),
			m_scale(Vector3(0.0f, 0.0f, 0.0f)),
			m_degree(Vector3(0.0f, 0.0f, 0.0f)),
			m_direction(Vector3(0.0f, 0.0f, 0.0f)),
			m_size(Vector3(0.0f, 0.0f, 0.0f)),
			m_collision_pos(Vector3(0.0f, 0.0f, 0.0f)),
			m_width(0.0f),
			m_height(0.0f),
			m_radius(0.0f),
			world_matrix(DirectX::XMMatrixIdentity())
		{
		}

		DirectX::XMMATRIX world_matrix;  //!ワールド行列
		Vector3 m_pos;     //!座標
		Vector3 m_degree;  //!角度
		Vector3 m_scale;   //!サイズ

		std::string m_key;  //!描画用キー

		Vector3 m_direction;     //!方向ベクトル
		Vector3 m_size;          //各軸のでのサイズ(縦幅、横幅、深さ)
		Vector3 m_collision_pos; //当たり判定用座標

		float m_width;    //!横幅
		float m_height;	  //!縦幅

		float m_radius;   //!半径
	};

	ObjectBase() {}
	virtual ~ObjectBase() {}

	/**
	 * @brief  初期化関数
	 * @details オブジェクト情報初期化
	 */
	virtual void Init() {}

    /**
	 * @brief  更新関数
	 * @details 更新処理まとめ関数
	 */
	virtual void Update() {}

	/**
	 * @brief   描画関数
	 */
	virtual void Draw() {}

	/**
     * @brief   影描画関数
     */
	virtual void ShadowDraw() {}
};

#endif
