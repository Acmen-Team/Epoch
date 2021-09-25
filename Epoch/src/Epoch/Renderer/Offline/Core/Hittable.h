#pragma once

#include "Ray.h"
#include "Rtweeked.h"

namespace Epoch {

  class RayMaterial;

  struct HitRecord
  {
	Point p;
	Vec3 normal;
	float time;
	//RayMaterial* material;
	std::shared_ptr<RayMaterial> material;
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