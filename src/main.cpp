#include <iostream>
#include <vector>

struct Vertex {
    double x, y, z, w;
};

struct Direction {
    double x, y, z;
};

struct Color {
    double r, g, b;
};

struct Ray {
    Vertex& start, end;
    Triangle& target;
    Color color;
    double importance;    
};

struct Pixel {
    Color color;
    std::vector<Ray&> rays;
};

class Triangle {
private:
    Vertex v1, v2, v3;
    Direction normal;
    Color color;

public:
    Triangle(Vertex v1, Vertex v2, Vertex v3, Color color)
        : v1{v1}, v2{v2}, v3{v3}, color{color}
    {
        // TODO: Calculate normal vector
    }

    Vertex rayIntersection(Ray ray) {
        // TODO: Implement Möller-Trumbore algorithm for intersections
        // TODO: Handle ray intersections outside of the triangle. What should we return?
    }
};

class Scene {
private:
    std::vector<Triangle> walls;

public:
    Scene(std::vector<Triangle> walls)
        : walls{walls}
    {
        // For final implementation:
        // * Floor and ceiling should be white 
        // * Each wall should be a unique color
    }

    Vertex rayIntersection(Ray ray) {
        for (Triangle triangle : walls) {
            Vertex hit = triangle.rayIntersection(ray);
            // TODO: Handle a potentially bad hit
            ray.end = hit;
            ray.target = triangle;
        }
    }    
};

class Camera {
private:
    Vertex primaryEye, secondaryEye;
    bool primaryEyeActive;
    Pixel image [800][800];

public:
    Camera(Vertex primaryEye, Vertex secondaryEye) 
        : primaryEye{primaryEye}, secondaryEye{secondaryEye}, primaryEyeActive{true}
    {}

    void render() {
        // TODO: Do raytracing here!x
    }
};



int main() {
    std::cout << "Welcome to CRaytracer\n";
    return 0;
}