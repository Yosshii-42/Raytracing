// Texture(基底), SolidColor, Checker(床用チェッカー), UVChecker(球用チェッカー), Image(画像貼り付け用)
#pragma once

#include "Utils.hpp"
#include "stb_image.h"

#include <iostream>

class Texture {
public:
  virtual ~Texture() {}
  virtual color value(double u, double v, const point3& p) const = 0;
};

class SolidColor : public Texture {
public:
  SolidColor() {}
  SolidColor(color c) : color_value(c) {}
  SolidColor(double red, double green, double blue) : SolidColor(color(red, green, blue)) {}
  virtual color value(double u, double v, const Vec3& p) const {
    (void)u;
    (void)v;
    (void)p;
    return color_value;
  }

private:
  color color_value;
};

// チェッカー床用
class Checker : public Texture {
public:
  Checker() {}
  Checker(shared_ptr<Texture> t0, shared_ptr<Texture>t1) : even(t0), odd(t1) {}

  virtual color value(double u, double v, const point3& p) const {
    (void)u;
    (void)v;
    double  sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
    if (sines < 0)
      return odd->value(u, v, p);
    return even->value(u, v, p);
  }

public:
  shared_ptr<Texture> even;
  shared_ptr<Texture> odd;
};

// チェッカー球用
class UVChecker : public Texture {
public:
  UVChecker() {}
  UVChecker(shared_ptr<Texture> t0, shared_ptr<Texture>t1) : even(t0), odd(t1) {}

  virtual color value(double u, double v, const point3& p) const {
    int iu = int(u * 50);
    int iv = int(v * 50);
    if ((iu + iv) % 2 == 0)
      return odd->value(u, v, p);
    else
      return even->value(u, v, p);
  }

public:
  shared_ptr<Texture> even;
  shared_ptr<Texture> odd;
};

// 画像貼り付け用
class Image : public Texture {
public:
  const static int  byte_per_pixel = 3;

  Image() : data(nullptr), width(0), height(0), byte_per_scanline(0) {}
  Image(const char* filename) {
    int  components_per_pixel = byte_per_pixel;
  
    data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);
    if (!data) {
      std::cerr << "ERROR: Could not load texure image file '"
                << filename
                << "'.\n";
      width = height = 0;
    }

    byte_per_scanline = byte_per_pixel * width;
  }

  ~Image() {
    delete data;
  }

  virtual color value(double u, double v, const Vec3& p) const {
    if (data == nullptr)
      return color(0, 1, 1);
    
    (void)p;
    // 入力されたテクスチャ座標を[0, 1]で切り捨てる
    u = clamp(u, 0.0, 1.0);
    v = 1.0 - clamp(v, 0.0, 1.0); // vを反転させて画像の座標形に合わせる

    int  i = static_cast<int>(u * width);
    int  j = static_cast<int>(v * height);

    // 整数座標をさらに切り捨てる(テクスチャ座標は1.0になってはいけない)
    if (i >= width) i = width - 1;
    if (j >= height) j = height - 1;

    const double  color_scale = 1.0 / 255.0;
    unsigned char*  pixel = data + j * byte_per_scanline + i * byte_per_pixel;

    return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
  }

private:
  unsigned char *data;
  int width, height;
  int byte_per_scanline;
};
