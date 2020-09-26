#include "dependencies.hpp"

// SCENE

void Scene::rayIntersection(Ray &ray) const {
    // 1. Iterate over all triangels in the scene
    // 2. Iterate over all spheres in the scene
    // 3. Return the best hit from 1 and 2 

    float nearest = FLT_MAX;
    glm::vec3 bestHit;
    Triangle bestTriangle{ glm::vec4(), glm::vec4(), glm::vec4(), Color{ 1.0, 0.0, 1.0 } };

    for (Triangle triangle : this->walls) {
        glm::vec3 hit = triangle.rayIntersection(ray);
        // std::cout << "u = " << hit.y << " v = " << hit.z << '\n';
        // TODO: Handle a potentially bad hit
        if (hit.x < 0)
            continue;

        if (hit.y >= 0.0f && hit.z >= 0.0f && hit.y + hit.z <= 1.0f && hit.x < nearest) {
            bestHit = hit;
            bestTriangle = triangle;
            nearest = hit.x;
        }
    }
    ray.end = (1.0f - bestHit.y - bestHit.z)*bestTriangle.v1 + bestHit.y*bestTriangle.v2 + bestHit.z*bestTriangle.v3;
    ray.target = std::make_shared<Triangle>(bestTriangle);
    ray.color = bestTriangle.color;
}

void Scene::addTetrahedron(float width, float height, glm::vec4 m, Color color) {
    using namespace glm;
    float offset = height/sqrt(2); 
    vec4 a = m + vec4(0.0, 0.0, height, 0.0);
    vec4 b = m + vec4(-width, 0.0, 0.0, 0.0);
    vec4 c = m + vec4(offset, offset, 0.0, 0.0);
    vec4 d = m + vec4(offset, -offset, 0.0, 0.0);

    std::vector<Triangle> tris{
        Triangle{ b, a, c, color },
        Triangle{ d, a, b, color },
        Triangle{ c, a, d, color },
        Triangle{ d, b, c, color }
    };

    this->walls.insert(this->walls.end(), tris.begin(), tris.end());
}

/*void Scene::addSphere(float radius, vec4 m, Color sphereColor)
{
    using namespace ::glm;
    
}*/

void Scene::addPointLight(PointLight pointLight) {
    this->pointLights.push_back(pointLight);   
}

Color Scene::localLighting(Ray& ray) const {
    Color totalLight = Color{ 0.0, 0.0, 0.0 };
    for (const PointLight& light : this->pointLights) {
        totalLight += ray.localLighting(light);
    }
    return totalLight;
}

// RAY

Color Ray::localLighting(const PointLight& light) {
    using namespace glm;
    vec3 dirToLight = light.position - this->end;
    double lightAmount = max(dot(normalize(dirToLight), target->calculateNormal(this->end)), 0.0f);
    return light.color * light.intensity * lightAmount / sqrt(length(dvec3(dirToLight)));
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
                Ray ray{ start, (pixelCoord-start).xyz(), 1.0/samplesPerPixel };
                
                // 5. Find the intersecting triangle or object in the scene and add to ray tree
                scene.rayIntersection(ray);

                // 6. More rays based on the inital ray's target object, add to ray tree and refer to index of parent/children

                // 7. Traverse ray tree backwards and calculate final ray's color value
                // Calculate local lighting (done for each step of 7)
                // TODO: Shadow rays when calculating local light 💡
                Color localLight = scene.localLighting(ray);

                // Add ray to image pixel (TODO: Remove local light contribution here 🤷‍♀️)
                pixel.color += (localLight + ray.color) * ray.importance;
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
            pixelColor.red   += 255 * pow(color.r / maxIntensity, 0.7);
            pixelColor.green += 255 * pow(color.g / maxIntensity, 0.7);
            pixelColor.blue  += 255 * pow(color.b / maxIntensity, 0.7);

            // Output color to image
            rgbImage[y][x] = pixelColor;
        }
    }

    rgbImage.write(filename);
}

void Camera::toggleActiveEye() {
    this->primaryEyeActive = !this->primaryEyeActive;
}