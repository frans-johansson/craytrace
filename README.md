# CRaytrace 🔭
Implementation of a Monte-Carlo raytracer in C++. This project was done for the course TNCG15 at LiU, ITN during the fall of 2020.

## TODO:
- [ ] Implement scene objects
  - [ ] Tetrahedron
  - [ ] Sphere (implicitly with radius)
  - [ ] Handle mutliple ray intersections, using the nearest intersection
- [ ] Simple local lighting model
  - [ ] Check that the triangle normals make sense
- [ ] Camera should have separate render and create_image functions
  - [ ] Render: Cast all the rays and calculate the floating point color values of each pixel
  - [ ] Create image: Convert the floating point RGB values to `png::byte`
- [x] Save render results to an image
  - [ ] Convert from `double`-valued colors to 8 bit unsigned integer values
  - [x] Find a way to push pixel values into some kind of an image format in C++

## DONE:
- [x] Walls
- [x] Möller-Trumbore algorithm for triangle intersections
- [x] Set up CMake for building the project
- [x] Handle dependencies with CMake
  - [x] Set up GLM library as dependency
  - [x] Set up PNG++ library as dependency
- [x] Implement (simple) rendering loop
  - [x] Iterate over all pixels
  - [x] Send rays through each pixel and set the color of the pixel to the color of the first intersected triangle
- [x] Move class/struct definitions into header files
- [x] Create implementation file for class memeber functions
- [x] Make triangle verticies public/add getter functions
- [x] Get XYZ components from a `glm::DVec4`
- [x] Changed dvec to vec

Developed by:
- Frans Johansson
- Moa Gutenwik
