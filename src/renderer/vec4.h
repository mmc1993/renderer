#pragma once

#include <cmath>

class Vec4 {
public:
	float x, y, z, w;

	Vec4(float _x, float _y, float _z, float _w)
	{
        x = _x; y = _y;
        z = _z; w = _w;
    }

    Vec4(float _x, float _y, float _z)
    {
        x = _x; y = _y;
        z = _z; w = 1.0f;
    }

    Vec4(const Vec4 & v)
    {
        *this = v;
    }

    Vec4()
    {
        x = y = z = 0; w = 1;
    }

    Vec4 & operator =(const Vec4 & v)
    {
        x = v.x; y = v.y;
        z = v.z; w = v.w;
        return *this;
    }

    Vec4 & operator +=(const Vec4 & v)
    {
        return (*this = *this + v);
    }

    Vec4 & operator -=(const Vec4 & v)
    {
        return (*this = *this - v); 
    }

	Vec4 operator +(const Vec4 & v) const
	{
		return { x + v.x, y + v.y, z + v.z };
	}

	Vec4 operator -(const Vec4 & v) const
	{
		return { x - v.x, y - v.y, z - v.z };
	}

	Vec4 operator *(const float v) const
	{
		return { x * v, y * v, z * v };
	}

    Vec4 operator *(const Vec4 & v) const
    {
        return { x * v.x, y * v.y, z * v.z };
    }

	Vec4 operator /(const float v) const
	{
		return { x / v, y / v, z / v };
	}

	float Dot(const Vec4 & v) const
	{
		return x * v.x + y * v.y + z * v.z;
	}

	Vec4 Cross(const Vec4 & v) const
	{
		return {	y * v.z - z * v.y,
					z * v.x - x * v.z,
					x * v.y - y * v.x	};
	}

	float LengthSqrt() const
	{
		return Dot(*this);
	}

	float Length() const
	{
		return std::sqrt(LengthSqrt());
	}

	const Vec4 & Normal()
	{
		return (*this = GetNormal());
	}

	Vec4 GetNormal() const
	{
		Vec4 ret;
		auto len = Length();
		if (len != 0)
		{
			auto v = 1.0f / len;
			ret.x = x * v;
			ret.y = y * v;
			ret.z = z * v;
		}
		return ret;
	}
};
