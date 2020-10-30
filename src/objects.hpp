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

    virtual double rayIntersection(const Ray& ray) = 0;
    virtual glm::dvec3 calculateNormal(glm::dvec3 p) = 0;
    virtual std::vector<Ray> sampleShadowRays(glm::dvec3 start) = 0;
};


struct Triangle : SceneObject { 
    glm::dvec3 v1, v2, v3;
    glm::dvec3 normal;

    Triangle(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, std::unique_ptr<Material> _material)
        : SceneObject(std::move(_material)), v1{_v1}, v2{_v2}, v3{_v3}
    {
        this->normal = glm::normalize(glm::cross((v2-v1), (v3-v1)));
    } 

    double rayIntersection(const Ray& ray);
    glm::dvec3 calculateNormal(glm::dvec3 p);
    std::vector<Ray> sampleShadowRays(glm::dvec3 start);
};


// struct EmittingTriangle : Triangle {
//     double emittance;

//     EmittingTriangle(glm::dvec3 _v1, glm::dvec3 _v2, glm::dvec3 _v3, Color _color, double _emittance)
//     : Triangle(_v1, _v2, _v3, _color, SurfaceType::EMITTER), emittance{_emittance} {}

//     std::vector<Ray> sampleLightRays(glm::dvec3 start);
// };


struct Sphere : SceneObject {
    glm::dvec3 m; //Middlepoint
    double radius;
    
    Sphere(glm::dvec3 _m, double _radius, std::unique_ptr<Material> _material) 
        : SceneObject(std::move(_material)), m{_m}, radius{_radius} {}

    double rayIntersection(const Ray& ray);
    glm::dvec3 calculateNormal(glm::dvec3 p);
    std::vector<Ray> sampleShadowRays(glm::dvec3 start);
    
    // Sphere ekv. 
};