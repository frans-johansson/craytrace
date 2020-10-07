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

    SceneObject(Color _color, SurfaceType _surface)
    : color{_color}, surface{_surface} {
        this->absorption = (float) 0.9 * glm::length(color) / glm::length(WHITE);
    }

    virtual float rayIntersection(const Ray& ray) = 0;
    virtual glm::vec3 calculateNormal(glm::vec4 p) = 0;

    Ray perfectReflection(const Ray& incoming);
    Ray diffuseReflection(const Ray& incoming);
};


struct Triangle : SceneObject { 
    glm::vec4 v1, v2, v3;
    glm::vec3 normal;

    Triangle(glm::vec4 _v1, glm::vec4 _v2, glm::vec4 _v3, Color _color, SurfaceType _surface = SurfaceType::LAMBERTIAN)
        : SceneObject(_color, _surface), v1{_v1}, v2{_v2}, v3{_v3}
    {
        this->normal = glm::normalize(glm::cross((v2.xyz()-v1.xyz()), (v3.xyz()-v1.xyz())));
    } 

    float rayIntersection(const Ray& ray);
    glm::vec3 calculateNormal(glm::vec4 p);
};


struct Sphere : SceneObject {
    glm::vec4 m; //Middlepoint
    float radius;
    
    Sphere(glm::vec4 _m, float _radius, Color _color , SurfaceType _surface = SurfaceType::LAMBERTIAN) 
        : SceneObject(_color, _surface), m{_m}, radius{_radius} {}

    float rayIntersection(const Ray& ray);
    glm::vec3 calculateNormal(glm::vec4 p);
    
    // Sphere ekv. 
};