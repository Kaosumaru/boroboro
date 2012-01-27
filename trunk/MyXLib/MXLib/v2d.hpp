#pragma once

#include <math.h>
//#include <ostream>

struct v2d
{
	float x, y;
	explicit v2d(float x=0, float y=0) : x(x), y(y) {}
	v2d(const v2d& v) : x(v.x), y(v.y) {}
	v2d operator+(const v2d& v) const
	{
		return v2d(x+v.x, y+v.y);
	}
	v2d operator-(const v2d& v) const
	{
		return v2d(x-v.x, y-v.y);
	}
	v2d operator*(float s) const 
	{
		return v2d(x*s, y*s);
	}
};

inline float dot(const v2d& a, const v2d& b)
{
	return a.x*b.x+a.y*b.y;
}

inline float norm(const v2d& v)
{
	return dot(v, v);
}

inline float length(const v2d& v)
{
	return sqrt(norm(v));
}

inline v2d dirVec(float a)
{
	return v2d(cos(a), sin(a)); //v2d(sin(a), cos(a));
}

inline v2d normalized(const v2d& v)
{
   return v*float(1.0f/length(v));
}

//inline std::wostream