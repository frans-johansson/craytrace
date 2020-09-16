#pragma once

#include <iostream>
#include <vector>
#include <memory>
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <png++/png.hpp>

const int IMAGE_SIZE = 800;
const float PIXEL_SIZE = 2.0f / IMAGE_SIZE;

struct Ray;

struct Color {
    double r, g, b;
};

class Triangle { 
private:
    glm::vec3 normal;

public:
    glm::vec4 v1, v2, v3;
    Color color;

    Triangle(glm::vec4 _v1, glm::vec4 _v2, glm::vec4 _v3, Color _color)
        : v1{_v1}, v2{_v2}, v3{_v3}, color{_color}
    {
        this->normal = glm::cross((v2.xyz()-v1.xyz()), (v3.xyz()-v1.xyz()));
    } 

    glm::vec3 rayIntersection(const Ray& ray);
};

struct Ray {
    glm::vec4 start, end;
    std::shared_ptr<Triangle> target;
    Color color;
    double importance;    
};

struct Pixel {
    Color color;
    std::vector<Ray> rays;
};


class Scene {
private:
    std::vector<Triangle> walls;

public:
    Scene(std::vector<Triangle> _walls)
    : walls{_walls} { }

    void rayIntersection(Ray &ray) const;
};

class Camera {
private:
    glm::vec4 primaryEye, secondaryEye;
    bool primaryEyeActive;
    // Pixel image [IMAGE_SIZE][IMAGE_SIZE];

public:
    Camera(glm::vec4 _primaryEye, glm::vec4 _secondaryEye) 
    : primaryEye{_primaryEye}, secondaryEye{_secondaryEye}, primaryEyeActive{true} {}
    
    void render(const Scene&, const char* filename);

    void toggleActiveEye();
};

