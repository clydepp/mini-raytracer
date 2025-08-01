#include "util.h"

#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "texture.h"

colour ray_colour(const ray& r, const hittable& world) {
  hit_record rec;
  if (world.hit(r, interval(0, infinity), rec)) { return 0.5*(rec.normal + colour(1,1,1)); }

  vec3 unit_direction = unit_vector(r.direction());
  auto a = 0.5*(unit_direction.y() + 1.0);
  return (1.0-a)*colour(1.0, 1.0, 1.0)+ a*colour(0.5, 0.7, 1.0);
}

void bouncing_spheres() {
  hittable_list world;

  auto checker = make_shared<checker_texture>(0.32, colour(.2, .3, .1), colour(.9, .9, .9));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, make_shared<lambertian>(checker)));

//   auto ground_material = make_shared<lambertian>(colour(0.5, 0.5, 0.5));
//   world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

  for (int a = -2; a < 2; a++) {
      for (int b = -2; b < 2; b++) {
          auto choose_mat = random_double();
          point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

          if ((center - point3(4, 0.2, 0)).length() > 0.9) {
              shared_ptr<material> sphere_material;

              if (choose_mat < 0.8) {
                  // diffuse
                  auto albedo = colour::random() * colour::random();
                  sphere_material = make_shared<lambertian>(albedo);
                  auto center2 = center + vec3(0, random_double(0, 0.5), 0);
                  world.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
              } else if (choose_mat < 0.95) {
                  // metal
                  auto albedo = colour::random(0.5, 1);
                  auto fuzz = random_double(0, 0.5);
                  sphere_material = make_shared<metal>(albedo, fuzz);
                  world.add(make_shared<sphere>(center, 0.2, sphere_material));
              } else {
                  // glass
                  sphere_material = make_shared<dielectric>(1.5);
                  world.add(make_shared<sphere>(center, 0.2, sphere_material));
              }
          }
      }
  }

  auto material1 = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(colour(0.4, 0.2, 0.1));
  world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(colour(0.7, 0.6, 0.5), 0.0);
  world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  world = hittable_list(make_shared<bvh_node>(world));

  camera cam;

  cam.aspect_ratio      = 16.0 / 9.0;
  cam.image_width       = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth         = 50;

  cam.vfov     = 20;
  cam.lookfrom = point3(13,2,3);
  cam.lookat   = point3(0,0,0);
  cam.vup      = vec3(0,1,0);

  cam.defocus_angle = 0.6;
  cam.focus_dist    = 10.0;

  cam.render(world);
}

void checkered_spheres () {
  hittable_list world;

  auto checker = make_shared<checker_texture>(0.32, colour(0.2, 0.3, 0.1), colour(0.9, 0.9, 0.9));

  world.add(make_shared<sphere>(point3(0, -10, 0), 10, make_shared<lambertian>(checker)));
  world.add(make_shared<sphere>(point3(0, 10, 0), 10, make_shared<lambertian>(checker)));

  camera cam;

  cam.aspect_ratio = 16.0/9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 30;

  cam.vfov = 20;
  cam.lookfrom = point3(13, 2, 3);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);
  cam.defocus_angle = 0;

  cam.render(world);
}

void earth () {
  auto earth_texture = make_shared<image_texture>("earthmap.jpg");
  auto earth_surface = make_shared<lambertian>(earth_texture);
  auto globe = make_shared<sphere>(point3(0,0,0), 2, earth_surface);

  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 50;
  cam.max_depth = 20;

  cam.vfov = 20;
  cam.lookfrom = point3(0,0,12);
  cam.lookat = point3(0,0,0);
  cam.vup = vec3(0,1,0);

  cam.defocus_angle = 0;
  cam.render(hittable_list(globe));
}

int main () {
  // here different scenes are defined
  switch (3) {
    case 1: bouncing_spheres(); break;
    case 2: checkered_spheres(); break;
    case 3: earth(); break;
  }
  
}