#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"

class material {
  public:
    virtual ~material() = default;

    virtual bool scatter (const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const {
      return false;
    }
};

class lambertian : public material {
  public:
    lambertian(const colour& albedo) : tex(make_shared<solid_colour>(albedo)) {}
    lambertian(shared_ptr<texture> tex) : tex(tex) {}

    bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const override {
      auto scatter_direction = rec.normal + random_unit_vector();

      // catch degenerate scatter direction
      if (scatter_direction.near_zero()) scatter_direction = rec.normal;

      scattered = ray(rec.p, scatter_direction, r_in.time());
      attenuation = tex->value(rec.u, rec.v, rec.p);
      return true;
    }
  
    private:
      shared_ptr<texture> tex;
};

class metal : public material {
  public:
    metal(const colour& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const override {
      vec3 reflected = reflect(r_in.direction(), rec.normal); // this is v + 2b
      reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
      
      scattered = ray(rec.p, reflected, r_in.time());
      attenuation = albedo;
      return (dot(scattered.direction(), rec.normal) > 0);
    }
  
    private:
      colour albedo;
      double fuzz;
};

class dielectric : public material {
  public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const override {
      attenuation = colour(1.0, 1.0, 1.0);
      double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

      vec3 unit_direction = unit_vector(r_in.direction());
      // vec3 refracted = refract(unit_direction, rec.normal, ri);

      // adding in TIR or Snell's
      double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
      double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

      bool cannot_refract = ri * sin_theta > 1.0;
      vec3 direction;

      if (cannot_refract || reflectance(cos_theta, ri) > random_double()) direction = reflect(unit_direction, rec.normal);
      else direction = refract(unit_direction, rec.normal, ri);

      scattered = ray(rec.p, direction, r_in.time());
      return true;
    }
  
  private:
    double refraction_index;

    static double reflectance(double cosine, double refraction_index) {
      auto r0 = (1 - refraction_index) / (1 + refraction_index);
      r0 = r0*r0;
      return r0 + (1-r0)*std::pow((1 - cosine), 5);
    }
};

#endif