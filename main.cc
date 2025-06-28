#include "util.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

colour ray_colour(const ray& r, const hittable& world) {
  hit_record rec;
  if (world.hit(r, interval(0, infinity), rec)) { return 0.5*(rec.normal + colour(1,1,1)); }

  vec3 unit_direction = unit_vector(r.direction());
  auto a = 0.5*(unit_direction.y() + 1.0);
  return (1.0-a)*colour(1.0, 1.0, 1.0)+ a*colour(0.5, 0.7, 1.0);
}

int main(){
  hittable_list world;

  // auto material_ground = make_shared<lambertian>(colour(0.8, 0.8, 0.0));
  // auto material_center = make_shared<lambertian>(colour(0.1, 0.2, 0.5));
  // auto material_left = make_shared<dielectric>(1.50); // left sphere is air bubble in water
  // auto material_bubble = make_shared<dielectric>(1.00 / 1.50); // left sphere is air bubble in water
  // auto material_right = make_shared<metal>(colour(0.8, 0.6, 0.2), 0.1);

  // world.add(make_shared<sphere>(point3(0,0,-1.2), 0.5, material_center));
  // world.add(make_shared<sphere>(point3(0,-100.5,-1), 100, material_ground));
  // world.add(make_shared<sphere>(point3(-1,0,-1), 0.5, material_left));
  // world.add(make_shared<sphere>(point3(-1,0,-1), 0.45, material_bubble));
  // world.add(make_shared<sphere>(point3(1,0,-1), 0.5, material_right));

  auto R = std::cos(pi/4);
  auto material_left = make_shared<lambertian>(colour(0,0,1));
  auto material_right = make_shared<lambertian>(colour(1,0,0));

  world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
  world.add(make_shared<sphere>(point3(R, 0, -1), R, material_right));

  // make an instance of a camera
  camera cam;

  cam.aspect_ratio = 16.0/9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;

  cam.vfov = 90;

  cam.render(world);

}