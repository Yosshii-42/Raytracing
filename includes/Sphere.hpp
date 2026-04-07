#pragma once

#include "Hittable.hpp"
#include "Vec3.hpp"
#include "Utils.hpp"

void  get_sphere_uv(const Vec3& p, double& u, double& v);

class Sphere: public Hittable {
public:
  Sphere() {}
  Sphere(point3 cen, double r, shared_ptr<Material> m)
    : center(cen), radius(r), mat_ptr(m) {}

  virtual bool hit(
    const Ray& r,
    double tmin,
    double tmax,
    hit_record& rec
  ) const;

public:
  point3  center; // 球の中心
  double  radius; // 半径
  shared_ptr<Material>  mat_ptr; // 球の持つマテリアル(mainで初期化)が代入される
};

bool  Sphere::hit(const Ray& r, double t_min, double t_max, hit_record& rec) const {
  Vec3  oc = r.origin() - center;
  double  a = dot(r.direction(), r.direction());
  double  half_b = dot(oc, r.direction());
  double  c = oc.length_squared() - radius * radius;
  double  discriminant = half_b * half_b - a * c;
  if (discriminant < 0) return false;
  double  root = sqrt(discriminant);
  double  temp = (-half_b - root) / a;
  if (temp < t_max && temp > t_min) {
    rec.t = temp;
    rec.p = r.at(rec.t);
    Vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;
    return true;
  }
  temp = (-half_b + root) / a;
  if (temp < t_max && temp > t_min) {
    rec.t = temp;
    rec.p = r.at(rec.t);
    Vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat_ptr = mat_ptr;
    return true;
  }    
  return false;
}

// u, vは[0, 1]にマッピングする
void  get_sphere_uv(const Vec3& p, double& u, double& v) {
  double  phi = atan2(p.z(), p.x());
  double  theta = asin(p.y());
  u = 1- (phi + pi) / (2 * pi);
  v = (theta + pi / 2) / pi;
}
