# CRaytrace 🔭
Implementation of a Monte-Carlo raytracer in C++. This project was done for the course TNCG15 at LiU, ITN during the fall of 2020.

## Issues:
- [*Should be resolved*] RGB values might not be normalized to [0, 255] in some cases
- Möller-Trumbore check might fail sometimes due to doubleing point errors 
- Area light source appears gray-ish when it should be white
- Cosine term from rendering equation as `lightAmount` cause shadow-acne for some reason

## TODO:
- [ ] Ignore rays that end on light sources, use penultimate ray as final ray.
- [ ] Figure out why rays seem to get trapped inside of boxes and tetrahedra.
- [ ] Refactor code base
  - [x] Separate code into more files
  - [x] Collect forward-declarations in one header file, which is included first
  - [x] Replace `Color` with `glm::dvec3`
  - [ ] Use either `snake_case` or `camelCase` for variables/methods (`PascalCase` for classes)
  - [x] `Color` convenience methods and constants
- [ ] Oren Nayar surfaces
- [ ] Transparent surfaces
- [ ] Set const rendering parameters
  - [ ] Lightning sample amount
  - [ ] Importance reduction 
  - [ ] Russian roulette cutoff


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
- [x] Camera should have separate render and create_image functions
  - [x] Render: Cast all the rays and calculate the doubleing point color values of each pixel
  - [x] Create image: Convert the doubleing point RGB values to `png::byte`
- [x] Save render results to an image
  - [x] Convert from `double`-valued colors to 8 bit unsigned integer values
  - [x] Find a way to push pixel values into some kind of an image format in C++
- [x] Simple local lighting model
  - [x] Implement lights in the scene
  - [x] Check that the triangle normals make sense
- [x] Implement scene objects
  - [x] Tetrahedron
  - [x] Sphere (implicitly with radius) *MOA*
  - [x] Handle mutliple ray intersections, using the nearest intersection
- [x] Shadow rays on "local light check"
  - Is there an intersection for the ray going from the object to the light source?
- [x] Handle "ray trees"
  - [x] Spawn more rays when intersecting something reflective/refractive
  - [x] Calculate intensity values for the ray by working from the ends to the original ray

## How to terminal (Guide for Dummies *cough* Moa)
- Go up one directory: `cd ..`
- List the items in a directory: `ls`
- List *all* the items: `la`
- Make a new directory: `mkdir [name]` (e.g. `mkdir build`)

### Building and running
From the root directory of the project (e.g. `craytrace`)
1. `cd build`
2. `cmake ..` (sometimes you need to `rm -rf build/*` from `craytrace` first to restore build)
3. `cmake --build .`
4. `./CRaytracer` (or whatever the binary is called. tip: tab is your friend)

Developed by:
- Frans Johansson
- Moa Gutenwik
