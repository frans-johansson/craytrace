#include "dependencies.hpp"

// TRIANGLE

float Triangle::rayIntersection(const Ray& ray) {
    using namespace glm;
    vec3 ps = ray.start;
    // vec3 pe = ray.end;

    vec3 T = ps - this->v1;
    vec3 E1 = this->v2 - this->v1;
    vec3 E2 = this->v3 - this->v1;
    // vec3 D = pe-ps;
    vec3 D = ray.direction;
    vec3 P = cross(D, E2);
    vec3 Q = cross(T, E1);

    vec3 hit = (1/dot(P, E1)) * vec3(dot(Q, E2), dot(P, T), dot(Q, D));

    if (hit.y < 0.0 || hit.z < 0.0 || hit.y + hit.z > 1.0f || hit.x < 0.0) {
        return -1.0;
    }

    return hit.x;
}

glm::vec3 Triangle::calculateNormal(glm::vec3 p) {
    return this->normal;
}

std::vector<Ray> Triangle::sampleShadowRays(glm::vec3 start) {
    std::vector<Ray> rays;

    for (int i = 0; i < LIGHT_SAMPLES; ++i) {
        // Randomly select u and v coordinates
        float u = ((float) rand()) / RAND_MAX;
        float v = (1 - u) * ((float) rand()) / RAND_MAX;
        glm::vec3 end = this->v1 * u + this->v2 * v + this->v3 * (1 - u - v);

        rays.emplace_back( Ray{ start, end } );
    }

    return rays;
}

// SPHERE

float Sphere::rayIntersection(const Ray& ray) {
    using namespace glm;
     
    float a = dot(ray.direction, ray.direction);
    float b = dot(2.0f*ray.direction, ray.start - this->m);
    float c = dot(ray.start - this->m, ray.start - this->m) - this->radius * this->radius;

    float pq = (b/2.0f)*(b/2.0f) - a*c;

    if (glm::sign(pq) < 0.0)
        return -1.0;

    float dpos = -(b/2.0f) + sqrt(pq);
    float dneg = -(b/2.0f) - sqrt(pq);
    float d = glm::min(dpos, dneg);

    if (glm::sign(d) < 0.0)
        return  -1.0;

    return d;
 }

glm::vec3 Sphere::calculateNormal(glm::vec3 p) {
    // Träffpunkten - radien
    glm::vec3 result = p - this->m;
    return glm::normalize(result);
}

std::vector<Ray> Sphere::sampleShadowRays(glm::vec3 start) {
    return { Ray{ start, this->m } };
}