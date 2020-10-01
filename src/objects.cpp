#include "dependencies.hpp"

// SCENE OBJECT

Ray SceneObject::perfectReflection(Ray incoming) {
    return Ray{
        incoming.end,
        glm::reflect(incoming.direction, this->calculateNormal(incoming.end)),
        incoming.importance
    };
}

// TRIANGLE

glm::vec3 Triangle::rayIntersection(std::shared_ptr<Ray> ray) {
    using namespace glm;
    vec3 ps = ray->start.xyz();
    // vec3 pe = ray->end.xyz();

    vec3 T = ps - this->v1.xyz();
    vec3 E1 = this->v2.xyz() - this->v1.xyz();
    vec3 E2 = this->v3.xyz() - this->v1.xyz();
    // vec3 D = pe-ps;
    vec3 D = ray->direction;
    vec3 P = cross(D, E2);
    vec3 Q = cross(T, E1);

    vec3 solution = (1/dot(P, E1)) * vec3(dot(Q, E2), dot(P, T), dot(Q, D));

    return solution;
}

glm::vec3 Triangle::calculateNormal(glm::vec4 p) {
    return this->normal;
}

/*
 glm::vec3 Sphere::rayIntersection(const Ray& r) {


 }

glm::vec3 Sphere::calculateNormal() {}


*/