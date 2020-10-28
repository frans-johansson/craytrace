#pragma once
#include "dependencies.hpp"

// enum SurfaceType {
//     LAMBERTIAN,
//     OREN_NAYAR,
//     MIRROR,
//     TRANSPARENT,
//     EMITTER,
// };

struct SceneObject {
    std::unique_ptr<Material> material;

    SceneObject(std::unique_ptr<Material> _material)
    : material{std::move(_material)} {}

    virtual float rayIntersection(const Ray& ray) = 0;
    virtual glm::vec3 calculateNormal(glm::vec4 p) = 0;
    virtual std::vector<Ray> sampleShadowRays(glm::vec4 start) = 0;
};


struct Triangle : SceneObject { 
    glm::vec4 v1, v2, v3;
    glm::vec3 normal;

    Triangle(glm::vec4 _v1, glm::vec4 _v2, glm::vec4 _v3, std::unique_ptr<Material> _material)
        : SceneObject(std::move(_material)), v1{_v1}, v2{_v2}, v3{_v3}
    {
        this->normal = glm::normalize(glm::cross((v2.xyz()-v1.xyz()), (v3.xyz()-v1.xyz())));
    } 

    float rayIntersection(const Ray& ray);
    glm::vec3 calculateNormal(glm::vec4 p);
    std::vector<Ray> sampleShadowRays(glm::vec4 start);
};


// struct EmittingTriangle : Triangle {
//     double emittance;

//     EmittingTriangle(glm::vec4 _v1, glm::vec4 _v2, glm::vec4 _v3, Color _color, double _emittance)
//     : Triangle(_v1, _v2, _v3, _color, SurfaceType::EMITTER), emittance{_emittance} {}

//     std::vector<Ray> sampleLightRays(glm::vec4 start);
// };


struct Sphere : SceneObject {
    glm::vec4 m; //Middlepoint
    float radius;
    
    Sphere(glm::vec4 _m, float _radius, std::unique_ptr<Material> _material) 
        : SceneObject(std::move(_material)), m{_m}, radius{_radius} {}

    float rayIntersection(const Ray& ray);
    glm::vec3 calculateNormal(glm::vec4 p);
    std::vector<Ray> sampleShadowRays(glm::vec4 start);
    
    // Sphere ekv. 
};