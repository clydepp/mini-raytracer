#ifndef SPHERE_H
#define SPHERE_H

#include "util.h"
#include "hittable.h"

class sphere : public hittable {
  public:
   sphere(const point3& center, double radius) : center(center), radius(std::fmax(0, radius)) {}

   bool hit(const ray&r, interval ray_t, hit_record& rec) const override {
    vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = h*h - a*c;
    if (discriminant < 0) return false;
    
    auto sqrtd = std::sqrt(discriminant);

    // find the nearest root in the range defined by the min and max t
    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
      root = (h + sqrtd) / a;
      if (!ray_t.surrounds(root))
        return false;
    }

    // update hit record
    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center) / radius; // where p is the intersection point
    rec.set_face_normal(r, outward_normal);


    return true;
   }
  private:
   point3 center;
   double radius;

};

#endif