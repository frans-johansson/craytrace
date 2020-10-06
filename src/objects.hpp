#pragma once
#include "dependencies.hpp"

enum SurfaceType {
    LAMBERTIAN,
    OREN_NAYAR,
    MIRROR,
    TRANSPARENT
};

struct SceneObject {
    Color color;
    float absorption;
    SurfaceType surface;

    virtual float rayIntersection(const Ray& ray) = 0;
    virtual glm::vec3 calculateNormal(glm::vec4 p) = 0;

    Ray perfectReflection(Ray incoming);
};

struct Triangle : SceneObject { 
    glm::vec4 v1, v2, v3;
    glm::vec3 normal;

    Triangle(glm::vec4 _v1, glm::vec4 _v2, glm::vec4 _v3, Color _color, SurfaceType _surface = SurfaceType::LAMBERTIAN)
        : v1{_v1}, v2{_v2}, v3{_v3}
    {
        this->color = _color;
        this->surface = _surface;
        this->normal = glm::normalize(glm::cross((v2.xyz()-v1.xyz()), (v3.xyz()-v1.xyz())));
    } 

    float rayIntersection(const Ray& ray);
    glm::vec3 calculateNormal(glm::vec4 p);
};


struct Sphere : SceneObject {
    glm::vec4 m; //Middlepoint
    float radius;
    
    Sphere(glm::vec4 _m, float _radius, Color _color , SurfaceType _surface = SurfaceType::LAMBERTIAN) 
        : m{_m}, radius{_radius}  {
            this->color = _color;
            this->surface = _surface;
        }

    float rayIntersection(const Ray& ray);
    glm::vec3 calculateNormal(glm::vec4 p);
    
    // Sphere ekv. 
};