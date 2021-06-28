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
public:

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