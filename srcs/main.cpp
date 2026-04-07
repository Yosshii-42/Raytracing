// main.cpp

#include "Utils.hpp"
#include "Color.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "World.hpp"

// #include <iostream>

// #define STB_IMAGE_IMPLEMENTATION
// #include "stb_image.h"

// 色の計算
color ray_color(const Ray& r, const color& background, const Hittable& world, int depth) {
  //　hitを問い合わせる
  hit_record  rec;
  
  // 反射回数が定数以上になったら追跡をやめる
  if (depth <= 0)
    return color(0, 0, 0);

  // レイがどのオブジェクトとも交わらないなら、背景色を返す
  if (!world.hit(r, 0.001, infinity, rec))
    return background;

  Ray scattered;
  color attenuation;
  color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

  if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
    return emitted;

  return emitted + attenuation * ray_color(scattered, background, world, depth - 1);

  // 背景の描画
  Vec3 unit_direction = unit_vector(r.direction());
  double t = 0.5 * (unit_direction.y() + 1.0); // -1 ~ +1 を 0 ~ 1 に変換
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
  // const double aspect_ratio = 16.0 / 9.0;
  const double aspect_ratio = 1.0;

  const int image_width = 384;
  // const int image_width = 500;

  const int image_height = static_cast<int>(image_width / aspect_ratio);
  const int samples_per_pixcel = 1000; // 1点あたりのレイを飛ばす回数
  const int max_depth = 20;
  // const color background(0.70, 0.80, 1.00);
  const color background(0, 0, 0);

  std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

  HittableList  world;
  // world = random_scene();
  // world = checker();
  // world = earth();
  // world = sample_light();
  world = cornel_box();

  Camera  cam(point3(278, 278, -800), point3(278, 278, 0), Vec3(0, 1, 0), 40, aspect_ratio);

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      color pixel_color(0, 0, 0);
      for (int s = 0; s < samples_per_pixcel; ++s) {
        double  u = (i + random_double()) / (image_width - 1);
        double  v = (j + random_double()) / (image_height - 1);
        // レイを生成する
        Ray r = cam.get_ray(u, v);  
        pixel_color += ray_color(r, background, world, max_depth);

      }
      // 出力
      write_color(std::cout, pixel_color, samples_per_pixcel);
    }
  }
  std::cerr << "\nDone\n";
}
