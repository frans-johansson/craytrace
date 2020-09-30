#pragma once
#include "dependencies.hpp"

struct SceneObject {
    Color color;
    float reflectiveness = 0.0f;
    float transparency = 0.0f;

    virtual glm::vec3 rayIntersection(std::shared_ptr<Ray> ray) = 0;
    virtual glm::vec3 calculateNormal(glm::vec4 p) = 0;

    Ray perfectReflection(std::shared_ptr<Ray> incoming);
};

struct Triangle : SceneObject { 
    glm::vec4 v1, v2, v3;
    glm::vec3 normal;

    Triangle(glm::vec4 _v1, glm::vec4 _v2, glm::vec4 _v3, Color _color, float _reflectiveness = 0.0f)
        : v1{_v1}, v2{_v2}, v3{_v3}
    {
        this->color = _color;
        this->reflectiveness = _reflectiveness;
        this->normal = glm::normalize(glm::cross((v2.xyz()-v1.xyz()), (v3.xyz()-v1.xyz())));
    } 

    glm::vec3 rayIntersection(std::shared_ptr<Ray> ray);
    glm::vec3 calculateNormal(glm::vec4 p);
};

struct Sphere : SceneObject {
    glm::vec3 m; //Middlepoint
    double radius;
    
    Sphere(glm::vec3 _m, double _radius, Color _color) 
        : m{_m}, radius{_radius}  {
            this->color = _color;
        }
    
    // Sphere ekv. 
};