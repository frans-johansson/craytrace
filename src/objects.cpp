#include "dependencies.hpp"

// SCENE OBJECT

Ray SceneObject::perfectReflection(const Ray& incoming) {
    return Ray{
        incoming.end,
        glm::reflect(incoming.direction, this->calculateNormal(incoming.end)),
        incoming.importance
    };
}

Ray SceneObject::diffuseReflection(const Ray& incoming) {
    // Local coordinate system
    using namespace glm;
    vec3 Z = this->calculateNormal(incoming.end);
    vec3 X = normalize(incoming.direction - dot(incoming.direction, Z) * Z);
    vec3 Y = cross(-X, Z);
    // Transformation matrix for local coordinates
    // float trans[] = {
    //     1.0f, 0.0f, 0.0f, -incoming.end.x,
    //     0.0f, 1.0f, 0.0f, -incoming.end.y,
    //     0.0f, 0.0f, 1.0f, -incoming.end.z,
    //     0.0f, 0.0f, 0.0f, 1.0f,
    // };
    float rot[] = {
        X.x,  X.y,  X.z,  0.0f,
        Y.x,  Y.y,  Y.z,  0.0f,
        Z.x,  Z.y,  Z.z,  0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };
    mat4 M = make_mat4(rot); 
    // Monte carlo method to randomly select ray direction in local hemispherical coordinates
    float phi = 2 * M_PI * rand() / RAND_MAX;
    float theta = asin(sqrt(((float) rand()) / RAND_MAX));
    // Cartesian local coordinates for the direction
    float x = cos(phi) * sin(theta);
    float y = sin(phi) * sin(theta);
    float z = cos(theta);
    vec4 localDir = vec4(x, y, z, 0.0f);
    // Transform direction to world coordinates
    vec3 worldDir = inverse(M) * localDir * M;

    return Ray {
        incoming.end,
        worldDir,
        incoming.importance * 0.8 // HACK: This value should not be hard coded like this
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