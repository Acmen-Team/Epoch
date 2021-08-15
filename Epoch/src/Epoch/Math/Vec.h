#pragma once

namespace Epoch {

  class Vec3
  {
  public:
	Vec3() : e{ 0, 0, 0 }
	{
	}

	Vec3(float x, float y, float z) : e{ x, y, z }
	{
	}

	float x() const { return e[0]; }
	float y() const { return e[1]; }
	float z() const { return e[2]; }

	Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
	float operator[](int i) const { return e[i]; }
	float& operator[](int i) { return e[i]; }

	Vec3& operator+=(const Vec3& v)
	{
	  e[0] += v.e[0];
	  e[1] += v.e[1];
	  e[2] += v.e[2];

	  return *this;
	}

	Vec3& operator*=(const float t)
	{
	  e[0] *= t;
	  e[1] *= t;
	  e[2] *= t;

	  return *this;
	}

	Vec3& operator/=(const float t)
	{
	  return *this *= 1 / t;
	}

	float Length() const;

	float LengthSquared() const;

  public:
	float e[3];
  };

  using Point = Vec3;
  using Color = Vec3;

  inline Vec3 operator+(const Vec3& u, const Vec3& v)
  {
	return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
  }

  inline Vec3 operator-(const Vec3& u, const Vec3& v)
  {
	return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
  }

  inline Vec3 operator*(const Vec3& u, const Vec3& v)
  {
	return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
  }

  inline Vec3 operator*(float t, const Vec3& v)
  {
	return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
  }

  inline Vec3 operator*(const Vec3& v, float t)
  {
	return t * v;
  }

  inline Vec3 operator/(Vec3 v, float t)
  {
	return (1 / t) * v;
  }

  inline float dot(const Vec3& u, const Vec3& v)
  {
	return u.e[0] * v.e[0]
	  + u.e[1] * v.e[1]
	  + u.e[2] * v.e[2];
  }

  inline Vec3 normalize(Vec3 v)
  {
	return v / v.Length();
  }

}