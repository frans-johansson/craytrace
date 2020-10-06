#include "dependencies.hpp"

// SCENE

Color Scene::traceRay(std::shared_ptr<Ray> ray) const {
    auto target = this->rayIntersection(ray);

    // TODO: Material properties to determine how much local light contributes to the ray color 🐱‍👤
    ray->color = this->localLighting(ray) * target->color;
    
    if (ray->importance < 0.1) {
        return ray->color;
    }

    if (target->transparency > 0.0f)
    {
        // TODO: Unimplemented
        // // Calculate reflection and refraction coefficients
        
        // // Handle refraction
        // // auto refracted = target->perfectRefraction(ray);

        // // Handle reflection
        // auto reflected = std::make_shared<Ray>(target->perfectReflection(ray));
        // ray->color = this->traceRay(reflected); // Add local lightning
    } 
    else if (target->reflectiveness > 0.0f)
    {
        auto reflected = std::make_shared<Ray>(target->perfectReflection(*ray));
        reflected->importance *= 0.8;

        // TODO: Material properties to determine how much local light contributes to the ray color 🐱‍👤
        ray->color += this->traceRay(reflected) * reflected->importance;
    } 

    return ray->color;
}

std::shared_ptr<SceneObject> Scene::rayIntersection(std::shared_ptr<Ray> ray) const {
    float nearest = FLT_MAX;
    ray->target = std::make_shared<Triangle>(Triangle{
        glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), MAGENTA
    });

    // TODO: Make this one single loop
    for (auto triangle : this->triangles) {
        float hit = triangle->rayIntersection(ray);

        if (glm::dot(triangle->calculateNormal(ray->end), ray->direction) > 0.0f)
            continue;

        if (hit > 0.00001f && hit < nearest) {
            nearest = hit;
            ray->setEnd(hit);
            ray->target = triangle;
        }
    }

    for (auto sphere : this->spheres) {
        float hit = sphere->rayIntersection(ray);
        
        // if (glm::dot(sphere->calculateNormal(ray->end), ray->direction) > 0.0f)
        //     continue;    

        if (hit > 0.00001f && hit < nearest) {
            nearest = hit;
            ray->setEnd(hit);
            ray->target = sphere;
        }
    }

    return ray->target;
}

void Scene::addTetrahedron(float width, float height, glm::vec4 m, Color color, float reflectiveness) {
    using namespace glm;
    float offset = height/sqrt(2); 
    vec4 a = m + vec4(0.0, 0.0, height, 0.0);
    vec4 b = m + vec4(-width, 0.0, 0.0, 0.0);
    vec4 c = m + vec4(offset, offset, 0.0, 0.0);
    vec4 d = m + vec4(offset, -offset, 0.0, 0.0);

    std::vector<std::shared_ptr<Triangle>> tris{
        std::make_shared<Triangle>(Triangle{ b, a, c, color, reflectiveness }),
        std::make_shared<Triangle>(Triangle{ d, a, b, color, reflectiveness }),
        std::make_shared<Triangle>(Triangle{ c, a, d, color, reflectiveness }),
        std::make_shared<Triangle>(Triangle{ d, b, c, color, reflectiveness })
    };

    this->triangles.insert(this->triangles.end(), tris.begin(), tris.end());
}

void Scene::addSphere(float radius, glm::vec4 m, Color sphereColor, float reflectiveness = 0.0f)
{
    using namespace glm;
    this->spheres.push_back(std::make_shared<Sphere>(Sphere{m, radius, sphereColor, reflectiveness}));
}

void Scene::addPointLight(PointLight pointLight) {
    this->pointLights.push_back(pointLight);   
}

Color Scene::localLighting(std::shared_ptr<Ray> ray) const {
    Color totalLight = Color{ 0.0, 0.0, 0.0 };
    for (const PointLight& light : this->pointLights) {
        auto lightRay = std::make_shared<Ray>(Ray{ light.position, ray->end });

        // TOFIX: This seems to be causing errors when working with spheres :/
        // if (notOccluded(lightRay))
            totalLight += ray->localLighting(light);
    }
    return totalLight;
}

bool Scene::notOccluded(std::shared_ptr<Ray> ray) const {
    // Save inital endpoint
    glm::vec4 initial = ray->end;
    // Calculate potential closer intersections causing occlusion
    rayIntersection(ray);

    float distance = glm::abs(glm::distance(initial, ray->end));
    return distance < 0.00001f;
}

// RAY

Color Ray::localLighting(const PointLight& light) {
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
    // 1. Define ray tree as vector
    // 2. Iterate over all pixels in the image
    for (size_t y = 0; y < IMAGE_SIZE; ++y)
    {
        for (size_t x = 0; x < IMAGE_SIZE; ++x)
        {
            // Get a reference to the current pixel
            Pixel& pixel = getImagePixel(x, y); 

            // 3. Iterate over all subsamples of the pixel
            for (int i = 0; i < this->samplesPerPixel; ++i) {
                // 4. Spawn a new ray going through the pixel from the camera
                glm::vec4 start = this->primaryEyeActive ? this->primaryEye : this->secondaryEye;
                glm::vec4 pixelCoord = glm::vec4(0.0, -((float)x-401.0f+0.5f) * PIXEL_SIZE, -((float)y-401.0f+0.5f) * PIXEL_SIZE, 1.0);
                auto ray = std::make_shared<Ray>(Ray{ start, glm::normalize((pixelCoord-start).xyz()), 1.0/samplesPerPixel });

                // // 5. Find the intersecting triangle or object in the scene and add to ray tree
                // scene.rayIntersection(ray);

                // // 6. More rays based on the inital ray's target object, add to ray tree and refer to index of parent/children
                // if (ray->target->reflectiveness > 0.0) {
                //     Ray reflected = ray->target->perfectReflection(ray);
                // }

                // // 7. Traverse ray tree backwards and calculate final ray's color value
                // // Calculate local lighting (done for each step of 7)
                // // TODO: Shadow rays when calculating local light 💡
                // Color localLight = scene.localLighting(ray);

                // Add ray to image pixel (TODO: Remove local light contribution here 🤷‍♀️)
                pixel.color += scene.traceRay(ray) * ray->importance;
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