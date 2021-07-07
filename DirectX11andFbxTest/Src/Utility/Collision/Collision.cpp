#include "Collision.h"
#include <math.h>

//立方体と球の当たり判定
bool Collision::RectAndSphere(Vector3 rect_pos_, Vector3 sphere_pos_, Vector3 box_size_, float radius_, float rad_)
{
	Vector3 rot_pos(0.0f, 0.0f, 0.0f); //矩形の回転角度分円をずらした時の座標
	Vector3 ver_pos(0.0f, 0.0f, 0.0f);  //衝突時の頂点座標保存用

	Vector3 vec(0.0f, 0.0f, 0.0f); //矩形の頂点と円とのベクトル

	//衝突時、対象の回転角度分座標をずらす
	rot_pos = Calculation::Rotate(sphere_pos_, rect_pos_, rad_);

	//各軸を配列の要素としている [0]=x,[1]=y,[2]=z
	float sphere_pos[3];
	float rect_pos[3];
	float box_size[3];

	//球
	sphere_pos[0] = sphere_pos_.x;
	sphere_pos[1] = sphere_pos_.y;
	sphere_pos[2] = sphere_pos_.z;

	//立方体
	rect_pos[0] = rect_pos_.x;
	rect_pos[1] = rect_pos_.y;
	rect_pos[2] = rect_pos_.z;

	//立方体のサイズ
	box_size[0] = box_size_.x;
	box_size[1] = box_size_.y;
	box_size[2] = box_size_.z;

	float vec_length = 0.0f;

	//各軸から当たっているか見る
	for (int i = 0; i < 3; i++)
	{
		float point = sphere_pos[i];
		float box_min = rect_pos[i] + box_size[i] * -0.5f;
		float box_max = rect_pos[i] + box_size[i] * 0.5f;

		if (point < box_min) 
		{
			vec_length += (point - box_min) * (point - box_min);
		}
		if (point > box_max)
		{
			vec_length += (point - box_max) * (point - box_max);
		}
	}

	vec_length = (float)sqrt(vec_length);

	// 距離の二乗が0だったらCubeの内部にSphereの中心があるということ
	if (vec_length == 0.0f) 
	{
		return true;
	}
	//距離の二乗が球の半径より小さい
	else if (vec_length <= radius_)
	{
		return true;
	}

	return false;
}

//球(BoundingSphere)と点の当たり判定
bool Collision::BoundingSphereAndPoint(Vector3 rect_pos, Vector3 point_pos_, Vector3 box_size_)
{
	Vector3 boxsize = box_size_ / 2;

	//立方体をぴったり包む程のサイズ
	float b_sphere_radius = Calculation::Length(boxsize);

	float vec_length = Calculation::Length(point_pos_ - rect_pos);

	if (vec_length <= b_sphere_radius)
	{
		return true;
	}

	return false;
}

//円柱とレイの当たり判定
bool Collision::CalcRayInfCilinder(Vector3 lay_start_,Vector3 lay_end_,Vector3 lay_dir_,Vector3 cilinder_pos_,
	float height_,float r,
	Vector3& q1,Vector3& q2) 
{
	if (VectorAndCircle(lay_start_, lay_end_, cilinder_pos_, r) == false)
	{
		return false;
	}

	Vector3 cilinder_start = cilinder_pos_;
	cilinder_start.y += height_ / 2;

	Vector3 cilinder_end = cilinder_pos_;
	cilinder_end.y -= height_ / 2;

	Vector3 p = cilinder_start - lay_start_;  //レイの始点から円柱軸の1点までのベクトル
	cilinder_end = cilinder_end - lay_start_; //レイの始点から円柱軸のもう1点までのベクトル
	
	Vector3 s = cilinder_end - p; //円柱軸のベクトル

	// 各種内積値
	float Dvv = lay_dir_.x * lay_dir_.x + lay_dir_.y * lay_dir_.y + lay_dir_.z * lay_dir_.z; //レイの方向ベクトルの内積
	float Dsv = s.x * lay_dir_.x + s.y * lay_dir_.y + s.z * lay_dir_.z; //レイの方向ベクトルと円柱軸の内積
	float Dpv = p.x * lay_dir_.x + p.y * lay_dir_.y + p.z * lay_dir_.z; //レイの始点から円柱軸の1点までのベクトルの内積
	float Dss = s.x * s.x + s.y * s.y + s.z * s.z; //円柱軸の内積
	float Dps = p.x * s.x + p.y * s.y + p.z * s.z; //レイの始点から円柱軸のもう1点までのベクトルと円柱軸の内積
	float Dpp = p.x * p.x + p.y * p.y + p.z * p.z; //レイの始点から円柱軸の1点までのベクトルの内積
	float rr = r * r;

	if (Dss == 0.0f)
		return false; // 円柱が定義されない

	float A = Dvv - Dsv * Dsv / Dss;
	float B = Dpv - Dps * Dsv / Dss;
	float C = Dpp - Dps * Dps / Dss - rr;

	if (A == 0.0f)
		return false;

	float s_ = B * B - A * C;
	if (s_ < 0.0f)
		return false; // レイが円柱と衝突していない
	s_ = sqrtf(s_);

	float a1 = (B - s_) / A;
	float a2 = (B + s_) / A;

	q1 = lay_start_ + (lay_dir_ * a1);
	q2 = lay_start_ + (lay_dir_ * a2);

	return true;
}

//円と線分の当たり判定
bool Collision::VectorAndCircle(Vector3 lay_start_, Vector3 lay_end_, Vector3 cilinder_pos_, float r)
{
	// ベクトルの作成
	Vector2 start_to_center = Vector2(cilinder_pos_.x - lay_start_.x, cilinder_pos_.z - lay_start_.z);
	Vector2 end_to_center = Vector2(cilinder_pos_.x - lay_end_.x, cilinder_pos_.z - lay_end_.z);
	Vector2 start_to_end = Vector2(lay_end_.x - lay_start_.x, lay_end_.z - lay_start_.z);

	Vector2 nor_start_to_end = start_to_end;
	Calculation::TwoNormalization(nor_start_to_end);

	float cross = start_to_center.x * nor_start_to_end.y - nor_start_to_end.x * start_to_center.y;

	if (fabsf(cross) >= r)
	{
		return false;
	}

	// 始点 => 終点と始点 => 円の中心の内積を計算する
	float dot01 = start_to_center.x * start_to_end.x + start_to_center.y * start_to_end.y;
	// 始点 => 終点と終点 => 円の中心の内積を計算する
	float dot02 = end_to_center.x * start_to_end.x + end_to_center.y * start_to_end.y;

	// 二つの内積の掛け算結果が0以下なら当たり
	if (dot01 * dot02 <= 0.0f)
	{
		return true;
	}

	if (Calculation::Length(Vector3(start_to_center.x, start_to_center.y, 0.0f)) < r ||
		Calculation::Length(Vector3(end_to_center.x, end_to_center.y, 0.0f)) < r)
	{
		return true;
	}

	return false;
}
