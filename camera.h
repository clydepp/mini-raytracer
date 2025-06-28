#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"
#include <fstream>

class camera {
  public:
    double aspect_ratio = 1.0;
    int image_width = 100;
    int samples_per_pixel = 10;
    int max_depth = 10;

    void render(const hittable& world) {
      initialise();

      std::ofstream file("image.ppm");
      file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

      for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {
          colour pixel_colour(0,0,0);
          for (int sample = 0; sample < samples_per_pixel; sample++) {
            ray r = get_ray(i, j);
            pixel_colour += ray_colour(r, max_depth, world);
          }
      
          write_colour(file, pixel_samples_scale * pixel_colour);
        }
      }
          
      file.close();

    }

  private:
    int image_height;
    double pixel_samples_scale;
    point3 center;
    point3 pixel00_loc;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;

    void initialise() {
      image_height = int(image_width/aspect_ratio);
      image_height = (image_height < 1) ? 1 : image_height;

      pixel_samples_scale = 1.0 / samples_per_pixel;

      auto focal_length = 1.0;
      auto viewport_height = 2.0;
      auto viewport_width = viewport_height * (double(image_width)/image_height);
      center = point3(0,0,0);

      auto viewport_u = vec3(viewport_width, 0, 0);
      auto viewport_v = vec3(0, -viewport_height, 0);

      pixel_delta_u = viewport_u / image_width;
      pixel_delta_v = viewport_v / image_height;

      // location setup of upper left pixel
      auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
      pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    }

    ray get_ray(int i, int j) const {
      // randomly sample around pixel i, j

      auto offset = sample_square();
      auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

      auto ray_origin = center;
      auto ray_direction = pixel_sample - ray_origin;

      return ray(ray_origin, ray_direction);
    }

    vec3 sample_square() const {
      // returns vector to random point within the half-way bounded unit square
      return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    colour ray_colour(const ray& r, int depth, const hittable& world) const {
      if (depth <= 0) return colour(0,0,0);
      
      hit_record rec;
      if (world.hit(r, interval(0.001, infinity), rec)) {
        ray scattered;
        colour attenuation;
        
        if (rec.mat->scatter(r, rec, attenuation, scattered)) 
          return attenuation * ray_colour(scattered, depth-1, world);
        return colour(0,0,0);
        // vec3 direction = rec.normal + random_unit_vector(); // this is vector PS
        // return 0.5*ray_colour(ray(rec.p, direction), depth-1, world);
      }

      vec3 unit_direction = unit_vector(r.direction());
      auto a = 0.5*(unit_direction.y() + 1.0);
      return (1.0-a)*colour(1.0, 1.0, 1.0)+ a*colour(0.5, 0.7, 1.0);
    }
};

#endif