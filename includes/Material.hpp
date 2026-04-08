// Material(基底クラス), Lambertian(マット), Metal(金属), Dielectric(透明), Light(発光体)
#pragma once

#include "Utils.hpp"
#include "Hittable.hpp"
#include "Texure.hpp"

class Material {
public:
  virtual ~Material() {}

  // 発光しないマテリアルでemitted()を実装しないで済むよ実装しないで済むように、基底クラスで黒を返す
  virtual color emitted(double u, double v, const point3& p) const {
    (void)u;
    (void)v;
    (void)p;
    return color(0, 0, 0);
  }

  // 物体表面と例の相互作用を処理する
  virtual bool  scatter(
    const Ray& r_in,
    const hit_record& rec,
    color& attnuation,
    Ray& scattered
  ) const = 0;
};

// シュリックの近似
inline double schlick(double cosine, double ref_idx) {
  double  r0 = (1 - ref_idx) / (1 + ref_idx);
  r0 = r0 * r0;
  return r0 + (1 - r0) * pow((1 - cosine), 5);
}

// マット
class Lambartian : public Material{
public:
  Lambartian(shared_ptr<Texture> a) : albedo(a) {}

  virtual bool  scatter(
    const Ray& r_in,
    const hit_record& rec,
    color& attenuation, // この物体で光がどれだけ減衰するか（色として）
    Ray& scattered
  ) const {
    (void)r_in;
    Vec3 scatter_direction = rec.normal + random_unit_vector();
    scattered = Ray(rec.p, scatter_direction);
    attenuation = albedo->value(rec.u, rec.v, rec.p);
    return true;
  }

public:
  shared_ptr<Texture> albedo; // 反射の色
};

// 金属
class Metal : public Material {
public:
  Metal(const color& a) : albedo(a) {}

  virtual bool  scatter(
    const Ray& r_in,
    const hit_record& rec,
    color& attenuation,
    Ray& scattered
  ) const {
    Vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
    scattered = Ray(rec.p, reflected);
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
  }

public:
  color albedo;
};

// 透明
class Dielectric: public Material {
public:
  Dielectric(double ri) : ref_idx(ri) {}

  virtual bool scatter(
    const Ray& r_in,
    const hit_record& rec,
    color&  attenuation,
    Ray& scattered
  ) const {
    attenuation = color(1.0, 1.0, 1.0);
    double etai_over_etat;
    if (rec.front_face)               // true 空気(1.0)->ガラス(1.5)
      etai_over_etat = 1.0 / ref_idx;
    else                              // false ガラス(1.5)->空気(1.0)  
      etai_over_etat = ref_idx;
  
    // 
    Vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    if (etai_over_etat * sin_theta > 1.0) {
      Vec3 reflected = reflect(unit_direction, rec.normal);
      scattered = Ray(rec.p, reflected);
      return true;
    }

    // シュリックの近似を追加
    double reflect_prob = schlick(cos_theta, etai_over_etat);
    if (random_double() < reflect_prob) {
      Vec3 reflected = reflect(unit_direction, rec.normal);
      scattered = Ray(rec.p, reflected);
      return true;
    }

    Vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
    // rec.p:当たった点 refrected:屈折語の新しい方向
    scattered = Ray(rec.p, refracted);
    return true;
  }

public:
  double  ref_idx;
};

class Light : public Material {
public:
  Light(shared_ptr<Texture> a) : emit(a) {}

  virtual bool  scatter(
    const Ray& r_in,
    const hit_record& rec,
    color& attenuation,
    Ray& scattered
  ) const {
    (void)r_in;
    (void)rec;
    (void)attenuation;
    (void)scattered;
    return false;
  }

  virtual color emitted(double u, double v, const point3& p) const {
    return emit->value(u, v, p);
  }

public:
  shared_ptr<Texture>  emit;
};
