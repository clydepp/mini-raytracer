# mini-raytracer
> Ray tracing calculates the intersection of rays (essentially lines) with scene geometry.
## Motivation
I took interest in my year 2 IAC module, and this inspired me to design and write a fundamental part of a CPU or GPU. I deduced that writing a raytracer would ultimately benefit me as:
* I would develop my use of C++ and Linux
* I could use my knowledge of linear algebra and vectors in a ray/path tracing context
* this would challenge me (and allow me to get creative)

## Project aims
* Improve my knowledge of C++
* Understand the methods used in path/raytracing
* Eventually make it real-time, using CUDA and OpenGL OR an FPGA.

## Usage
Go into the project folder and run these three commands:
```
cmake --build build
.\build\simple_implement.exe
Start-Process image.ppm
```