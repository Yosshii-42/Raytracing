#pragma once

#include "Vec3.hpp"

class AABB {
public:
  AABB(){}
  AABB(const point3& a, const point3& b) : minimum(a), maximum(b) {}

  bool  hit(const Ray& r, double t_min, double t_max) const;

  point3  min() const { return minimum; }
  point3  max() const { return maximum; }

public:
  point3  minimum;
  point3  maximum;
};
