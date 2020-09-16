#include "craytrace.hpp"

// TRIANGLE

glm::vec3 Triangle::rayIntersection(const Ray& ray) {
        using namespace glm;
        vec3 ps = ray.start.xyz();
        vec3 pe = ray.end.xyz();

        vec3 T = ps - this->v1.xyz();
        vec3 E1 = this->v2.xyz() - this->v2.xyz();
        vec3 E2 = this->v3.xyz() - this->v1.xyz();
        vec3 D = ps-pe;
        vec3 P = cross(D, E2);
        vec3 Q = cross(T, E1);

        vec3 solution = (1/dot(P, E1)) * vec3(dot(Q, E2), dot(P, T), dot(Q, D));

        return solution;
    }

// SCENE

void Scene::rayIntersection(Ray &ray) {
    for (Triangle triangle : walls) {
        glm::vec3 hit = triangle.rayIntersection(ray);
        // TODO: Handle a potentially bad hit
        if (hit.y >= 0 && hit.z >= 0 && hit.y + hit.z <= 1) {
            ray.end = (1.0f - hit.y - hit.z)*triangle.v1 + hit.y*triangle.v2 + hit.z*triangle.v3;
            ray.target = triangle;   
        }
    }
} 

// CAMERA

void render()
{
    // TODO: Do raytracing here! 😺
}