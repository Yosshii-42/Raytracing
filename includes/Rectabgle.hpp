#pragma once

#include "Hittable.hpp"
#include "Vec3.hpp"
#include "AABB.hpp"

class RectangleXY : public Hittable {
public:
  RectangleXY() {}
  RectangleXY(
    double _x0, double _x1, double _y0, double _y1, double _k,
    shared_ptr<Material> mat
  ) : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {}
  
  virtual bool  hit(const Ray& r, double t0, double t1, hit_record& rec) const;

  virtual bool  bounding_box(double t0, double t1, AABB& output_box) const {
    (void)t0;
    (void)t1;
    // AABBの辺の長さはゼロであってはならない　z方向に空くしだけ厚みを持たせる
    output_box = AABB(point3(x0, y0, k - 0.0001), point3(x1, y1,k + 0.0001));
    return true;
  }

public:
  double  x0, x1, y0, y1, k;
  shared_ptr<Material>  mp;
};

bool  RectangleXY::hit(const Ray& r, double t0, double t1, hit_record& rec) const {
  double  t = (k - r.origin().z()) / r.direction().z();
  if (t < t0 || t > t1)
    return false;
  double x = r.origin().x() + t * r.direction().x();
  double y = r.origin().y() + t * r.direction().y();
  if (x < x0 || x > x1 || y < y0 || y > y1)
    return false;
  rec.u = (x - x0) / (x1 - x0);
  rec.v = (y - y0) / (y1 - y0);
  rec.t = t;
  Vec3 outward_normal = Vec3(0, 0, 1);
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mp;
  rec.p = r.at(t);
  return true;
}

class RectangleYZ : public Hittable {
public:
  RectangleYZ() {}
  RectangleYZ(
    double _y0, double _y1, double _z0, double _z1, double _k,
    shared_ptr<Material> mat
  ) : y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat) {}
  
  virtual bool  hit(const Ray& r, double t0, double t1, hit_record& rec) const;

  virtual bool  bounding_box(double t0, double t1, AABB& output_box) const {
    (void)t0;
    (void)t1;
    // AABBの辺の長さはゼロであってはならない　z方向に空くしだけ厚みを持たせる
    output_box = AABB(point3(y0, z0, k - 0.0001), point3(y1, z1,k + 0.0001));
    return true;
  }

public:
  double  y0, y1, z0, z1, k;
  shared_ptr<Material>  mp;
};

bool  RectangleYZ::hit(const Ray& r, double t0, double t1, hit_record& rec) const {
  double  t = (k - r.origin().x()) / r.direction().x();
  if (t < t0 || t > t1)
    return false;
  double y = r.origin().y() + t * r.direction().y();
  double z = r.origin().z() + t * r.direction().z();
  if (y < y0 || y > y1 || z < z0 || z > z1)
    return false;
  rec.u = (y - y0) / (y1 - y0);
  rec.v = (z - z0) / (z1 - z0);
  rec.t = t;
  Vec3 outward_normal = Vec3(0, 0, 1);
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mp;
  rec.p = r.at(t);
  return true;
}


class RectangleZX : public Hittable {
public:
  RectangleZX() {}
  RectangleZX(
    double _z0, double _z1, double _x0, double _x1, double _k,
    shared_ptr<Material> mat
  ) : z0(_z0), z1(_z1), x0(_x0), x1(_x1), k(_k), mp(mat) {}
  
  virtual bool  hit(const Ray& r, double t0, double t1, hit_record& rec) const;

  virtual bool  bounding_box(double t0, double t1, AABB& output_box) const {
    (void)t0;
    (void)t1;
    // AABBの辺の長さはゼロであってはならない　z方向に空くしだけ厚みを持たせる
    output_box = AABB(point3(z0, x0, k - 0.0001), point3(z1, x1,k + 0.0001));
    return true;
  }

public:
  double  z0, z1, x0, x1, k;
  shared_ptr<Material>  mp;
};

bool  RectangleZX::hit(const Ray& r, double t0, double t1, hit_record& rec) const {
  double  t = (k - r.origin().y()) / r.direction().y();
  if (t < t0 || t > t1)
    return false;
  double z = r.origin().z() + t * r.direction().z();
  double x = r.origin().x() + t * r.direction().x();
  if (z < z0 || z > z1 || x < x0 || x > x1)
    return false;
  rec.u = (z - z0) / (z1 - z0);
  rec.v = (x - x0) / (x1 - x0);
  rec.t = t;
  Vec3 outward_normal = Vec3(0, 0, 1);
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mp;
  rec.p = r.at(t);
  return true;
}
