// hit_record(構造体), Hittable(衝突判定), HittableList
#pragma once

#include "Ray.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

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

// レイを受け取って自身と交わるかを判定する関数hitを持つ
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

// hit()を使って複数の物体のどこが一番近いhitポイントであるかを判別する
class HittableList: public Hittable {
public:
  HittableList() {}
  HittableList(shared_ptr<Hittable> object) { add(object); }

  void  clear() { objects.clear(); }
  void  add(shared_ptr<Hittable> object) { objects.push_back(object); }

  virtual bool  hit(const Ray& r, double tmin, double tmax, hit_record& rec) const;

public:
  std::vector<shared_ptr<Hittable>> objects;
};

// 複数のオブジェクトに対してレイとの交差判定を行い、最も手前（カメラに最も近い）で交差したオブジェクトの情報を取得する関数
inline bool  HittableList::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
  hit_record  temp_rec;
  bool        hit_anything = false;
  double      closest_so_far = t_max;
  size_t      i = 0;

  while (i < objects.size()) {
    const shared_ptr<Hittable>& object = objects[i];
    if (object->hit(r, t_min, closest_so_far, temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
    ++i;
  }
  return hit_anything;
}
