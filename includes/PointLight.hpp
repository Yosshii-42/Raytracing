#pragma once

#include "Vec3.hpp"
#include "Hittable.hpp"
#include "Material.hpp"

struct PointLight {
  point3  position;
  color   intensity;
};

inline bool  in_shadow(const point3& p, const PointLight& light, const Hittable& world) {
  Vec3    to_light = light.position - p;          // 光源より手前に遮る物体があるかだけを調べる
  double  distance_to_light = to_light.length();
  Vec3    direction = unit_vector(to_light);

  Ray     shadow_ray(p + 0.001 * direction, direction);

  hit_record  rec;
  return world.hit(shadow_ray, 0.001, distance_to_light, rec);
}

inline color computed_direct_light(const hit_record& rec, const PointLight& light, const Hittable& world) {
  Vec3    to_light = light.position - rec.p;          // 光源より手前に遮る物体があるかだけを調べる
  double  distance_to_light = to_light.length();
  Vec3    direction = unit_vector(to_light);

  Ray     shadow_ray(rec.p + 0.001 * direction, direction);

  hit_record  shadow_rec;
  if (world.hit(shadow_ray, 0.001, distance_to_light, shadow_rec))
    return color(0, 0, 0);

  double  n_dot_l = dot(rec.normal, direction);
  if (n_dot_l < 0)
    n_dot_l = 0;
  
  color surface_color = rec.mat_ptr->diffuse_color(rec.u, rec.v, rec.p);

  return  surface_color * n_dot_l * light.intensity;
}

inline color direct_light(const hit_record& rec, const PointLight& light, const Hittable& world) {
  if (in_shadow(rec.p, light, world))
    return color(0, 0, 0);

  Vec3    light_dir = unit_vector(light.position - rec.p);
  double  brightness = dot(rec.normal, light_dir);

  if (brightness < 0)
    brightness = 0;

  return brightness * light.intensity;
}
