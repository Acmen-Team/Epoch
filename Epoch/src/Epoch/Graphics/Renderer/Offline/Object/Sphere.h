#pragma once

#include "Epoch/Graphics/Renderer/Offline/Core/Hittable.h"
#include "Epoch/Core/Math/Vec.h"

namespace Epoch {

  class Sphere : public Hittable
  {
  public:
	Sphere() {}
	Sphere(Point center, float r, std::shared_ptr<RayMaterial> material)
	  : m_Center(center), m_Radius(r), m_Material(material)
	{
	}

	virtual bool Hit(const Ray& ray, float timeMin, float timeMax, HitRecord& rec) const override;

  public:
	Point m_Center;
	float m_Radius;
	std::shared_ptr<RayMaterial> m_Material;
  };

  bool Sphere::Hit(const Ray& ray, float timeMin, float timeMax, HitRecord& rec) const
  {
	Vec3 oc = ray.GetOrigin() - m_Center;
	auto a = ray.GetDirection().LengthSquared();
	auto half_b = dot(ray.GetDirection(), oc);
	auto c = oc.LengthSquared() - m_Radius * m_Radius;
	auto discriminant = (half_b * half_b) - (a * c);

	if (discriminant < 0) return false;

	auto sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	auto root = (-half_b - sqrtd) / a;
	if (root < timeMin || root > timeMax)
	{
	  root = (-half_b + sqrtd) / a;
	  if (root < timeMin || root > timeMax)
		return false;
	}

	rec.time = root;
	rec.p = ray.At(rec.time);
	Vec3 outwardNormal = (rec.p - m_Center) / m_Radius;
	rec.SetFaceNormal(ray, outwardNormal);
	rec.material = m_Material;

	return true;
  }

}