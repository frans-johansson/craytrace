# CRaytrace 🔭
Implementation of a Monte-Carlo raytracer in C++. This project was done for the course TNCG15 at LiU, ITN during the fall of 2020.

## TODO:
- [ ] Move class/struct definitions into header files
- [ ] Create implementation file for class memeber functions
- [ ] Make triangle verticies public/add getter functions
- [ ] Get XYZ components from a `glm::DVec4`
- [ ] Triangle normals
- [ ] Implement (simple) rendering loop
  - [ ] Iterate over all pixels
  - [ ] Send rays through each pixel and set the color of the pixel to the color of the first intersected triangle
- [ ] Save render results to an image
  - [ ] Convert from `double`-valued colors to 8 bit unsigned integer values
  - [ ] Find a way to push pixel values into some kind of an image format in C++

## DONE:
- [x] Walls
- [x] Möller-Trumbore algorithm for triangle intersections
- [x] Set up CMake for building the project
- [x] Set up GLM library as dependency

Developed by:
- Frans Johansson
- Moa Gutenwik
