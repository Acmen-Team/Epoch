#pragma once

#include <cmath>
#include <memory>
#include <limits>
#include <cstdlib>

// Constants

const float infinity = std::numeric_limits<float>::infinity();
const float pi = 3.141592;

// Utility Functions

inline float degrees_to_radians(float degrees) {
  return degrees * pi / 180.0;
}

inline float clamp(float x, float min, float max) {
  if (x < min) return min;
  if (x > max) return max;
  return x;
}

inline float random_float()
{
  return rand() / (RAND_MAX + 1.0);
}

inline float random_float(float min, float max)
{
  return min + (max - min) * random_float();
}

