/**
 * @file Collision.h
 * @brief 当たり判定を行う
 * @details 中身：当たり判定クラス
**/
#ifndef COLLISION_H_
#define COLLISION_H_
#include "../Utility.h"
#include "../Calculation.h"

/**
* 当たり判定クラス
*/

class Collision
{
public:
	Collision() {}
	~Collision() {}

	/**
　   * @brief  立方体と球の当たり判定
	 * @param[in] r_pos 立方体の座標
	 * @param[in] c_pos 球の座標
	 * @param[in] box_size_ 立方体のサイズ
	 * @param[in] radius_ 球の半径
	 * @param[in] rad_ 立方体の回転角度
　   */
	static bool RectAndSphere(Vector3 r_pos, Vector3 c_pos, Vector3 box_size_, float radius_, float rad_);

	/**
　   * @brief  球(BoundingSphere)と点の当たり判定
	 * @param[in] rect_pos 立方体の座標
	 * @param[in] point_pos_ 対象の座標
	 * @param[in] box_size_ 立方体のサイズ
	 * @details BoundingSphere = 立方体を囲む球
　   */
	static bool BoundingSphereAndPoint(Vector3 rect_pos, Vector3 point_pos_, Vector3 box_size_);

	/**
　   * @brief  円柱とレイの当たり判定
	 * @param[in] lay_start_ レイの始点
	 * @param[in] lay_end_ レイの終点
	 * @param[in] lay_dir_ レイの方向ベクトル
	 * @param[in] cilinder_pos_ 円柱軸の1点
	 * @param[in] height_ 円柱軸のもう1点
	 * @param[in] r 円柱の半径
	 * @param[out] p1 貫通開始点（戻り値）
	 * @param[out] p2 貫通終了点（戻り値）
　   */
	static bool CalcRayInfCilinder(Vector3 lay_start_, Vector3 lay_end_, Vector3 lay_dir_, Vector3 cilinder_pos_, float height_, float r, Vector3& q1, Vector3& q2);

	/**
　   * @brief  円と線分の当たり判定
	 * @param[in] lay_start_ 線分の始点
	 * @param[in] lay_end_ 線分の終点
	 * @param[in] cilinder_pos_ 円の座標
	 * @param[in] r 円の半径

　   */
	static bool VectorAndCircle(Vector3 lay_start_, Vector3 lay_end_, Vector3 cilinder_pos_, float r);

};



#endif