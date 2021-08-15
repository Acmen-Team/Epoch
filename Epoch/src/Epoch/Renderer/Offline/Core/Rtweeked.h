#pragma once

#include <cmath>
#include <memory>
#include <limits>

// Constants

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.141592;

// Utility Functions

inline float degrees_to_radians(float degrees) {
  return degrees * pi / 180.0;
}

// Common Headers

#include "Ray.h"
#include "HitManager.h"