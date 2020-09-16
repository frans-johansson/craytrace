#include "craytrace.hpp"

// TRIANGLE

glm::vec3 Triangle::rayIntersection(const Ray& ray) {
        using namespace glm;
        vec3 ps = ray.start.xyz();
        vec3 pe = ray.end.xyz();

        vec3 T = ps - this->v1.xyz();
        vec3 E1 = this->v2.xyz() - this->v1.xyz();
        vec3 E2 = this->v3.xyz() - this->v1.xyz();
        vec3 D = pe-ps;
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
            return;
        }
    }
} 

// CAMERA

void Camera::render(const Scene& scene, const char* filename)
{
    png::image<png::rgb_pixel> image(IMAGE_SIZE, IMAGE_SIZE);
    for (size_t y = 0; y < image.get_height(); ++y)
    {
        for (size_t x = 0; x < image.get_width(); ++x)
        {
            // TODO: Do raytracing here! 😺
            // Spawn a new ray going through the pixel
            glm::vec4 start = this->primaryEyeActive ? this->primaryEye : this->secondaryEye;
            glm::vec4 end = glm::vec4(0.0, ((float)x-401.0f+0.5f) * PIXEL_SIZE, ((float)y-401.0f+0.5f) * PIXEL_SIZE, 1.0);

            Ray ray = Ray{ start, end, nullptr, Color{ 0.0, 0.0, 0.0 }, 1.0 };
            
            // Find the intersecting triangle in the scene
            scene.rayIntersection(ray);

            // Get the color of the ray as R, G, and B
            png::byte r, g, b;
            if (ray.target) {
                r = ray.target->color.r * 255;
                g = ray.target->color.g * 255;
                b = ray.target->color.b * 255;
            } else {
                std::cout << "Ray did not have a target triangle\n";
                r = 255;
                g = 0;
                b = 255;
            }

            // Output color to image
            image[y][x] = png::rgb_pixel(r, g, b);
        }
    }
    image.write(filename);
}

void Camera::toggleActiveEye() {
    this->primaryEyeActive = !this->primaryEyeActive;
}