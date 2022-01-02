#pragma once
//#include "Epoch/Math/Vec.h"
#include "Ray.h"

namespace Epoch {

  class RayCamera
  {
  public:
	RayCamera(Point lookfrom, Point lookat, Vec3 vup, float vfov, float aspect_raio, float aperture,
	  float focus_dist)
	{
	  auto theta = degrees_to_radians(vfov);
	  auto h = tan(theta / 2);
	  auto viewport_height = 2.0 * h;
	  auto viewport_width = aspect_raio * viewport_height;

	  w = normalize(lookfrom - lookat);
	  u = normalize(Cross(vup, w));
	  v = Cross(w, u);

	  origin = lookfrom;
	  horizontal = focus_dist * viewport_width * u;
	  vertical = focus_dist * viewport_height * v;
	  lower_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

	  lens_radius = aperture / 2;
	}

	Ray get_ray(double s, double t) const {
	  Vec3 rd = lens_radius * random_in_unit_disk();
	  Vec3 offset = u * rd.x() + v * rd.y();

	  return Ray(
		origin + offset,
		lower_left_corner + s * horizontal + t * vertical - origin - offset
	  );
	}
  private:
	Point origin;
	Point lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	float lens_radius;
  };

}