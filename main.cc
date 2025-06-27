#include <iostream>
#include <fstream>

int main(){

    int image_width = 256;
    int image_height = 256;

    std::ofstream file("image_new.ppm");
    file << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        for (int i = 0; i < image_width; i++) {

            // colours scaled between 0.0 and 1.0
            auto r = 0.0;
            auto g = double(j)/ (image_height - 1);
            auto b = double(i)/ (image_width - 1);

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            file << ir << ' ' << ig << ' ' << ib << '\n';
        }
     }
     
     file.close();
}