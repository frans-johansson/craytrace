#include "craytrace.hpp"

// TRIANGLE

glm::vec3 Triangle::rayIntersection(const Ray& ray) {
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

        vec3 solution = (1/dot(P, E1)) * vec3(dot(Q, E2), dot(P, T), dot(Q, D));

        return solution;
    }

// SCENE

void Scene::rayIntersection(Ray &ray) const {
    for (Triangle triangle : this->walls) {
        glm::vec3 hit = triangle.rayIntersection(ray);
        // std::cout << "u = " << hit.y << " v = " << hit.z << '\n';
        // TODO: Handle a potentially bad hit
        if (hit.y >= 0.0f && hit.z >= 0.0f && hit.y + hit.z <= 1.0f) {
            ray.end = (1.0f - hit.y - hit.z)*triangle.v1 + hit.y*triangle.v2 + hit.z*triangle.v3;
            ray.target = std::make_shared<Triangle>(triangle);
            ray.color = triangle.color;
            return;
        }
    }
} 

// CAMERA

Pixel& Camera::getImagePixel(size_t x, size_t y) {
    return (*image)[IMAGE_SIZE * y + x];
}

void Camera::render(const Scene& scene)
{
    for (size_t y = 0; y < IMAGE_SIZE; ++y)
    {
        for (size_t x = 0; x < IMAGE_SIZE; ++x)
        {
            // TODO: Do raytracing here! 😺
            // Spawn a new ray going through the pixel
            glm::vec4 start = this->primaryEyeActive ? this->primaryEye : this->secondaryEye;
            glm::vec4 pixelCoord = glm::vec4(0.0, ((float)x-401.0f+0.5f) * PIXEL_SIZE, ((float)y-401.0f+0.5f) * PIXEL_SIZE, 1.0);

            Ray ray{ start, (pixelCoord-start).xyz(), 1.0 };
            
            // Find the intersecting triangle in the scene
            scene.rayIntersection(ray);

            // Add ray to image pixel
            getImagePixel(x, y).rays.push_back(ray);
        }
    }

}

void Camera::create_image(const char* filename) {
    png::image<png::rgb_pixel> rgbImage(IMAGE_SIZE, IMAGE_SIZE);

    for (size_t y = 0; y < rgbImage.get_height(); ++y)
    {
        for (size_t x = 0; x < rgbImage.get_width(); ++x)
        {
            png::rgb_pixel pixelColor{ 0, 0, 0 };

            for (const Ray& ray : getImagePixel(x, y).rays)
            {
                // Get the color of the ray as R, G, and B
                pixelColor.red   += ray.color.r * 255 * ray.importance;
                pixelColor.green += ray.color.g * 255 * ray.importance;
                pixelColor.blue  += ray.color.b * 255 * ray.importance;
            }

            // Output color to image
            rgbImage[y][x] = pixelColor;
        }
    }

    rgbImage.write(filename);
}

void Camera::toggleActiveEye() {
    this->primaryEyeActive = !this->primaryEyeActive;
}