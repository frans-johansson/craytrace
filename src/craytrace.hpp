#pragma once
#include "dependencies.hpp"

const int IMAGE_SIZE = 800;
const float PIXEL_SIZE = 2.0f / IMAGE_SIZE;

struct Ray {
    // NOTE: end and target are not set until the ray has intersected a triangle
    glm::vec4 start, end;
    glm::vec3 direction;
    std::shared_ptr<SceneObject> target;
    Color color;
    double importance;

    Ray(glm::vec4 _start, glm::vec3 _direction, double _importance)
    : start{_start}, direction{_direction}, color{Color{0.0, 0.0, 0.0}}, importance{_importance} {}
};

struct Pixel {
    Color color;
    std::vector<Ray> rays;
};


class Scene {
private:
    std::vector<Triangle> walls;

public:
    Scene(std::vector<Triangle> _walls)
    : walls{_walls} { }

    void rayIntersection(Ray &ray) const;
    void addTetrahedron(float width, float height, glm::vec4 m, Color color);
};

class Camera {
private:
    glm::vec4 primaryEye, secondaryEye;
    bool primaryEyeActive;
    std::array<Pixel, IMAGE_SIZE*IMAGE_SIZE>* image;

public:
    Camera(glm::vec4 _primaryEye, glm::vec4 _secondaryEye, size_t _samplesPerPixel)
    : primaryEye{_primaryEye}, secondaryEye{_secondaryEye}, primaryEyeActive{true} {
        image = new std::array<Pixel, IMAGE_SIZE*IMAGE_SIZE>();
        image->fill(Pixel{ Color{ 0.0, 0.0, 0.0 }, std::vector<Ray>() });
    }
    
    Pixel& getImagePixel(size_t x, size_t y);

    void render(const Scene&);
    void create_image(const char* filename);
    void toggleActiveEye();
};

