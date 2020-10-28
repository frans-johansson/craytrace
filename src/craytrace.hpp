#pragma once
#include "dependencies.hpp"

const int IMAGE_SIZE = 800;
const float PIXEL_SIZE = 2.0f / IMAGE_SIZE;

struct Ray {
    // NOTE: end and target are not set until the ray has intersected a triangle
    glm::vec4 start = glm::vec4(0.0); 
    glm::vec4 end = glm::vec4(0.0);
    glm::vec3 direction;
    std::shared_ptr<SceneObject> target;
    Color color;
    double importance;
    double radiance = 0.0;
    int innerReflections = 0; 

    Ray(glm::vec4 _start, glm::vec3 _direction, double _importance)
    : start{_start}, direction{glm::normalize(_direction)}, color{BLACK}, importance{_importance} {}

    Ray(glm::vec4 _start, glm::vec4 _end)
    : start{_start}, end{_end}, direction{glm::normalize(end-start)}, color{BLACK}, importance{0.0} {}

    Color localLighting(const PointLight& light) const;
    void setEnd(float t);
};

struct Pixel {
    Color color;
    // std::vector<Ray> rays;
};

class Scene {
private:
    std::vector<std::shared_ptr<Sphere>> spheres;
    std::vector<std::shared_ptr<Triangle>> triangles;
    std::vector<std::shared_ptr<SceneObject>> areaLights;
    std::vector<PointLight> pointLights;

public:
    Scene(std::vector<std::shared_ptr<Triangle>> _walls)
    : triangles{_walls} {}

    Color traceRay(Ray& ray) const;

    std::shared_ptr<SceneObject> rayIntersection(Ray& ray) const;
    void addTetrahedron(float width, float height, glm::vec4 m, std::unique_ptr<Material> material);
    void addBox(float width, float height, float depth, glm::vec4 m, std::unique_ptr<Material> material);
    void addSphere(float radius, glm::vec4 m, std::unique_ptr<Material> material);
    void addAreaLight(std::shared_ptr<SceneObject> object);
    void addPointLight(PointLight pointLight);

    Color localLighting(const Ray& ray) const;
    bool rayOccluded(Ray& ray) const;
};

struct PointLight {
    glm::vec4 position;
    double intensity;
    Color color;
};

class Camera {
private:
    glm::vec4 primaryEye, secondaryEye;
    int samplesPerPixel;
    float subpixelSize;
    bool primaryEyeActive;
    std::array<Pixel, IMAGE_SIZE*IMAGE_SIZE>* image;
    double maxIntensity;
    double minIntensity;

public:
    Camera(glm::vec4 _primaryEye, glm::vec4 _secondaryEye, int _samplesPerPixel)
    : primaryEye{_primaryEye}, secondaryEye{_secondaryEye}, samplesPerPixel{_samplesPerPixel}, subpixelSize{PIXEL_SIZE / (float)glm::sqrt(_samplesPerPixel)}, primaryEyeActive{true}, maxIntensity{0.0} {
        image = new std::array<Pixel, IMAGE_SIZE*IMAGE_SIZE>();
        image->fill(Pixel{ BLACK });
    }
    
    Pixel& getImagePixel(size_t x, size_t y);
    void render(const Scene& scene, size_t x_lower, size_t x_upper, size_t y_lower, size_t y_upper);
    void create_image(const char* filename);
    void toggleActiveEye();
};
