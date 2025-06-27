#include "colour.h"
#include "vec3.h"

#include <iostream>
#include <fstream>

int main(){

  int image_width = 256;
  int image_height = 256;

  std::ofstream file("image_head.ppm");
  file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

  for (int j = 0; j < image_height; j++) {
    for (int i = 0; i < image_width; i++) {
      auto pixel_colour = colour(double(i)/(image_width-1), 0, double(j)/(image_height-1));
      write_colour(file, pixel_colour);
    }
  }
      
  file.close();
}