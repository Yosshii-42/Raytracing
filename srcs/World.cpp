
#include "Utils.hpp"

#include "Hittable.hpp"
#include "Color.hpp"
#include "Camera.hpp"
#include "Material.hpp"

#include "Sphere.hpp"
#include "Rectabgle.hpp"
#include "World.hpp"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// 多数の球を表示
Scene random_scene() {
  HittableList world;

  shared_ptr<Material> ground_material = 
    make_shared<Lambartian>(make_shared<SolidColor>(0.5, 0.5, 0.5));
  world.add(make_shared<Sphere>(point3(0,-1000,0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      double choose_mat = random_double();
      point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

      if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<Material> sphere_material;
        if (choose_mat < 0.8) {
          // diffuse
          color albedo = color::random() * color::random();
          sphere_material = make_shared<Lambartian>(make_shared<SolidColor>(albedo));
          world.add(make_shared<Sphere>(center, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          color albedo = color::random(0.5, 1);
          // double fuzz = random_double(0, 0.5);
          sphere_material = make_shared<Metal>(albedo);
          world.add(make_shared<Sphere>(center, 0.2, sphere_material));
        } else {
          // glass
          sphere_material = make_shared<Dielectric>(1.5);
          world.add(make_shared<Sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  shared_ptr<Material> material1 = make_shared<Dielectric>(1.5);
  world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));
  shared_ptr<Material> material2 = make_shared<Lambartian>(make_shared<SolidColor>(0.4, 0.2, 0.1));
  world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));
  shared_ptr<Material> material3 = make_shared<Metal>(color(0.7, 0.6, 0.5));
  world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));

  return Scene{
    world,
    Camera(point3(13, 2, 3), point3(0, 0, 0), Vec3(0, 1, 0), 20, 16.0 / 9.0),
    color(0.70, 0.80, 1.00) 
  };
}

// 地球儀
Scene  earth() {
  shared_ptr<Texture>  earth_tex = make_shared<Image>("images/earthmap.jpg");
  shared_ptr<Lambartian>  earth_surface = make_shared<Lambartian>(earth_tex);
  HittableList world;
  world.add(make_shared<Sphere>(point3(0, 0, 0), 2, earth_surface));
  return Scene{
    world,
    Camera(point3(12, 2, 3), point3(0, 0, 0), Vec3(0, 1, 0), 20, 16.0 / 9.0),
    color(0.70, 0.80, 1.00) 
  };
}

// チェッカーマーク入り
Scene  checker() {
  double  R = cos(pi/4);
  HittableList  world;

  // 平面に貼る時はChecker, 球体に貼る時はUVCcheckerを使う
  shared_ptr<Texture>  checker = make_shared<Checker>(
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
  world.add(make_shared<Sphere>(
    point3(R, 0, -6), (R * -0.9), make_shared<Dielectric>(1.5)));

  return Scene{
    world,
    Camera(point3(6, 6, 1), point3(0, 0, -6), Vec3(0, 1, 0), 15, 16.0 / 9.0),
    color(0.70, 0.80, 1.00) 
  };
}

// 発光体 使用時には、main()のbackgroundを(0, 0, 0)にする
Scene  sample_light() {
  HittableList  world;
  shared_ptr<Material> red = make_shared<Lambartian>(make_shared<SolidColor>(0.9, 0.1, 0.1));
  shared_ptr<Material> grey = make_shared<Lambartian>(make_shared<SolidColor>(0.5, 0.5, 0.5));
  shared_ptr<Material> difflight = make_shared<Light>(make_shared<SolidColor>(8, 8, 8));
  shared_ptr<Material> wall = make_shared<Lambartian>(make_shared<SolidColor>(0.95, 0.92, 0.88));

  world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, grey));  // グメーの床面
  world.add(make_shared<Sphere>(point3(2, 2, -5), 2, red));         // 赤い球
  world.add(make_shared<Sphere>(point3(0, 7, -5), 3, make_shared<Dielectric>(1.5)));    // ガラス球
  world.add(make_shared<Sphere>(point3(0, 7, -5), 0.5, difflight));   // 球の発光体
  world.add(make_shared<RectangleXY>(3, 5, 1, 3, -2, difflight));     // 四角い発光体
  world.add(make_shared<RectangleXY>(-20, 20, -2, 20, -10, wall));
  // world.add(make_shared<Sphere>(point3(0, 7, -5), -2.5, make_shared<Dielectric>(1.5))); // ガラス球を中空にする
  return Scene{
    world,
    Camera(point3(0, 3, 40), point3(0, 3, 0), Vec3(0, 1, 0), 20, 16.0 / 9.0),
    color(0.0, 0.0, 0.0) 
  };
}

// コーネルボックス
Scene cornel_box() {
  HittableList world;

  shared_ptr<Material>  red_wall = make_shared<Lambartian>(make_shared<SolidColor>(.65, .05, .05));
  shared_ptr<Material>  white_wall = make_shared<Lambartian>(make_shared<SolidColor>(.73, .73, .73));
  shared_ptr<Material>  green_wall = make_shared<Lambartian>(make_shared<SolidColor>(.12, .45, .15));
  shared_ptr<Material>  light = make_shared<Light>(make_shared<SolidColor>(100, 100, 100));

  world.add(make_shared<RectangleYZ>(0, 555, 0, 555, 555, green_wall));
  world.add(make_shared<RectangleYZ>(0, 555, 0, 555, 0, red_wall));
  world.add(make_shared<RectangleZX>(213, 343, 227, 332, 554, light));
  world.add(make_shared<RectangleZX>(0, 555, 0, 555, 0, white_wall));
  world.add(make_shared<RectangleZX>(0, 555, 0, 555, 555, white_wall));
  world.add(make_shared<RectangleXY>(0, 555, 0, 555, 555, white_wall));
  return Scene{
    world,
    Camera(point3(278, 278, -800), point3(278, 278, 0), Vec3(0, 1, 0), 40, 1.0),
    color(0.0, 0.0, 0.0) 
  };
}
