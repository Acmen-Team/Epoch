#include "eppch.h"

#include "Vec.h"

namespace Epoch {

  float Vec3::Length() const
  {
	return sqrt(LengthSquared());
  }

  float Vec3::LengthSquared() const
  {
	return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
  }

}
