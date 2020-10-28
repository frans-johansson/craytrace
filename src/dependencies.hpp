#pragma once
// Required before including glm
// #define GLM_FORCE_SWIZZLE

// Standard library 
#include <iostream>
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
// const float IMPORTANCE_FALLOFF = 0.50;
// const double EPSILON = 0.00001;
const float MAX_SHADOW_BIAS = 1e-5;
const float MIN_ABSORPTION = 0.75;
const float MAX_ABSORPTION = 0.85;
const int LIGHT_SAMPLES = 1;
const int PIXEL_SAMPLES = 4;
const double LOCAL_LIGHTING_CONTRIBUTION = 0.5;
const int INNER_REFLECTIONS_LIMIT = 1;
const float AIR_INDEX = 1.0;

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