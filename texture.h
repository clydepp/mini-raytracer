#ifndef TEXTURE_H
#define TEXTURE_H

class texture {
  public:
    virtual ~texture() = default;
    virtual colour value (double u, double v, const point3& p) const = 0;
};

class solid_colour : public texture {
  public:
    solid_colour(const colour& albedo) : albedo(albedo) {}
    solid_colour(double red, double green, double blue) : solid_colour(colour(red, green, blue)) {}

    colour value (double u, double v, const point3& p) const override {
      return albedo;
    }

  private:
    colour albedo;
};

class checker_texture : public texture {
  public:
    checker_texture(double scale, shared_ptr<texture> even,  shared_ptr<texture> odd)
      : inv_scale(1.0 / scale), even(even), odd(odd) {}

    checker_texture(double scale, const colour& c1, const colour& c2)
      : checker_texture(scale, make_shared<solid_colour>(c1),  make_shared<solid_colour>(c2)) {}

    colour value (double u, double v, const point3& p) const override {
      auto xInt = int(std::floor(inv_scale *  p.x()));
      auto yInt = int(std::floor(inv_scale *  p.y()));
      auto zInt = int(std::floor(inv_scale *  p.z()));
      
      bool isEven = (xInt + yInt + zInt) % 2 == 0;
      return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }
  private:
    double inv_scale;
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};

#endif