// main.cpp

#include "Utils.hpp"

#include "HittableList.hpp"
#include "Sphere.hpp"
#include "Color.hpp"
#include "Camera.hpp"
#include "Material.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// 色の計算
color ray_color(const Ray& r, const Hittable& world, int depth) {
  //　worldにhitを問い合わせる
  hit_record  rec;
  
  // 反射回数が定数以上になったら追跡をやめる
  if (depth <= 0)
    return color(0, 0, 0);

  // マットな物体の反射
  // if (world.hit(r, 0.001, infinity, rec)) {
  //   point3  target = rec.p + rec.normal + random_in_hemisphere(rec.normal);
  //   return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth - 1);
  // }

  // 金属反射
  if (world.hit(r, 0.001, infinity, rec)) {
    Ray   scattered;
    color attenuation;
    if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
      return attenuation * ray_color(scattered, world, depth - 1);
    return color(0, 0, 0);
  } 

  // 背景の描画
  Vec3 unit_direction = unit_vector(r.direction());
  auto t = 0.5 * (unit_direction.y() + 1.0); // -1 ~ +1 を 0 ~ 1 に変換
  return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

// HittableList random_scene() {
//   HittableList world;

//   auto ground_material = make_shared<Lambartian>(color(0.5, 0.5, 0.5));
//   world.add(make_shared<Sphere>(point3(0,-1000,0), 1000, ground_material));

//   for (int a = -11; a < 11; a++) {
//     for (int b = -11; b < 11; b++) {
//       auto choose_mat = random_double();
//       point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

//       if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
//         shared_ptr<Material> sphere_material;
//         if (choose_mat < 0.8) {
//           // diffuse
//           auto albedo = color::random() * color::random();
//           sphere_material = make_shared<Lambartian>(albedo);
//           world.add(make_shared<Sphere>(center, 0.2, sphere_material));
//         } else if (choose_mat < 0.95) {
//           // metal
//           auto albedo = color::random(0.5, 1);
//           // auto fuzz = random_double(0, 0.5);
//           sphere_material = make_shared<Metal>(albedo);
//           world.add(make_shared<Sphere>(center, 0.2, sphere_material));
//         } else {
//           // glass
//           sphere_material = make_shared<Dielectric>(1.5);
//           world.add(make_shared<Sphere>(center, 0.2, sphere_material));
//         }
//       }
//     }
//   }

//   auto material1 = make_shared<Dielectric>(1.5);
//   world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));
//   auto material2 = make_shared<Lambartian>(color(0.4, 0.2, 0.1));
//   world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));
//   auto material3 = make_shared<Metal>(color(0.7, 0.6, 0.5));
//   world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

//   return world;
// }

HittableList  earth() {
  auto  earth = make_shared<Image>("earthmap.jpg");
  auto  earth_surface = make_shared<Lambartian>(earth);
  auto  globe = make_shared<Sphere>(point3(0, 0, 0), 2, earth_surface);
  return HittableList(globe);
}

int main() {
  const auto aspect_ratio = 16.0 / 9.0;
  const int image_width = 384;
  const int image_height = static_cast<int>(image_width / aspect_ratio);
  // 10 100回レイを飛ばす
  const int samples_per_pixcel = 200;
  const int max_depth = 40;

  std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

  HittableList  world;
  // world = random_scene();
  // world = earth();
  // point3 lookfrom(13,2,3);
  // point3 lookat(0,0,0);
  // Vec3 vup(0,1,0);
  // Camera cam(lookfrom, lookat, vup, 20, aspect_ratio);

  Camera  cam(point3(6, 6, 1), point3(0, 0, -6), Vec3(0, 1, 0), 15, aspect_ratio);
  auto  R = cos(pi/4);

  // world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100));
  // world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5));
  auto  checker = make_shared<Checker>(
    make_shared<SolidColor>(0.2, 0.3, 0.1),
    make_shared<SolidColor>(0.9, 0.9, 0.9)
  );
  world.add(make_shared<Sphere>(point3(0, -100.7, -6), 100, make_shared<Lambartian>(checker)));
  world.add(make_shared<Sphere>(
    point3(0, 0, -6), 0.5, make_shared<Lambartian>(make_shared<SolidColor>(0.0, 0.3, 0.3))));
  // world.add(make_shared<Sphere>(
  //   point3(0, -100.7, -6), 100, make_shared<Lambartian>(make_shared<SolidColor>(0.8, 0.8, 0))));
  world.add(make_shared<Sphere>(
    point3(-R, 0, -6), R, make_shared<Metal>(color(0.0, 1.0, 0.0))));
  world.add(make_shared<Sphere>(
    point3(R, 0, -6), R, make_shared<Dielectric>(1.5)));
  // world.add(make_shared<Sphere>(
  //   point3(R, 0, -6), (R * -0.9), make_shared<Dielectric>(1.5)));

  for (int j = image_height - 1; j >= 0; --j) {
    std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
    for (int i = 0; i < image_width; ++i) {
      color pixel_color(0, 0, 0);
      for (int s = 0; s < samples_per_pixcel; ++s) {
        auto  u = (i + random_double()) / (image_width - 1);
        auto  v = (j + random_double()) / (image_height - 1);
        // レイを生成する
        Ray r = cam.get_ray(u, v);  
        pixel_color += ray_color(r, world, max_depth);

      }
      // 出力
      write_color(std::cout, pixel_color, samples_per_pixcel);
    }
  }
  std::cerr << "\nDone\n";
}
