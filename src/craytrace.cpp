#include "dependencies.hpp"

// SCENE

Color Scene::traceRay(Ray& ray) const {
    auto target = this->rayIntersection(ray);

    switch (target->surface)
    {
    case SurfaceType::LAMBERTIAN:
    {
        // Local lighting
        ray.color = target->color * this->localLighting(ray);
        // Russian roulette scheme to determine if the ray is reflected or killed
        float absorb = ((float) rand()) / RAND_MAX;
        if (absorb < target->absorption) {
            Ray reflected = target->diffuseReflection(ray);
            ray.color += this->traceRay(reflected) * reflected.importance;
            ray.color /= target->absorption;
        }
        break;
    }
    case SurfaceType::OREN_NAYAR:
    {
        /* TODO: Unimplemented */
        break;
    }
    case SurfaceType::TRANSPARENT:
    {
        /* TODO: Unimplemented */
        /* Note no break, should continue to MIRROR */ 
    }
    case SurfaceType::MIRROR:
    {
        Ray reflected = target->perfectReflection(ray);
        ray.color = this->traceRay(reflected) * reflected.importance + this->localLighting(ray);
        break;
    }
    default:
        break;
    }

    // ray.color = this->localLighting(ray) * target->color;
    
    // if (ray.importance < 0.1) {
    //     return ray.color;
    // }   

    // if (target->surface == SurfaceType::TRANSPARENT)
    // {
    //     // TODO: Unimplemented
    //     // // Calculate reflection and refraction coefficients
        
    //     // // Handle refraction
    //     // // auto refracted = target->perfectRefraction(ray);

    //     // // Handle reflection
    //     // auto reflected = std::make_shared<Ray>(target->perfectReflection(ray));
    //     // ray.color = this->traceRay(reflected); // Add local lightning
    // } 
    // else if (target->surface == SurfaceType::MIRROR)
    // {
    //     Ray reflected = target->perfectReflection(ray);
    //     reflected.importance *= 0.8;

    //     ray.color += this->traceRay(reflected) * reflected.importance;
    // }

    return ray.color;
}

std::shared_ptr<SceneObject> Scene::rayIntersection(Ray& ray) const {
    float nearest = FLT_MAX;
    ray.target = std::make_shared<Triangle>(Triangle{
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), MAGENTA
    });

    // TODO: Make this one single loop
    for (auto triangle : this->triangles) {
        float hit = triangle->rayIntersection(ray);

        // if (glm::dot(triangle->calculateNormal(ray.end), ray.direction) > 0.0f)
        //     continue;

        if (hit > 0.00001f && hit < nearest) {
            nearest = hit;
            ray.setEnd(hit);
            ray.target = triangle;
        }
    }

    for (auto sphere : this->spheres) {
        float hit = sphere->rayIntersection(ray);

        // if (glm::dot(sphere->calculateNormal(ray.end), ray.direction) > 0.0f)
        //     continue;

        if (hit > 0.00001f && hit < nearest) {
            ray.setEnd(hit);
            nearest = hit;
            ray.target = sphere;
        }
    }

    return ray.target;
}

void Scene::addTetrahedron(float width, float height, glm::vec4 m, Color color, SurfaceType surface) {
    using namespace glm;
    float offset = height/sqrt(2); 
    vec4 a = m + vec4(0.0, 0.0, height, 0.0);
    vec4 b = m + vec4(-width, 0.0, 0.0, 0.0);
    vec4 c = m + vec4(offset, offset, 0.0, 0.0);
    vec4 d = m + vec4(offset, -offset, 0.0, 0.0);

    std::vector<std::shared_ptr<Triangle>> tris{
        std::make_shared<Triangle>(Triangle{ b, a, c, color, surface }),
        std::make_shared<Triangle>(Triangle{ d, a, b, color, surface }),
        std::make_shared<Triangle>(Triangle{ c, a, d, color, surface }),
        std::make_shared<Triangle>(Triangle{ d, b, c, color, surface })
    };

    this->triangles.insert(this->triangles.end(), tris.begin(), tris.end());
}

void Scene::addSphere(float radius, glm::vec4 m, Color sphereColor, SurfaceType surface)
{
    using namespace glm;
    this->spheres.push_back(std::make_shared<Sphere>(Sphere{m, radius, sphereColor, surface}));
}

void Scene::addPointLight(PointLight pointLight) {
    this->pointLights.push_back(pointLight);   
}

Color Scene::localLighting(const Ray& ray) const {
    Color totalLight = Color{ 0.0, 0.0, 0.0 };
    for (const PointLight& light : this->pointLights) {
        Ray lightRay = Ray{ light.position, ray.end };
        lightRay.target = ray.target;

        if (notOccluded(lightRay))
            totalLight += ray.localLighting(light);
    }
    return totalLight;
}

bool Scene::notOccluded(Ray& ray) const {
    // Save inital endpoint
    // glm::vec4 initial = ray.end;
    auto initial = ray.target;
    // Calculate potential closer intersections causing occlusion
    auto hit = rayIntersection(ray);

    // float distance = glm::abs(glm::distance(initial, ray.end));
    return initial == hit;
}

// RAY

Color Ray::localLighting(const PointLight& light) const {
    using namespace glm;
    vec3 dirToLight = light.position - this->end;
    double lightAmount = max(dot(normalize(dirToLight), this->target->calculateNormal(this->end)), 0.0f);
    return light.color * light.intensity * lightAmount / sqrt(length(dvec3(dirToLight)));
}

void Ray::setEnd(float t) {
    this->end = glm::vec4(this->start.xyz + this->direction * t, 1.0);
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
            Pixel& pixel = getImagePixel(x, y); 

            for (int i = 0; i < this->samplesPerPixel; ++i) {
                // Subpixel coordinates
                float xi = ((float)x-401.0f) * PIXEL_SIZE + (i % 3) * this->subpixelSize;
                float yi = ((float)y-401.0f) * PIXEL_SIZE + glm::floor(i/3) * this->subpixelSize;
                // Random offsets into the subpixel
                float dx = this->subpixelSize * ((float) rand()) / RAND_MAX;
                float dy = this->subpixelSize * ((float) rand()) / RAND_MAX;
                // Spawn a new ray from the active eye point going through the subpixel coordinate
                glm::vec4 pixelCoord = glm::vec4(0.0, -(xi + dx), -(yi + dy) , 1.0);
                glm::vec4 start = this->primaryEyeActive ? this->primaryEye : this->secondaryEye;
                Ray ray = Ray{ start, (pixelCoord-start).xyz(), 1.0/samplesPerPixel };

                pixel.color += scene.traceRay(ray) * ray.importance;
            }

            maxIntensity = glm::max(maxIntensity, pixel.color.r);
            maxIntensity = glm::max(maxIntensity, pixel.color.g);
            maxIntensity = glm::max(maxIntensity, pixel.color.b);
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

            // Get the color of the pixel and transform to R, G, and B
            // TODO: Handle with operator overloading on Color instead?
            Color color = getImagePixel(x, y).color;
            pixelColor.red   += 255 * pow(color.r / maxIntensity, 0.4);
            pixelColor.green += 255 * pow(color.g / maxIntensity, 0.4);
            pixelColor.blue  += 255 * pow(color.b / maxIntensity, 0.4);

            // Output color to image
            rgbImage[y][x] = pixelColor;
        }
    }

    rgbImage.write(filename);
}

void Camera::toggleActiveEye() {
    this->primaryEyeActive = !this->primaryEyeActive;
}