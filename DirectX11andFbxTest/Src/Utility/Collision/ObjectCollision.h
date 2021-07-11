/**
 * @file ObjectCollision.h
 * @brief オブジェクト同士の当たり判定を管理する(第三者)
 * @details 中身：当たり判定クラス
**/
#ifndef OBJECTCOLLISION_H_
#define OBJECTCOLLISION_H_
#include "Collision.h"
#include "../Utility.h"
#include "../Calculation.h"
#include "../../Object/ObjectBase.h"

/**
* オブジェクト当たり判定管理
*/

class ObjectCollision
{
public:
	ObjectCollision() :m_hit_circleblock_id(0), m_hit_rectblock_id(0) {}
	~ObjectCollision() {}

	static ObjectCollision* Instance();

	/**
 　　* @brief 矩形ブロック当たり判定関数
 　　* @details ステージにある矩形ブロックとプレイヤーとの当たり判定を行う
 　　*/
	bool HitRectBlock();

	/**
 　　* @brief 円形ブロック当たり判定関数
 　　* @details ステージにある円形ブロックとプレイヤーとの当たり判定を行う
 　　*/
	bool HitCircleBlock();

#pragma region オブジェクト情報Setter

	/**
　　* @brief  カメラ情報Set関数
	* @param[in] obj_info_ カメラオリジナル情報構造体
　　* @details 当たり判定用クラスのカメラ情報構造体にオリジナルの情報をコピーする
　　*/
	const void SetCameraInfo(ObjectBase::ObjectInfo obj_info_) { m_camera_copy = obj_info_; }

	/**
　　* @brief  矩形ブロック情報Set関数
	* @param[in] obj_info_ 矩形ブロックオリジナル情報構造体
　　* @details 当たり判定用クラスの矩形ブロック情報構造体にオリジナルの情報をコピーする
　　*/
	const void SetRectBlockInfo(ObjectBase::ObjectInfo obj_info_) { m_rectblock_copy.push_back(obj_info_); }

	/**
　　* @brief  円形ブロック情報Set関数
	* @param[in] obj_info_ 円形ブロックオリジナル情報構造体
　　* @details 当たり判定用クラスの円形ブロック情報構造体にオリジナルの情報をコピーする
　　*/
	const void SetCircleBlockInfo(ObjectBase::ObjectInfo obj_info_) { m_circleblock_copy.push_back(obj_info_); }

#pragma endregion 

	/**
　　* @brief 衝突矩形IDGetter関数
　　* @details 衝突した矩形ブロックのIDを返す
　　*/
	const __int8 GetRectBlockID() { return m_hit_rectblock_id; }

	/**
　　* @brief 衝突円形IDGetter関数
　　* @details 衝突した円形ブロックのIDを返す
　　*/
	const __int8 GetCircleBlockID() { return m_hit_circleblock_id; }

	/**
　　* @brief  オブジェクト情報Getter
	* @param[out] copy_info_ 保存用オブジェクト構造体
	* @details 引数にオブジェクトの情報を渡す
　　*/
	const void GetCameraInfo(ObjectBase::ObjectInfo& copy_info_) { copy_info_ = m_camera_copy; }

	/**
	*	@brief Instance解放関数(Debug用)
	*	@details 手動でインスタンスを解放する
	*/
	void ReleaseInstance();

private:
	static ObjectCollision* mp_instance;
	 
	std::vector<ObjectBase::ObjectInfo> m_rectblock_copy;    //!矩形ブロック情報保存用
	std::vector<ObjectBase::ObjectInfo> m_circleblock_copy;  //!円形ブロック情報保存用
	ObjectBase::ObjectInfo m_camera_copy;                    //!円形ブロック情報保存用

	__int8 m_hit_circleblock_id;  //!衝突した円形ブロックのID保存用
	__int8 m_hit_rectblock_id;	  //!衝突した矩形ブロックのID保存用

};



#endif