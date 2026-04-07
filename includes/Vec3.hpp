#pragma once

#include "Utils.hpp"

#include <cmath>
#include <iostream>

using std::sqrt;

class Vec3 {
  public:
    Vec3() : e{0,0,0} {}
    Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    Vec3& operator+=(const Vec3& v) {
      e[0] += v.e[0];
      e[1] += v.e[1];
      e[2] += v.e[2];
      return *this;
    }

    Vec3& operator*=(const double t) {
      e[0] *= t;
      e[1] *= t;
      e[2] *= t;
      return *this;
    }

    Vec3& operator/=(const double t) {
      return *this *= 1/t;
    }

    double length() const {
      return sqrt(length_squared());
    }

    double length_squared() const {
      return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; 
    }

    inline static Vec3  random() {
      return Vec3(random_double(), random_double(), random_double());
    }

    inline static Vec3  random(double min, double max) {
      return Vec3(random_double(min, max),
                  random_double(min, max),
                  random_double(min, max));
    }

  public:
    double  e[3];
};

//Vec3の型エイリアス
using point3 = Vec3; // 3D 点
using color = Vec3;  // RGB

// utils 関数
inline std::ostream& operator<<(std::ostream &out, const Vec3& v) {
  return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
  return Vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
  return Vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
  return Vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vec3 operator*(double t, const Vec3& v) {
  return Vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}

inline Vec3 operator*(const Vec3& v, double t) {
  return t * v;
}

inline Vec3 operator/(const Vec3& v, double t) {
  return (1 / t) * v;
}

inline double dot(const Vec3& u, const Vec3& v) {
  return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline Vec3 cross(const Vec3& u, const Vec3& v) {
  return Vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);  
}

inline Vec3 unit_vector(Vec3 v) {
  return v / v.length();
}

// 拡散方向を作る
// 球体の中にランダムに点を取る ノイズ多い
inline Vec3  random_in_unit_sphere() {
  while (true) {
    Vec3  p = Vec3::random(-1, 1);
    if (p.length_squared() >= 1) continue;
    return p;
  }
}

// ランバーティアン法 物理的に正しい
inline Vec3  random_unit_vector() {
  double  a = random_double(0, 2 * pi);
  double  z = random_double(-1, 1);
  double  r = sqrt(1 - z * z);
  return Vec3(r * cos(a), r * sin(a), z);
}

// 均一な半球分布 綺麗
inline Vec3  random_in_hemisphere(const Vec3& normal) {
  Vec3  in_unit_sphere = random_in_unit_sphere();
  if (dot(in_unit_sphere, normal) > 0.0)
    return in_unit_sphere;
  else
    return -in_unit_sphere;
}

// 反射
inline Vec3  reflect(const Vec3& v, const Vec3& n) {
  return v - 2 * dot(v, n) * n;
}

// 屈折
// uv:正規化された入射レイ(unit vector)
// n:接点の法線(normal)
// eta:屈折率(η/イータ)
// i:入射側(incident)
// t:透過側(transmitted)
// etai_over_etat: 入射側の屈折率 ηi を透過側の屈折率 ηt で割った値
// r:レイ
// out:外に出る(屈折後)
// perpendicular: 垂直
// r_out_parallel: 表面に平行な成分
// r_out_perp: 法線方向の成分（名前は perp だが、この式では n 方向の成分）
inline Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
  double cos_theta = fmin(dot(-uv, n), 1.0);
  Vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
  Vec3 r_out_perp = -sqrt(fabs(1.0 - r_out_parallel.length_squared())) * n;
  return r_out_parallel + r_out_perp;
}
