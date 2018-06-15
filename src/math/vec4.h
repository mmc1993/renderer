#pragma once

#include <cmath>

class Vec4 {
public:
	float x, y, z, w;

	Vec4(float _x = 0, float _y = 0, float _z = 0, float _w = 1) : x(_x), y(_y), z(_z), w(_w)
	{ }

    Vec4 & operator+= (const Vec4 & val)
    {
        return (*this = *this + val);
    }

    Vec4 & operator-= (const Vec4 & val)
    {
        return (*this = *this - val); 
    }

	Vec4 operator+ (const Vec4 & val) const
	{
		return { x + val.x, y + val.y, z + val.z };
	}

	Vec4 operator- (const Vec4 & val) const
	{
		return { x - val.x, y - val.y, z - val.z };
	}

	Vec4 operator* (const float val) const
	{
		return { x * val, y * val, z * val };
	}

    Vec4 operator* (const Vec4 & val) const
    {
        return { x * val.x, y * val.y, z * val.z, w * val.w };
    }

	Vec4 operator/ (const float val) const
	{
		return { x / val, y / val, z / val };
	}

	float Dot(const Vec4 & val) const
	{
		return { x * val.x + y * val.y + z * val.z };
	}

	Vec4 Cross(const Vec4 & val) const
	{
		return {	y * val.z - z * val.y,
					z * val.x - x * val.z,
					x * val.y - y * val.x	};
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
