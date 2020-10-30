#include "dependencies.hpp"

// switch (target->material->reflectionType)
    // {
    // case ReflectionType::DIFFUSE:
    // {
    //     // Russian roulette scheme to determine if the ray is reflected or killed
    //     if (target->material->rayReflected()) {
    //         Ray reflected = target->diffuseReflection(ray);
    //         ray.color += this->traceRay(reflected);
    //         // ray.color /= target->material->absorption;
    //     }
    //     ray.color += materialColor;
    //     localLighting *= LOCAL_LIGHTING_CONTRIBUTION;
    //     break;
    // }
    // case ReflectionType::TRANSPARENT:
    // {
    //     /* TODO: Unimplemented */
    //     break;
    // }
    // case ReflectionType::MIRROR:
    // {
    //     localLighting *= glm::pow(LOCAL_LIGHTING_CONTRIBUTION, PIXEL_SAMPLES / 2.0);
    //     break;
    // }
    // case ReflectionType::EMITTER:
    // {
    //     ray.color = materialColor;
    //     localLighting = BLACK;
    //     break;
    // }
    // default:
    //     break;
    // }

// SCENE

Color Scene::traceRay(Ray& ray) const {
    auto target = this->rayIntersection(ray);

    Color localColor;
    // Color localLightingColor = WHITE;
    double localLightingRadiance;

    Color incomingColor = BLACK;
    double incomingImportance = 0.0;
    double incomingRadiance = 0.0;
    
    auto branchingRays = target->material->nextRayBranch(ray);

    // Is target an emittor?
    if (target->material->emittance > 0.0) {
        // localLightingRadiance = target->material->emittance;

        // Is the light source directly visible or indirectly via specular reflection?
        if (ray.importance == 1.0/PIXEL_SAMPLES) {
            localLightingRadiance = -1.0;
        } else {
            localLightingRadiance = 0.0;
        }
    }
    else {
        Color localLighting = this->localLighting(ray);
        localLightingRadiance = glm::length(localLighting);
        // localLightingColor = glm::normalize(localLighting);
    }

    // In the middle of the ray tree?
    if (branchingRays.size() > 0) {
        localColor = target->material->sampleColor(branchingRays[0], ray);
        localLightingRadiance *= target->material->localLightContribution;
    } else {
        localColor = target->material->sampleColor(ray, ray);
    }
        
    // Handle branching rays
    for (auto branchingRay : branchingRays) {
        incomingColor += this->traceRay(branchingRay);
        incomingImportance += branchingRay.importance;
        incomingRadiance += branchingRay.radiance;
        ray.depth += branchingRay.depth;
    }

    ray.radiance  = localLightingRadiance;
    ray.radiance += incomingRadiance * incomingImportance / ray.importance;
    ray.color     = localColor * ray.radiance;
    ray.color    += incomingColor;

    return ray.color;
}

std::shared_ptr<SceneObject> Scene::rayIntersection(Ray& ray) const {
    double nearest = FLT_MAX;
    ray.target = DUMMY_TARGET;

    // TODO: Make this one single loop
    for (auto triangle : this->triangles) {
        double hit = triangle->rayIntersection(ray);

        // if (glm::dot(triangle->calculateNormal(ray.end), ray.direction) > 0.0)
        //     continue;

        if (hit > EPSILON && hit < nearest) {
            ray.target = triangle;
            ray.setEnd(hit);
            nearest = hit;
        }
    }

    for (auto sphere : this->spheres) {
        double hit = sphere->rayIntersection(ray);

        // if (glm::dot(sphere->calculateNormal(ray.end), ray.direction) > 0.0)
        //     continue;

        if (hit > EPSILON && hit < nearest) {
            ray.target = sphere;
            ray.setEnd(hit);
            nearest = hit;
        }
    }

    for (auto light : this->areaLights) {
        double hit = light->rayIntersection(ray);

        if (hit > EPSILON && hit < nearest) {
            ray.target = light;
            ray.setEnd(hit);
            nearest = hit;
        }
    }

    return ray.target;
}

void Scene::addTetrahedron(double width, double height, glm::dvec3 m, std::unique_ptr<Material> material) {
    using namespace glm;
    double offset = height/sqrt(2); 
    dvec3 a = m + dvec3(0.0, 0.0, height);
    dvec3 b = m + dvec3(-width, 0.0, 0.0);
    dvec3 c = m + dvec3(offset, offset, 0.0);
    dvec3 d = m + dvec3(offset, -offset, 0.0);

    std::vector<std::shared_ptr<Triangle>> tris{
        std::make_shared<Triangle>(Triangle{ b, a, c, std::make_unique<Material>(*material) }),
        std::make_shared<Triangle>(Triangle{ d, a, b, std::make_unique<Material>(*material) }),
        std::make_shared<Triangle>(Triangle{ c, a, d, std::make_unique<Material>(*material) }),
        std::make_shared<Triangle>(Triangle{ d, b, c, std::make_unique<Material>(*material) })
    };

    this->triangles.insert(this->triangles.end(), tris.begin(), tris.end());
}

void Scene::addBox(double width, double height, double depth, glm::dvec3 m, std::unique_ptr<Material> material) {
    using namespace glm;   
    // Vertices
    dvec3 f1 = m + dvec3(-depth/2.0,  width/2.0, 0.0);
    dvec3 f2 = m + dvec3(-depth/2.0,  width/2.0, height);
    dvec3 f3 = m + dvec3(-depth/2.0, -width/2.0, height);
    dvec3 f4 = m + dvec3(-depth/2.0, -width/2.0, 0.0);
    dvec3 b1 = m + dvec3( depth/2.0,  width/2.0, 0.0);
    dvec3 b2 = m + dvec3( depth/2.0,  width/2.0, height);
    dvec3 b3 = m + dvec3( depth/2.0, -width/2.0, height);
    dvec3 b4 = m + dvec3( depth/2.0, -width/2.0, 0.0);
    
    std::vector<std::shared_ptr<Triangle>> tris;
    // Front
    tris.emplace_back(std::make_shared<Triangle>(Triangle{ f1, f4, f2, std::make_unique<Material>(*material) }));
    tris.emplace_back(std::make_shared<Triangle>(Triangle{ f2, f4, f3, std::make_unique<Material>(*material) }));
    // Back
    tris.emplace_back(std::make_shared<Triangle>(Triangle{ b1, b2, b4, std::make_unique<Material>(*material) }));
    tris.emplace_back(std::make_shared<Triangle>(Triangle{ b4, b2, b3, std::make_unique<Material>(*material) }));
    // Left
    tris.emplace_back(std::make_shared<Triangle>(Triangle{ f2, b1, f1, std::make_unique<Material>(*material) }));
    tris.emplace_back(std::make_shared<Triangle>(Triangle{ f2, b2, b1, std::make_unique<Material>(*material) }));
    // Right
    tris.emplace_back(std::make_shared<Triangle>(Triangle{ f4, b4, f3, std::make_unique<Material>(*material) }));
    tris.emplace_back(std::make_shared<Triangle>(Triangle{ b4, b3, f3, std::make_unique<Material>(*material) }));
    // Top
    tris.emplace_back(std::make_shared<Triangle>(Triangle{ b3, f2, f3, std::make_unique<Material>(*material) }));
    tris.emplace_back(std::make_shared<Triangle>(Triangle{ b3, b2, f2, std::make_unique<Material>(*material) }));
    // Bottom
    tris.emplace_back(std::make_shared<Triangle>(Triangle{ f1, b4, f4, std::make_unique<Material>(*material) }));
    tris.emplace_back(std::make_shared<Triangle>(Triangle{ f1, b1, f4, std::make_unique<Material>(*material) }));

    this->triangles.insert(this->triangles.end(), tris.begin(), tris.end());
}

void Scene::addSphere(double radius, glm::dvec3 m, std::unique_ptr<Material> material)
{
    using namespace glm;
    this->spheres.push_back(std::make_shared<Sphere>(Sphere{m, radius, std::move(material)}));
}

void Scene::addAreaLight(std::shared_ptr<SceneObject> object) {
    this->areaLights.push_back(object);
}

void Scene::addPointLight(PointLight pointLight) {
    this->pointLights.push_back(pointLight);   
}

Color Scene::localLighting(const Ray& ray) const {
    Color totalLight = BLACK;
    // Handle point lights
    for (const PointLight& light : this->pointLights) {
        Ray lightRay = Ray{ light.position, ray.end };
        lightRay.target = ray.target;

        if (!rayOccluded(lightRay))
            totalLight += ray.localLighting(light);
    }
    // Handle area lights
    for (auto light : this->areaLights) {
        Color thisLight = BLACK;
        // glm::dvec3 offsetEnd = ray.offsetEndPoint();
        glm::dvec3 offsetEnd = ray.end + MAX_RAY_OFFSET * ray.target->calculateNormal(ray.end);
        auto shadowRays = light->sampleShadowRays(offsetEnd);

        for (auto shadowRay : shadowRays) {
            // Geometric term
            glm::dvec3 targetNormal = ray.target->calculateNormal(ray.end);
            glm::dvec3 lightNormal = light->calculateNormal(shadowRay.end);
            double alpha = glm::dot(targetNormal, shadowRay.direction);
            double beta = glm::dot(-shadowRay.direction, lightNormal);
            double cosTerm = alpha * beta;
            cosTerm = glm::max(cosTerm, 0.0);
            // Handle occlusion 
            shadowRay.target = light;
            if (!rayOccluded(shadowRay)) {
                double dropoff = glm::pow(glm::length(shadowRay.end - shadowRay.start), 2.0);
                thisLight += cosTerm * light->material->emittance * light->material->sampleColor(shadowRay, shadowRay) / dropoff;
            }
        }
        totalLight += thisLight / (double)shadowRays.size();
    }
    return totalLight;
}

bool Scene::rayOccluded(Ray& ray) const {
    // Save inital endpoint
    auto initial = ray.target;
    // Calculate potential closer intersections causing occlusion
    auto hit = rayIntersection(ray);

    // double distance = glm::abs(glm::distance(initial, ray.end));
    return (initial != hit) && hit->material->blocksLight;
}

// RAY

Color Ray::localLighting(const PointLight& light) const {
    using namespace glm;
    dvec3 dirToLight = light.position - this->end;
    double lightAmount = max(dot(normalize(dirToLight), this->target->calculateNormal(this->end)), 0.0);
    return light.color * light.intensity * lightAmount / sqrt(length(dvec3(dirToLight)));
}

void Ray::setEnd(double t) {
    this->end = this->start + this->direction * t;
}

glm::dvec3 Ray::offsetEndPoint() const {
    using namespace glm;
    dvec3 N = this->target->calculateNormal(this->end);
    double dN = 1.0;
    dvec3 result = this->end;
    double offset = dot(result, N);
    while (offset < 0 && offset > MAX_RAY_OFFSET) {
        offset = dot(result, N);
        dN = -0.75 * glm::sign(offset);
        result += N * dN;
    }
    return result;
}

// CAMERA

Pixel& Camera::getImagePixel(size_t x, size_t y) {
    return (*image)[IMAGE_SIZE * y + x];
}

void Camera::render(const Scene& scene, size_t x_lower, size_t x_upper, size_t y_lower, size_t y_upper)
{
    int pixelDims = glm::sqrt(this->samplesPerPixel);
    for (size_t y = y_lower; y < y_upper; ++y)
    {
        for (size_t x = x_lower; x < x_upper; ++x)
        {
            Pixel& pixel = getImagePixel(x, y); 

            for (int i = 0; i < this->samplesPerPixel; ++i) {
                // Subpixel coordinates
                double xi = ((double)x-401.0) * PIXEL_SIZE + (i % pixelDims) * this->subpixelSize;
                double yi = ((double)y-401.0) * PIXEL_SIZE + glm::floor(i/pixelDims) * this->subpixelSize;
                // Random offsets into the subpixel
                double dx = this->subpixelSize * ((double) rand()) / RAND_MAX;
                double dy = this->subpixelSize * ((double) rand()) / RAND_MAX;
                // Spawn a new ray from the active eye point going through the subpixel coordinate
                glm::dvec3 pixelCoord = glm::dvec3(0.0, -(xi + dx), -(yi + dy));
                glm::dvec3 start = this->primaryEyeActive ? this->primaryEye : this->secondaryEye;
                Ray ray = Ray{ start, pixelCoord-start, 1.0/samplesPerPixel };
                // Trace the ray through the scene and average its color over all rays in the tree
                Color rayColor = scene.traceRay(ray);
                rayColor /= (double)ray.depth;
                pixel.color += rayColor;
            }

            pixel.color /= (double)PIXEL_SAMPLES;

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
            if (color.r < 0.0 || color.g < 0.0 || color.b < 0.0) {
                color *= -maxIntensity;
            }
            pixelColor.red   += 255.99 * pow(color.r / maxIntensity, 0.38);
            pixelColor.green += 255.99 * pow(color.g / maxIntensity, 0.38);
            pixelColor.blue  += 255.99 * pow(color.b / maxIntensity, 0.38);
            // pixelColor.red   += 255.99 * log(1 + color.r / maxIntensity);
            // pixelColor.green += 255.99 * log(1 + color.g / maxIntensity);
            // pixelColor.blue  += 255.99 * log(1 + color.b / maxIntensity);

            // Output color to image
            rgbImage[y][x] = pixelColor;
        }
    }

    rgbImage.write(filename);
}

void Camera::toggleActiveEye() {
    this->primaryEyeActive = !this->primaryEyeActive;
}