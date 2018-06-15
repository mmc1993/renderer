#pragma once

#include "vec4.h"

#include <cmath>
#include <cstring>

class Matrix4x4 {
public:
	float m[4][4];

	Matrix4x4()
	{ }

	void Zero()
	{
		std::memset(&m, 0, sizeof(float) * 16);
	}

    const Matrix4x4 & Identity()
    {
        Zero();
        m[0][0] = 1.0f;
        m[1][1] = 1.0f;
        m[2][2] = 1.0f;
        m[3][3] = 1.0f;
        return *this;
    }

	Matrix4x4 operator* (const Matrix4x4 & val)
	{
		Matrix4x4 mat;
		for (auto y = 0; y != 4; ++y)
		{
			for (auto x = 0; x != 4; ++x)
			{
				mat.m[x][y] 
					= m[0][y] * val.m[x][0]
					+ m[1][y] * val.m[x][1]
					+ m[2][y] * val.m[x][2]
					+ m[3][y] * val.m[x][3];
			}
		}
		return mat;
	}

	static Matrix4x4 NewIdentity()
	{
		Matrix4x4 mat;
        return mat.Identity();
	}

	static Matrix4x4 NewTranslate(float x, float y, float z)
	{
		Matrix4x4 mat;
		mat.Identity();
		mat.m[3][0] = x;
		mat.m[3][1] = y;
		mat.m[3][2] = z;
		return mat;
	}

	static Matrix4x4 NewRotate(float x, float y, float z, float radian)
	{
		auto cos0 = std::cos(radian);
		auto sin0 = std::sin(radian);
		Matrix4x4 mat;
		mat.Identity();
		mat.m[0][0] = x * x * (1 - cos0) + cos0;
		mat.m[0][1] = x * y * (1 - cos0) + z * sin0;
		mat.m[0][2] = x * y * (1 - cos0) - y * sin0;

		mat.m[1][0] = x * y * (1 - cos0) - z * sin0;
		mat.m[1][1] = y * y * (1 - cos0) + cos0;
		mat.m[1][2] = y * z * (1 - cos0) + x * sin0;

		mat.m[2][0] = x * z * (1 - cos0) + y * sin0;
		mat.m[2][1] = y * z * (1 - cos0) - x * sin0;
		mat.m[2][2] = z * z * (1 - cos0) + cos0;
		return mat;
	}

	static Matrix4x4 NewScale(float x, float y, float z)
	{
		Matrix4x4 mat;
		mat.Identity();
		mat.m[0][0] = x;
		mat.m[1][1] = x;
		mat.m[2][2] = x;
		return mat;
	}
};

inline Vec4 operator* (const Vec4 & vec, const Matrix4x4 & mat)
{
    return Vec4(
		vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + vec.w * mat.m[3][0],
		vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + vec.w * mat.m[3][1],
		vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + vec.w * mat.m[3][2],
        vec.x * mat.m[0][3] + vec.y * mat.m[1][3] + vec.z * mat.m[2][3] + vec.w * mat.m[3][3]
	);
}

inline Vec4 & operator*= (Vec4 & vec, const Matrix4x4 & mat)
{
    return (vec = vec * mat);
}
