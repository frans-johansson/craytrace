#include "dependencies.hpp"

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

    if (hit.y < 0.0 || hit.z < 0.0 || hit.y + hit.z > 1.0f || hit.x < 0.0) {
        return -1.0;
    }

    // auto test1 = (1-hit.y-hit.z)*this->v1 + hit.y*this->v2 + hit.z*this->v3;
    // auto test2 = ray.start + vec4(ray.direction, 0.0) * hit.x;
    return hit.x;
}

glm::vec3 Triangle::calculateNormal(glm::vec4 p) {
    return this->normal;
}

std::vector<Ray> Triangle::sampleShadowRays(glm::vec4 start) {
    std::vector<Ray> rays;

    for (int i = 0; i < LIGHT_SAMPLES; ++i) {
        // Randomly select u and v coordinates
        float u = ((float) rand()) / RAND_MAX;
        float v = (1 - u) * ((float) rand()) / RAND_MAX;
        glm::vec4 end = this->v1 * u + this->v2 * v + this->v3 * (1 - u - v);

        rays.emplace_back( Ray{ start, end } );
    }

    return rays;
}

// SPHERE

float Sphere::rayIntersection(const Ray& ray) {
    using namespace glm;
     
    float a = dot(ray.direction, ray.direction);
    float b = dot(2.0f*ray.direction, ray.start.xyz - this->m.xyz);
    float c = dot(ray.start.xyz - this->m.xyz, ray.start.xyz - this->m.xyz) - this->radius * this->radius;

    float pq = (b/2.0f)*(b/2.0f) - a*c;

    if (pq < EPSILON)
        return -1.0;

    float dpos = -(b/2.0f) + sqrt(pq);
    float dneg = -(b/2.0f) - sqrt(pq);
    float d = glm::min(dpos, dneg);

    if (d < EPSILON)
        return  -1.0;

    return d;
 }

glm::vec3 Sphere::calculateNormal(glm::vec4 p) {
    // Träffpunkten - radien
    glm::vec3 result = p.xyz - this->m.xyz;
    return glm::normalize(result);
}

std::vector<Ray> Sphere::sampleShadowRays(glm::vec4 start) {
    return { Ray{ start, this->m } };
}