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
		std::memset(&m, 0, sizeof(m));
	}

	Matrix4x4 operator *(const Matrix4x4 & v) const
	{
		Matrix4x4 mat;
		for (auto y = 0; y != 4; ++y)
		{
			for (auto x = 0; x != 4; ++x)
			{
				mat.m[x][y] 
					= m[0][y] * v.m[x][0]
					+ m[1][y] * v.m[x][1]
					+ m[2][y] * v.m[x][2]
					+ m[3][y] * v.m[x][3];
			}
		}
		return mat;
	}

    Matrix4x4 & Identity()
    {
        Zero();
        m[0][0] = 1.0f;
        m[1][1] = 1.0f;
        m[2][2] = 1.0f;
        m[3][3] = 1.0f;
        return *this;
    }

	Matrix4x4 & Rotate(float x, float y, float z, float radian)
	{
		auto cos0 = std::cos(radian);
		auto sin0 = std::sin(radian);
		m[0][0] = x * x * (1 - cos0) + cos0;
		m[0][1] = x * y * (1 - cos0) + z * sin0;
		m[0][2] = x * y * (1 - cos0) - y * sin0;

		m[1][0] = x * y * (1 - cos0) - z * sin0;
		m[1][1] = y * y * (1 - cos0) + cos0;
		m[1][2] = y * z * (1 - cos0) + x * sin0;

		m[2][0] = x * z * (1 - cos0) + y * sin0;
		m[2][1] = y * z * (1 - cos0) - x * sin0;
		m[2][2] = z * z * (1 - cos0) + cos0;
		return *this;
	}

    Matrix4x4 & Translate(float x, float y, float z)
    {
        m[3][0] = x;
        m[3][1] = y;
        m[3][2] = z;
        return *this;
    }

	Matrix4x4 & Scale(float x, float y, float z)
	{
		m[0][0] = x;
		m[1][1] = x;
		m[2][2] = x;
		return *this;
	}
};

inline Vec4 operator *(const Vec4 & v, const Matrix4x4 & m)
{
    return Vec4(
		v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + v.w * m.m[3][0],
		v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + v.w * m.m[3][1],
		v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + v.w * m.m[3][2],
        v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + v.w * m.m[3][3]
	);
}

inline Vec4 & operator *=(Vec4 & v, const Matrix4x4 & m)
{
    return (v = v * m);
}
