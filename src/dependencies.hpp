#pragma once
// Required before including glm
#define GLM_FORCE_SWIZZLE

// Standard library 
#include <iostream>
#include <vector>
#include <array>
#include <memory>
#include <limits>
// External libraries
#include <glm/glm.hpp>
#include <png++/png.hpp>

// Forward declarations
class Camera;
class Scene;

struct Ray;
struct Pixel;
struct PointLight;
struct SceneObject;
struct Triangle;
struct Sphere;

// struct Color {
//     double r, g, b;

//     Color operator*(double scalar) const {
//         return { this->r * scalar, this->g * scalar, this->b * scalar };
//     }

//     Color operator+(const Color& other) const {
//         return { this->r + other.r, this->g + other.g, this->b + other.b };
//     }

//     void operator+=(const Color& other) {
//         *this = *this + other;
//     }
// };

// Project header files
#include "colors.hpp"
#include "objects.hpp"
#include "craytrace.hpp"