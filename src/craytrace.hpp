#pragma once
#include "dependencies.hpp"

const int IMAGE_SIZE = 800;
const double PIXEL_SIZE = 2.0 / IMAGE_SIZE;

struct Ray {
    // NOTE: end and target are not set until the ray has intersected a triangle
    glm::dvec3 start = glm::dvec3(0.0); 
    glm::dvec3 end = glm::dvec3(0.0);
    glm::dvec3 direction;
    std::shared_ptr<SceneObject> target;
    Color color;
    double importance;
    double radiance = 0.0;
    int innerReflections = 0; 
    int depth = 1;

    Ray(glm::dvec3 _start, glm::dvec3 _direction, double _importance)
    : start{_start}, direction{glm::normalize(_direction)}, color{BLACK}, importance{_importance} {}

    Ray(glm::dvec3 _start, glm::dvec3 _end)
    : start{_start}, end{_end}, direction{glm::normalize(end-start)}, color{BLACK}, importance{0.0} {}

    Color localLighting(const PointLight& light) const;
    void setEnd(double t);
    glm::dvec3 offsetEndPoint() const;
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
    void addTetrahedron(double width, double height, glm::dvec3 m, std::unique_ptr<Material> material);
    void addBox(double width, double height, double depth, glm::dvec3 m, std::unique_ptr<Material> material);
    void addSphere(double radius, glm::dvec3 m, std::unique_ptr<Material> material);
    void addAreaLight(std::shared_ptr<SceneObject> object);
    void addPointLight(PointLight pointLight);

    Color localLighting(const Ray& ray) const;
    bool rayOccluded(Ray& ray) const;
};

struct PointLight {
    glm::dvec3 position;
    double intensity;
    Color color;
};

class Camera {
private:
    glm::dvec3 primaryEye, secondaryEye;
    int samplesPerPixel;
    double subpixelSize;
    bool primaryEyeActive;
    std::array<Pixel, IMAGE_SIZE*IMAGE_SIZE>* image;
    double maxIntensity;
    double minIntensity;

public:
    Camera(glm::dvec3 _primaryEye, glm::dvec3 _secondaryEye, int _samplesPerPixel)
    : primaryEye{_primaryEye}, secondaryEye{_secondaryEye}, samplesPerPixel{_samplesPerPixel}, subpixelSize{PIXEL_SIZE / (double)glm::sqrt(_samplesPerPixel)}, primaryEyeActive{true}, maxIntensity{0.0} {
        image = new std::array<Pixel, IMAGE_SIZE*IMAGE_SIZE>();
        image->fill(Pixel{ BLACK });
    }
    
    Pixel& getImagePixel(size_t x, size_t y);
    void render(const Scene& scene, size_t x_lower, size_t x_upper, size_t y_lower, size_t y_upper);
    void create_image(const char* filename);
    void toggleActiveEye();
};
