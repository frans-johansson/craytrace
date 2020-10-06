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

float Triangle::rayIntersection(const Ray& ray) {
    using namespace glm;
    vec3 ps = ray.start.xyz();
    // vec3 pe = ray.end.xyz();

    vec3 T = ps - this->v1.xyz();
    vec3 E1 = this->v2.xyz() - this->v1.xyz();
    vec3 E2 = this->v3.xyz() - this->v1.xyz();
    // vec3 D = pe-ps;
    vec3 D = ray.direction;
    vec3 P = cross(D, E2);
    vec3 Q = cross(T, E1);

    vec3 hit = (1/dot(P, E1)) * vec3(dot(Q, E2), dot(P, T), dot(Q, D));

    if (hit.y < 0.0f || hit.z < 0.0f || hit.y + hit.z > 1.0f || hit.x < 0.00001f) {
        return -1.0;
    }

    return hit.x;
}

glm::vec3 Triangle::calculateNormal(glm::vec4 p) {
    return this->normal;
}


float Sphere::rayIntersection(const Ray& ray) {
    using namespace glm;
     
    float a = dot(ray.direction, ray.direction);
    float b = dot(2.0f*ray.direction, ray.start.xyz - this->m.xyz);
    float c = dot(ray.start.xyz - this->m.xyz, ray.start.xyz - this->m.xyz) - this->radius * this->radius;

    float pq = (b/2.0f)*(b/2.0f) - a*c;

    if (pq < 0.0)
        return -1.0;

    float dpos = -(b/2.0f) + sqrt(pq);
    float dneg = -(b/2.0f) - sqrt(pq);
    float d = abs(dpos) > abs(dneg) ? dneg : dpos;

    if (d < 0.00001f)
        return  -1.0;

    return d;
 }

glm::vec3 Sphere::calculateNormal(glm::vec4 p) {
    // Träffpunkten - radien
    glm::vec3 result = p.xyz - this->m.xyz;
    return result;
}