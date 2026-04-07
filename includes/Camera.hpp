#pragma once

#include "Utils.hpp"

class Camera {
public:
  Camera(
    point3 lookfrom,
    point3 lookat,
    Vec3   vup,   // 視線ベクトルと垂直
    double vfov,  // 垂直方向の視野角
    double aspect_ratio
  ) {
    double  theta = degrees_to_radians(vfov);
    double  h = tan(theta / 2);       // 画面の半分の高さを求める
    double  viewport_height = 2.0 * h;
    double  viewport_width = aspect_ratio * viewport_height;

    Vec3  w = unit_vector(lookfrom - lookat);
    Vec3  u = unit_vector(cross(vup, w));
    Vec3  v = cross(w, u);

    origin = lookfrom;
    horizontal = viewport_width * u;
    vertical = viewport_height * v;
    lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
  }

  // レイ生成
  // u:横位置(0-1) v:縦位置(0-1)
  Ray get_ray(double u, double v) const {
    return Ray(origin, lower_left_corner + u * horizontal + v * vertical - origin);
  }

public:
  point3  origin;
  point3  lower_left_corner;
  Vec3    horizontal;
  Vec3    vertical;
};
