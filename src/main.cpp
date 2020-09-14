#include <iostream>
#include <vector>
#include "../dependencies/glm/glm.hpp"

struct Color {
    double r, g, b;
};

struct Ray {
    glm::dvec4 start, end;
    Triangle target;
    Color color;
    double importance;    
};

struct Pixel {
    Color color;
    std::vector<Ray> rays;
};

class Triangle {
private:
    glm::dvec4 v1, v2, v3;
    glm::dvec3 normal;
    Color color;

public:
    Triangle(glm::dvec4 v1, glm::dvec4 v2, glm::dvec4 v3, Color color)
        : v1{v1}, v2{v2}, v3{v3}, color{color}
    {
        this->normal = glm::cross((v2.xyz-v1.xyz), (v3.xyz-v1.xyz));
    }

    glm::dvec3 rayIntersection(const Ray& ray) {
        using namespace glm;
        dvec3 ps = ray.start.xyz;
        dvec3 pe = ray.end.xyz;

        dvec3 T = ps - this->v1.xyz;
        dvec3 E1 = this->v2.xyz - this->v2.xyz;
        dvec3 E2 = this->v3.xyz - this->v1.xyz;
        dvec3 D = ps-pe;
        dvec3 P = cross(D, E2);
        dvec3 Q = cross(T, E1);

        dvec3 solution = (1/dot(P, E1)) * dvec3(dot(Q, E2), dot(P, T), dot(Q, D));

        return solution;
    }
};

class Scene {
private:
    std::vector<Triangle> walls;

public:
    Scene(std::vector<Triangle> walls)
        : walls{walls}
    { }

    void rayIntersection(const Ray &ray) {
        for (Triangle triangle : walls) {
            glm::dvec3 hit = triangle.rayIntersection(ray);
            // TODO: Handle a potentially bad hit
            if (hit.y >= 0 && hit.z >= 0 && hit.y + hit.z <= 1) {
                ray.end = (1 - hit.y - hit.z)*triangle.v1 + hit.y*triangle.v2 + hit.z*triangle.v3;
                ray.target = triangle;   
            }
        }
    }    
};

class Camera {
private:
    glm::dvec4 primaryEye, secondaryEye;
    bool primaryEyeActive;
    Pixel image [800][800];

public:
    Camera(glm::dvec4 primaryEye, glm::dvec4 secondaryEye) 
        : primaryEye{primaryEye}, secondaryEye{secondaryEye}, primaryEyeActive{true}
    {}

    void render() {
        // TODO: Do raytracing here! 😺
    }
};



int main() {
    using namespace glm;
    
    // Scene geometry
    // For final implementation:
    // * Floor and ceiling should be white 
    // * Each wall should be a unique color
    std::vector<Triangle> walls = {    
        // Ceiling
        Triangle{ dvec4(-3.0, 0.0, 5.0, 1.0), dvec4( 0.0, 6.0, 5.0, 1.0), dvec4( 5.0, 0.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4( 0.0, 5.0, 5.0, 1.0), dvec4(10.0, 6.0, 5.0, 1.0), dvec4( 5.0, 0.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(10.0, 6.0, 5.0, 1.0), dvec4(13.0, 0.0, 5.0, 1.0), dvec4( 5.0, 0.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(10.0,-6.0, 5.0, 1.0), dvec4(13.0, 0.0, 5.0, 1.0), dvec4( 5.0, 0.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4( 0.0,-6.0, 5.0, 1.0), dvec4(10.0,-6.0, 5.0, 1.0), dvec4( 5.0, 0.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(-3.0, 0.0, 5.0, 1.0), dvec4( 0.0,-6.0, 5.0, 1.0), dvec4( 5.0, 0.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        // Floor 
        Triangle{ dvec4(-3.0, 0.0,-5.0, 1.0), dvec4( 0.0, 6.0,-5.0, 1.0), dvec4( 5.0, 0.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4( 0.0, 5.0,-5.0, 1.0), dvec4(10.0, 6.0,-5.0, 1.0), dvec4( 5.0, 0.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(10.0, 6.0,-5.0, 1.0), dvec4(13.0, 0.0,-5.0, 1.0), dvec4( 5.0, 0.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(10.0,-6.0,-5.0, 1.0), dvec4(13.0, 0.0,-5.0, 1.0), dvec4( 5.0, 0.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4( 0.0,-6.0,-5.0, 1.0), dvec4(10.0,-6.0,-5.0, 1.0), dvec4( 5.0, 0.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(-3.0, 0.0,-5.0, 1.0), dvec4( 0.0,-6.0,-5.0, 1.0), dvec4( 5.0, 0.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        // Left and righ walls
        Triangle{ dvec4( 0.0,-6.0,-5.0, 1.0), dvec4( 0.0,-6.0, 5.0, 1.0), dvec4(10.0,-6.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4( 0.0,-6.0,-5.0, 1.0), dvec4( 0.0,-6.0, 5.0, 1.0), dvec4(10.0,-6.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4( 0.0, 6.0,-5.0, 1.0), dvec4( 0.0, 6.0, 5.0, 1.0), dvec4(10.0, 6.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4( 0.0, 6.0,-5.0, 1.0), dvec4( 0.0, 6.0, 5.0, 1.0), dvec4(10.0, 6.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        // Front walls
        Triangle{ dvec4(13.0, 0.0,-5.0, 1.0), dvec4(13.0, 0.0, 5.0, 1.0), dvec4(10.0, 6.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(13.0, 0.0,-5.0, 1.0), dvec4(13.0, 0.0, 5.0, 1.0), dvec4(10.0, 6.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(13.0, 0.0,-5.0, 1.0), dvec4(13.0, 0.0, 5.0, 1.0), dvec4(10.0,-6.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(13.0, 0.0,-5.0, 1.0), dvec4(13.0, 0.0, 5.0, 1.0), dvec4(10.0,-6.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        // Back wall
        Triangle{ dvec4(-3.0, 0.0,-5.0, 1.0), dvec4(-3.0, 0.0, 5.0, 1.0), dvec4( 0.0, 6.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(-3.0, 0.0,-5.0, 1.0), dvec4(-3.0, 0.0, 5.0, 1.0), dvec4( 0.0, 6.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(-3.0, 0.0,-5.0, 1.0), dvec4(-3.0, 0.0, 5.0, 1.0), dvec4( 0.0,-6.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(-3.0, 0.0,-5.0, 1.0), dvec4(-3.0, 0.0, 5.0, 1.0), dvec4( 0.0,-6.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } }
    };

    // Create the scene
    Scene scene = Scene{ walls };


    std::cout << "Welcome to CRaytracer\n";
    return 0;
}