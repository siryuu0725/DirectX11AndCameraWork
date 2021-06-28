/**
 * @file Calculation.h
 * @brief よく使う計算式などをまとめたもの
 * @details 中身：計算機クラス
**/
#ifndef CALCULATION_H
#define CALCULATION_H

#include <vector>
#include <string>
#include "Utility.h"
#include <DirectXTex.h>

#define PI ((float) 3.141592654f) 

#define Radian( degree ) ((degree) * (PI / 180.0f))
#define Degree( radian ) ((radian) * (180.0f / PI))


/**
* 計算機クラス
*/

class Calculation
{
public:
	Calculation() {}
	~Calculation() {}

	/**
 　　* @brief  ワールド座標計算関数
	 * @param[in] pos_ オブジェクトの座標
	 * @param[in] scale_ オブジェクトのサイズ
	 * @param[in] angle_ オブジェクトの回転角度
	 * @return 対象のワールド座標を計算する
 　　*/
	static DirectX::XMMATRIX Matrix(Vector3 pos_, Vector3 scale_, Vector3 angle_);


	/**
 　　* @brief  ベクトルの距離算出関数
	 * @param[in] target_pos_x_  対象のベクトルのX座標
	 * @param[in] target_pos_y_  対象のベクトルのY座標
	 * @return ベクトルの長さを出す
 　　*/
	static float Length(Vector3 target_vec_);

	/**
 　　* @brief  ベクトルの正規化関数(3次元)
	 * @param[in] target_vec_  対象のベクトル
	 * @return 単位ベクトルを出す
 　　*/
	static void ThreeNormalization(Vector3& target_vec_);

	/**
　　* @brief  ベクトルの正規化関数(2次元)
	* @param[in] target_vec_  対象のベクトル
	* @return 単位ベクトルを出す
　　*/
	static void TwoNormalization(Vector2& target_vec_);

	/**
 　　* @brief  ベクトル回転関数
	 * @param[in] target_pos_  回転させるベクトル
	 * @param[in] target2_pos_  回転させる中心点(軸)
	 * @param[in] radian_  回転角度
	 * @return 引数1のから引数2までのベクトルを引数3度分回転させる
 　　*/
	static Vector3 Rotate(Vector3 target_pos_, Vector3 target2_pos_, float radian_);

	/**
 　　* @brief  ベクトル回転関数
	 * @param[in] target_pos_  回転させるベクトル
	 * @param[in] target2_pos_  回転させる中心点(軸)
	 * @param[in] radian_  回転角度
	 * @return 引数1のから引数2までのベクトルを引数3度分回転させる
 　　*/
	static Vector3 RodriguesRotation(Vector3 target_pos_, Vector3 target2_pos_,Vector3 zik_vec_, float radian_);

	/**
 　　* @brief  線形補間関数
	 * @param[in] target_pos_  回転させるベクトル
	 * @param[in] target2_pos_  回転させる中心点(軸)
	 * @param[in] radian_  回転角度
	 * @return 引数1のから引数2までのベクトルを引数3度分回転させる
 　　*/
	static Vector3 Lerp(Vector3 start_vec_, Vector3 end_vec_, float time_);

	/**
 　　* @brief なす角計算関数
	 * @param[in] vec_ 対象1のベクトル
	 * @param[in] vec2_ 対象2のベクトル
	 * @return 引数1と引数2に指定したベクトルのなす角を求める
 　　*/
	static float EggplantAngle(Vector3 vec_, Vector3 vec2_);

	/**
 　　* @brief 内積計算関数
	 * @param[in] vec_ 対象1のベクトル
	 * @param[in] vec2_ 対象2のベクトル
	 * @return 引数1と引数2に指定したベクトルの内積を求める
 　　*/
	static float Dot(Vector3 vec_, Vector3 vec2_);
	
	/**
 　　* @brief 外積計算関数
	 * @param[in] vec_ 対象1のベクトル
	 * @param[in] vec2_ 対象2のベクトル
	 * @return 引数1と引数2に指定したベクトルの外積を求める
 　　*/
	static float SecondCross(Vector3 vec_, Vector3 vec2_);


	static void Clamp(float& value_, float min_, float max_);
};





#endif

