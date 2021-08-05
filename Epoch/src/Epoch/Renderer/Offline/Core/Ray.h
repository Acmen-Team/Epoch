#pragma once
#include "Epoch/Math/Vec.h"

namespace Epoch {

  class Ray
  {
  public:
	Ray() {}
	Ray(const Point& origin, const Vec3& direction)
	  : m_origin(origin), m_direction(direction)
	{
	}

	~Ray() {}

	Point GetOrigin() const { return m_origin; }
	Vec3 GetDirection() const { return m_direction; }

	Point At(float time)
	{
	  return m_origin + time * m_direction;
	}

  private:
	Point m_origin;
	Vec3 m_direction;
  };

}

