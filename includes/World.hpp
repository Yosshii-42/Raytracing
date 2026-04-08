#pragma once

#include "Hittable.hpp"
#include "Camera.hpp"
#include "Color.hpp"

struct Scene {
  HittableList  world;
  Camera        cam;
  color         background;
};

Scene random_scene();
Scene earth();
Scene checker();
Scene sample_light();
Scene cornel_box();

// HittableList random_scene();
// HittableList earth();
// HittableList checker();
// HittableList sample_light();
// HittableList cornel_box();
