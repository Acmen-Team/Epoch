#pragma once

#include "Epoch/Renderer/Offline/Core/Rtweeked.h"

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

	inline static Vec3 Random()
	{
	  return Vec3(random_float(), random_float(), random_float());
	}

	inline static Vec3 Random(float min, float max)
	{
	  return Vec3(random_float(min, max), random_float(min, max), random_float(min, max));
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

	float Length() const
	{
	  return sqrt(LengthSquared());
	}

	float LengthSquared() const
	{
	  return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	bool NearZero() const
	{
	  const auto s = 1e-8;

	  return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
	}

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

  inline Vec3 Random_in_unit_sphere()
  {
	while (true)
	{
	  auto p = Vec3::Random(-1, 1);
	  if (p.LengthSquared() >= 1) continue;
	  return p;
	}
  }

  inline Vec3 Random_unit_vector()
  {
	return normalize(Random_in_unit_sphere());
  }

  inline Vec3 Reflect(const Vec3& v, const Vec3& n)
  {
	return v - 2 * dot(v, n) * n;
  }

  inline Vec3 Refract(const Vec3& uv, const Vec3& n, double etai_over_etat)
  {
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	Vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.LengthSquared())) * n;
	return r_out_perp + r_out_parallel;
  }

}