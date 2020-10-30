#pragma once
// Required before including glm
// #define GLM_FORCE_SWIZZLE

// Standard library 
#include <iostream>
#include <sstream>
#include <vector>
#include <array>
#include <memory>
#include <limits>
#include <thread>
#include <chrono>
#include <math.h>
// External libraries
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <png++/png.hpp>

// Constants
// const double IMPORTANCE_FALLOFF = 0.50;
const double EPSILON = 1e-8;
const double MAX_RAY_OFFSET = 1e-4;
const double MIN_ABSORPTION = 0.65;
const double MAX_ABSORPTION = 0.85;
const int LIGHT_SAMPLES = 1;
const int PIXEL_SAMPLES = 1;
const double LOCAL_LIGHTING_CONTRIBUTION = 0.3;
const int INNER_REFLECTIONS_LIMIT = 1;
const double AIR_INDEX = 1.0;

// Forward declarations
class Camera;
class Scene;

struct Ray;
struct Pixel;
struct PointLight;
struct SceneObject;
struct Triangle;
struct Sphere;

// Project header files
#include "colors.hpp"
#include "materials.hpp"
#include "objects.hpp"
#include "craytrace.hpp"

const std::shared_ptr<Triangle> DUMMY_TARGET = std::make_shared<Triangle>(Triangle{
    glm::dvec3(-10.0, -10.0, -10.0),
    glm::dvec3(-10.0, -10.0, -10.0),
    glm::dvec3(-10.0, -10.0, -10.0),
    MaterialFactory::makeDiffuseLambertian(MAGENTA)
});