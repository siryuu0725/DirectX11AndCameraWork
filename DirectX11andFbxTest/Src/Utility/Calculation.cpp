#include "Calculation.h"
#include <math.h>
#include <sstream>
#include <iomanip>
#include <random>


//ワールド座標計算関数
DirectX::XMMATRIX Calculation::Matrix(Vector3 pos_, Vector3 scale_, Vector3 angle_)
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX mat_trans, mat_scale, mat_rot, mat_rot_x, mat_rot_y, mat_rot_z;

	world = DirectX::XMMatrixIdentity();

	mat_trans = DirectX::XMMatrixTranslation(pos_.x, pos_.y, pos_.z);
	mat_rot_x = DirectX::XMMatrixRotationX(Radian(angle_.x));
	mat_rot_y = DirectX::XMMatrixRotationY(Radian(angle_.y));
	mat_rot_z = DirectX::XMMatrixRotationZ(Radian(angle_.z));
	mat_scale = DirectX::XMMatrixScaling(scale_.x, scale_.y, scale_.z);

	mat_rot = mat_rot_x * mat_rot_y * mat_rot_z;

	world *= mat_scale * mat_rot * mat_trans;

	return world;
}

//なす角計算関数
float Calculation::EggplantAngle(Vector3 direction_, Vector3 vec_)
{
	//方向ベクトルと接線に垂直なベクトルのなす角を求める
	Calculation::ThreeNormalization(direction_);
	Calculation::ThreeNormalization(vec_);
	float a = (vec_.x * direction_.x) + (vec_.z * direction_.z);
	Clamp(a, -1.0f, 1.0f);
	return acosf(a);
}

//内積計算関数
float Calculation::Dot(Vector3 vec_, Vector3 vec2_)
{
	return (vec_.x * vec2_.x) + (vec_.y * vec2_.y) + (vec_.z * vec2_.z);
}

//外積計算関数
float Calculation::SecondCross(Vector3 vec_, Vector3 vec2_)
{
	return (vec_.x * vec2_.z) - (vec2_.x * vec_.z);
}

void Calculation::Clamp(float& value_, float min_, float max_)
{
	if (value_ < min_)
	{
		value_ = min_;
	}
	else if (value_ > max_)
	{
		value_ = max_;
	}
}

//ベクトルの距離算出関数
float Calculation::Length(Vector3 target_vec_)
{
	return sqrtf(target_vec_.x * target_vec_.x + target_vec_.y * target_vec_.y + target_vec_.z * target_vec_.z);
}

// ベクトルの正規化関数
void Calculation::ThreeNormalization(Vector3& target_vec_)
{
	target_vec_ = Vector3(target_vec_.x / Length(target_vec_), target_vec_.y / Length(target_vec_), target_vec_.z / Length(target_vec_));
}

// ベクトルの正規化関数
void Calculation::TwoNormalization(Vector2& target_vec_)
{
	target_vec_ = Vector2(target_vec_.x / Length(Vector3(target_vec_.x, target_vec_.y, 0.0f)) , target_vec_.y / Length(Vector3(target_vec_.x, target_vec_.y, 0.0f)));
}

//ベクトル回転関数
Vector3 Calculation::Rotate(Vector3 target_pos_, Vector3 target2_pos_, float radian_)
{
	Vector3 oldpos = target_pos_;
	Vector3 vec = oldpos - target2_pos_;


	target_pos_.x = vec.x * cos(radian_) - vec.z * sin(radian_) + target2_pos_.x;
	target_pos_.z = vec.x * sin(radian_) + vec.z * cos(radian_) + target2_pos_.z;

	return target_pos_;
}

Vector3 Calculation::RodriguesRotation(Vector3 target_pos_, Vector3 target2_pos_,Vector3 zik_vec_, float radian_)
{
	Vector3 oldpos = target_pos_;
	Vector3 vec = oldpos - target2_pos_;

	target_pos_.x = vec.x * (powf(zik_vec_.x, 2) * (1 - cos(radian_)) + cos(radian_)) + vec.y * (zik_vec_.x * zik_vec_.y * (1 - cos(radian_)) - zik_vec_.z * sin(radian_)) + vec.z * (zik_vec_.z * zik_vec_.x * (1 - cos(radian_) + zik_vec_.y * sin(radian_)));
	target_pos_.y = vec.x * (zik_vec_.x * zik_vec_.y * (1 - cos(radian_)) + zik_vec_.z * sin(radian_)) + vec.y * (powf(zik_vec_.y, 2) * (1 - cos(radian_)) + cos(radian_)) + vec.z * (zik_vec_.y * zik_vec_.z * (1 - cos(radian_)) - zik_vec_.x * sin(radian_));
	target_pos_.z = vec.x * (zik_vec_.z * zik_vec_.x * (1 - cos(radian_)) - zik_vec_.y * sin(radian_)) + vec.y * (zik_vec_.y * zik_vec_.z * (1 - cos(radian_)) + zik_vec_.x * sin(radian_)) + vec.z * (powf(zik_vec_.z, 2) * (1 - cos(radian_)) + cos(radian_));

	target_pos_ += target2_pos_;

	return target_pos_;
}

//線形補間関数
Vector3 Calculation::Lerp(Vector3 start_vec_, Vector3 end_vec_, float time_)
{
	Vector3 vec = Vector3(0.0f, 0.0f, 0.0f);

	vec.x = start_vec_.x + time_ * (end_vec_.x - start_vec_.x);
	vec.y = start_vec_.y + time_ * (end_vec_.y - start_vec_.y);
	vec.z = start_vec_.z + time_ * (end_vec_.z - start_vec_.z);

	return vec;
}

//球面線形補間関数
Vector3 Calculation::SphericalInterpolation(Vector3 start_, Vector3 end_, float time_)
{
	Vector3 s, e, out;
	s = start_;
	e = end_;

	const float max_angle = 180.0; //球面線形補間で使用する最大角

	// 2ベクトル間の角度（鋭角側）
	float angle = EggplantAngle(s, e);

	//補間する必要が無い時
	if (Degree(angle) == 0.0f)
	{
		return end_;
	}
	//180度補間させる場合
	else if (Degree(angle) == max_angle)
	{

		std::random_device rnd;
		const float add_rote_angle = 0.01f; //少し傾ける角度

		//左右のどちらに回転するかを決める
		if (rnd() % 2 == 0)
		{
			e.x += add_rote_angle;
			e.z += add_rote_angle;
		}
		else
		{
			e.x -= add_rote_angle;
			e.z -= add_rote_angle;
		}
	}

	// sinθ
	float SinTh = sin(angle);

	// 補間係数
	float Ps = sin(angle * (1 - time_));
	float Pe = sin(angle * time_);

	out = (s * Ps + e * Pe) / SinTh;

	// 一応正規化して球面線形補間に
	Calculation::ThreeNormalization(out);

	return out;
}

