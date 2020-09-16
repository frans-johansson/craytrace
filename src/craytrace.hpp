#pragma once

#include <iostream>
#include <vector>
#define GLM_FORCE_SWIZZLE
#include "../dependencies/glm/glm.hpp"


struct Ray;

struct Color {
    double r, g, b;
};

class Triangle { 
private:
    glm::vec3 normal;
    Color color;

public:
    glm::vec4 v1, v2, v3;

    Triangle(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3, Color color)
        : v1{v1}, v2{v2}, v3{v3}, color{color}
    {
        this->normal = glm::cross((v2.xyz()-v1.xyz()), (v3.xyz()-v1.xyz()));
    } 

    glm::vec3 rayIntersection(const Ray& ray);
};

struct Ray {
    glm::vec4 start, end;
    Triangle target;
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
    Scene(std::vector<Triangle> walls)
    : walls{walls} { }

    void rayIntersection(Ray &ray);
};

class Camera {
private:
    glm::vec4 primaryEye, secondaryEye;
    bool primaryEyeActive;
    Pixel image [800][800];

public:
    Camera(glm::vec4 pridmaryEye, glm::vec4 secondaryEye) 
    : primaryEye{primaryEye}, secondaryEye{secondaryEye}, primaryEyeActive{true} {}
    
    void render();
};

