/**
 * @file Utility.h
 * @brief 汎用性のある関数や構造体を保持する
 * @details 中身：汎用性のある構造体
**/
#ifndef UTILITY_H_
#define UTILITY_H_
#include <vector>
#include <string>

struct Vector3
{
	Vector3()
	{
		this->x = 0.0f;
		this->y = 0.0f;
		this->z = 0.0f;
	}

	Vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

    Vector3 operator + (const Vector3 dvec) const {
        Vector3 v3;

        v3.x = ((this)->x) + dvec.x;
        v3.y = ((this)->y) + dvec.y;
        v3.z = ((this)->z) + dvec.z;
        return v3;
    };

    Vector3 operator - (const Vector3 dvec) const {
        Vector3 v3;

        v3.x = ((this)->x) - dvec.x;
        v3.y = ((this)->y) - dvec.y;
        v3.z = ((this)->z) - dvec.z;
        return v3;
    };

    Vector3 operator * (float fDat) const {
        Vector3 v3;

        v3.x = ((this)->x) * fDat;
        v3.y = ((this)->y) * fDat;
        v3.z = ((this)->z) * fDat;

        return v3;
    };

    Vector3 operator / (float fDat) const {
        Vector3 v3;

        if (fDat == 0.0f) {
            return *this;
        }

        v3.x = ((this)->x) / fDat;
        v3.y = ((this)->y) / fDat;
        v3.z = ((this)->z) / fDat;
        return v3;
    };

    Vector3& operator += (const Vector3 dvec) {
        ((this)->x) += dvec.x;
        ((this)->y) += dvec.y;
        ((this)->z) += dvec.z;
        return *this;
    };

    Vector3& operator -= (const Vector3 dvec) {
        ((this)->x) -= dvec.x;
        ((this)->y) -= dvec.y;
        ((this)->z) -= dvec.z;
        return *this;
    };

    Vector3& operator *= (float fDat) {
        ((this)->x) *= fDat;
        ((this)->y) *= fDat;
        ((this)->z) *= fDat;
        return *this;
    };

    Vector3& operator /= (float fDat) {

        if (fDat == 0.0f) {
            return *this;
        }

        ((this)->x) /= fDat;
        ((this)->y) /= fDat;
        ((this)->z) /= fDat;
        return *this;
    };

	float x;
	float y;
	float z;
};

struct Vector2
{
	Vector2()
	{
		this->x = 0.0f;
		this->y = 0.0f;
	}

	Vector2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	float x;
	float y;
};

struct Quaternion
{
    Quaternion()
    {
        this->x = 0.0f;
        this->y = 0.0f;
        this->z = 0.0f;
        this->w = 0.0f;
    }

    Quaternion(float x, float y, float z, float w)
    {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    // 共役Quaternion
    Quaternion Conjugated()
    {
        return Quaternion(-x, -y, -z, w);
    };

    // Quaternion同士の積の計算
    Quaternion operator * (const Quaternion a_) const
    {
        Quaternion quaternion;
        quaternion.x = a_.x * this->w + a_.w * this->x - a_.z * this->y + a_.y * this->z;
        quaternion.y = a_.y * this->w + a_.z * this->x + a_.w * this->y - a_.x * this->z;
        quaternion.z = a_.z * this->w - a_.y * this->x + a_.x * this->y + a_.w * this->z;
        quaternion.w = a_.w * this->w - a_.x * this->x - a_.y * this->y - a_.z * this->z;

        return quaternion;
    }

    //QuaternionとVector3の積
    Vector3 operator * (const Vector3 vec_) const
    {
        Quaternion aQuaternion;

        aQuaternion.x = this->x;
        aQuaternion.y = this->y;
        aQuaternion.z = this->z;
        aQuaternion.w = this->w;

        // ベクトルをQuaternionに変換
        Quaternion bQuaternion = Quaternion(vec_.x, vec_.y, vec_.z, 1);
        // q * p * q^-1 でベクトルを回転
        Quaternion pos = aQuaternion * bQuaternion * aQuaternion.Conjugated();

        return Vector3(pos.x, pos.y, pos.z);
    }

   float x;
   float y;
   float z;
   float w;
};

struct Color
{
	float Red;
	float Green;
	float Blue;
	float Alpha;

	Color(float red, float green, float blue, float alpha)
	{
		Red = red;
		Green = green;
		Blue = blue;
		Alpha = alpha;
	}

	Color()
	{
		Red = Green = Blue = Alpha = 1.0f;
	}

};

void Split(char split_char, char* buffer, std::vector<std::string>& out);
void Replace(char search_char, char replace_char, char* buffer);

#endif