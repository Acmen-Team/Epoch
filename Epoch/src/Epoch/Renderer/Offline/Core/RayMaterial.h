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
	Metal(const Color& color, float fuzzier) 
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
	float fuzz;
  };

  class Dielectric : public RayMaterial
  {
  public:
	Dielectric(float index_of_refraction)
	  : ir(index_of_refraction)
	{
	}

	virtual bool Scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) override
	{
	  attenuation = Color(1.0f, 1.0f, 1.0f);
	  float refraction_ratio = rec.frontFace ? (1.0 / ir) : ir;

	  Vec3 unit_direction = normalize(rayIn.GetDirection());

	  float cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
	  float sin_theta = sqrt(1.0 - cos_theta * cos_theta);

	  bool cannot_refract = refraction_ratio * sin_theta > 1.0;
	  Vec3 direction;

	  if (cannot_refract || Reflectance(cos_theta, refraction_ratio) > random_float())
		direction = Reflect(unit_direction, rec.normal);
	  else
		direction = Refract(unit_direction, rec.normal, refraction_ratio);

	  scattered = Ray(rec.p, direction);
	  return true;
	}
  public:
	// Index of Refraction;
	float ir;
  private:
	static float Reflectance(float cosine, float ref_idx)
	{
	  // Use Schlick's approximation for reflectance.
	  auto r0 = (1 - ref_idx) / (1 + ref_idx);
	  r0 = r0 * r0;

	  return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
  };

}
