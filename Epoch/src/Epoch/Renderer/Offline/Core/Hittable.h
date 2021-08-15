#pragma once

#include "Ray.h"

namespace Epoch {

  struct HitRecord
  {
	Point p;
	Vec3 normal;
	float time;
	bool frontFace;

	inline void SetFaceNormal(const Ray& ray, const Vec3& outwardNormal)
	{
	  frontFace = dot(ray.GetDirection(), outwardNormal) < 0;
	  normal = frontFace ? outwardNormal : -outwardNormal;
	}

  };

  class Hittable
  {
  public:
	virtual bool Hit(const Ray& ray, float timeMin, float timeMax, HitRecord& rec) const = 0;

  };

}