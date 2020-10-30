#include "dependencies.hpp"

// TRIANGLE

double Triangle::rayIntersection(const Ray& ray) {
    using namespace glm;
    dvec3 ps = ray.start;
    // dvec3 pe = ray.end;

    dvec3 T = ps - this->v1;
    dvec3 E1 = this->v2 - this->v1;
    dvec3 E2 = this->v3 - this->v1;
    // dvec3 D = pe-ps;
    dvec3 D = ray.direction;
    dvec3 P = cross(D, E2);
    dvec3 Q = cross(T, E1);

    dvec3 hit = (1/dot(P, E1)) * dvec3(dot(Q, E2), dot(P, T), dot(Q, D));

    if (hit.y < 0.0 || hit.z < 0.0 || hit.y + hit.z > 1.0 || hit.x < 0.0) {
        return -1.0;
    }

    return hit.x;
}

glm::dvec3 Triangle::calculateNormal(glm::dvec3 p) {
    return this->normal;
}

std::vector<Ray> Triangle::sampleShadowRays(glm::dvec3 start) {
    std::vector<Ray> rays;

    for (int i = 0; i < LIGHT_SAMPLES; ++i) {
        // Randomly select u and v coordinates
        double u = ((double) rand()) / RAND_MAX;
        double v = (1 - u) * ((double) rand()) / RAND_MAX;
        glm::dvec3 end = this->v1 * u + this->v2 * v + this->v3 * (1 - u - v);

        rays.emplace_back( Ray{ start, end } );
    }

    return rays;
}

// SPHERE

double Sphere::rayIntersection(const Ray& ray) {
    using namespace glm;
     
    double a = dot(ray.direction, ray.direction);
    double b = dot(2.0*ray.direction, ray.start - this->m);
    double c = dot(ray.start - this->m, ray.start - this->m) - this->radius * this->radius;

    double pq = (b/2.0)*(b/2.0) - a*c;

    if (pq < EPSILON)
        return -1.0;

    double dpos = -(b/2.0) + sqrt(pq);
    double dneg = -(b/2.0) - sqrt(pq);
    double d = glm::min(dpos, dneg);

    if (d < EPSILON)
        return  -1.0;

    return d;
 }

glm::dvec3 Sphere::calculateNormal(glm::dvec3 p) {
    // Träffpunkten - radien
    glm::dvec3 result = p - this->m;
    return glm::normalize(result);
}

std::vector<Ray> Sphere::sampleShadowRays(glm::dvec3 start) {
    return { Ray{ start, this->m } };
}