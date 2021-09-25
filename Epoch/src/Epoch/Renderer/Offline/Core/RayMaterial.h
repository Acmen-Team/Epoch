#pragma once

#include "Ray.h"
#include "Epoch/Math/Vec.h"

namespace Epoch {

  struct HitRecord;

  class RayMaterial
  {
  public:
	virtual bool Scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) = 0;
  };

  class Lambertian : public RayMaterial
  {
  public:
	Lambertian(const Color& color)
	  : albedo(color)
	{
	}

	virtual bool Scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) override
	{
	  auto scatter_direction = rec.normal + Random_unit_vector();

	  if (scatter_direction.NearZero())
		scatter_direction = rec.normal;

	  scattered = Ray(rec.p, scatter_direction);
	  attenuation = albedo;
	  return true;
	}
  private:
	Color albedo;
  };

  class Metal : public RayMaterial
  {
  public:
	Metal(const Color& color, double fuzzier) 
	  : albedo(color), fuzz(fuzzier < 1 ? fuzzier : 1)
	{
	}

	virtual bool Scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) override
	{
	  Vec3 reflected = Reflect(normalize(rayIn.GetDirection()), rec.normal);
	  scattered = Ray(rec.p, reflected+ fuzz * Random_in_unit_sphere());
	  attenuation = albedo;
	  return (dot(scattered.GetDirection(), rec.normal) > 0);
	}

private:
	Color albedo;
	double fuzz;
  };

}
