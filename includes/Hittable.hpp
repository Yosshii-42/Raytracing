#pragma once

#include "Ray.hpp"

class Material;

struct hit_record {
  point3  p;      // どこで当たったか(座標)
  Vec3    normal; // どの向き(法線)
  shared_ptr<Material> mat_ptr;
  double  t;      // どれくらい離れているか
  double  u;      // テクスチャ座標(u, v)
  double  v;
  bool front_face;

  inline void set_face_normal(const Ray& r, const Vec3& outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

// レイを受け取って自身と交わるかを判定する関数交交わるかを判定する関数hitを持つ
class Hittable {
public:
  virtual ~Hittable() {}
  virtual bool hit(
    const Ray& r,
    double t_min,
    double t_max,
    hit_record& rec
  ) const = 0;
};
