#include "dependencies.hpp"

// MATERIAL FACTORY

std::unique_ptr<DiffuseLambertian> MaterialFactory::makeDiffuseLambertian(Color _color, double _rho) {
    return std::make_unique<DiffuseLambertian>(DiffuseLambertian{ _color, _rho });
}

std::unique_ptr<DiffuseOrenNayar> MaterialFactory::makeDiffuseOrenNayar(Color _color, double _rho, double _sig) {
    return std::make_unique<DiffuseOrenNayar>(DiffuseOrenNayar{ _color, _rho, _sig });
}

std::unique_ptr<PerfectReflector> MaterialFactory::makePerfectReflector() {
    return std::make_unique<PerfectReflector>(PerfectReflector{});
}

std::unique_ptr<PerfectRefractor> MaterialFactory::makePerfectRefractor(float index) {
    return std::make_unique<PerfectRefractor>(PerfectRefractor{index});
}

std::unique_ptr<LambertianEmitter> MaterialFactory::makeLambertianEmitter(Color _color, double _emittance) {
    return std::make_unique<LambertianEmitter>(LambertianEmitter{ _color, _emittance });
}

// MATERIAL

Ray Material::perfectReflection(const Ray& incoming) {
    return Ray{
        incoming.end,
        glm::reflect(incoming.direction, incoming.target->calculateNormal(incoming.end)),
        incoming.importance
    };
}

Ray Material::innerReflection(const Ray& incoming) {
    return Ray{
        incoming.end,
        glm::reflect(incoming.direction, -incoming.target->calculateNormal(incoming.end)),
        incoming.importance
    };
}

Ray Material::diffuseReflection(const Ray& incoming) {
    // Local coordinate system
    using namespace glm;
    vec3 Z = incoming.target->calculateNormal(incoming.end);
    vec3 X = normalize(-incoming.direction - dot(incoming.direction, Z) * Z);
    vec3 Y = normalize(cross(-X, Z));
    // Transformation matrix for local coordinates
    // float trans[] = {
    //     1.0f, 0.0f, 0.0f, -incoming.end.x,
    //     0.0f, 1.0f, 0.0f, -incoming.end.y,
    //     0.0f, 0.0f, 1.0f, -incoming.end.z,
    //     0.0f, 0.0f, 0.0f, 1.0f,
    // };
    // float rot[] = {
    //     X.x,  X.y,  X.z,  0.0f,
    //     Y.x,  Y.y,  Y.z,  0.0f,
    //     Z.x,  Z.y,  Z.z,  0.0f,
    //     0.0f, 0.0f, 0.0f, 1.0f,
    // };
    float rot[] = {
        X.x,  X.y,  X.z,
        Y.x,  Y.y,  Y.z,
        Z.x,  Z.y,  Z.z
    };
    mat3 M = make_mat3(rot); 
    // Monte carlo method to randomly select ray direction in local hemispherical coordinates
    float phi = 2 * M_PI * rand() / RAND_MAX;
    float theta = asin(sqrt(((float) rand()) / RAND_MAX));
    // Cartesian local coordinates for the direction
    float x = cos(phi) * sin(theta);
    float y = sin(phi) * sin(theta);
    float z = cos(theta);
    vec4 localDir = vec4(x, y, z, 0.0f);
    // Transform direction to world coordinates
    vec3 worldDir = inverse(M) * localDir;
    // Length of original ray used to calculate importance drop off
    // float dropoff = sqrt(length(incoming.end - incoming.start));
    // Offset starting point with some bias to avoid self-intersections
    vec4 offsetStart = incoming.end + vec4(Z * SHADOW_BIAS, 1.0);

    Ray exitant = Ray{ offsetStart, worldDir, incoming.importance };
    exitant.importance *= M_PI * this->sampleBRDF(incoming, exitant) / this->absorption;
    return exitant;
}

std::vector<Ray> Material::nextRayBranch(const Ray& incoming) {
    if (!this->rayAbsorbed(incoming)) {
        return { this->diffuseReflection(incoming) };
    }
    return {};
}

// DIFFUSE LAMBERTIAN

double DiffuseLambertian::sampleBRDF(const Ray& incoming, const Ray& exitant) {
    return this->rho / M_PI;
}

Color DiffuseLambertian::sampleColor(const Ray& incoming, const Ray& exitant) {
    // return this->color * this->sampleBRDF(incoming, exitant);
    return this->color;
}

// DIFFUSE OREN-NAYAR

double DiffuseOrenNayar::sampleBRDF(const Ray& incoming, const Ray& exitant) {
    // Spherical coordinates of directions
    float phi_in    = atan(incoming.direction.y / incoming.direction.x);
    float phi_out   = atan(exitant.direction.y / exitant.direction.x);
    float theta_in  = atan(pow(incoming.direction.y, 2.0) + pow(incoming.direction.x, 2.0) / incoming.direction.z);
    float theta_out = atan(pow(exitant.direction.y, 2.0) + pow(exitant.direction.x, 2.0) / exitant.direction.z);
    
    double sig2 = pow(sig, 2.0);
    double A = 1 - sig2 / (2 * (sig2 + 0.33));
    double B = 0.45 * sig2 / (sig2 + 0.09);
    double alpha = glm::max(theta_in, theta_out);
    double beta = glm::min(theta_in, theta_out);

    return this->rho * (A + B * glm::max(0.0f, cos(phi_in - phi_out)) * sin(alpha) * sin(beta)) / M_PI;
}

Color DiffuseOrenNayar::sampleColor(const Ray& incoming, const Ray& exitant) {
    // Color result = this->color * this->sampleBRDF(incoming, exitant);
    return this->color;
}

// PERFECT REFLECTOR

std::vector<Ray> PerfectReflector::nextRayBranch(const Ray& incoming) {
    return { this->perfectReflection(incoming) };
}

// PERFECT REFRACTOR

std::vector<Ray> PerfectRefractor::nextRayBranch(const Ray& incoming) {
    using namespace glm;
    std::vector<Ray> result;
    float indexRatio = AIR_INDEX / this->index;
    vec3 I = incoming.direction;
    vec3 N = incoming.target->calculateNormal(incoming.end);

    if (incoming.innerReflections == 0) {
        // Reflection on the outside
        Ray reflected = this->perfectReflection(incoming);
        reflected.innerReflections = 1;
        result.push_back(reflected);
    } else if (incoming.innerReflections < INNER_REFLECTIONS_LIMIT) {
        // Reflection on the inside
        Ray reflected = this->innerReflection(incoming);
        reflected.innerReflections = incoming.innerReflections + 1;
        result.push_back(reflected);

        // Refracting inside to outside
        indexRatio = 1 / indexRatio;
        // N *= -1.0;
    } else {
        return {};
    }

    // Check for perfect reflections
    if (acos(dot(I, N)) < maxAngle || incoming.innerReflections == 0) {
        // Refractions
        vec3 refractedDirection = indexRatio * I + N * (float)(-indexRatio * dot(I, N) - sqrt(1 - pow(indexRatio, 2.0) * (1 - pow(dot(I, N), 2.0))));
        result.emplace_back(Ray{ incoming.end, refractedDirection, incoming.importance });
        // Schlicks equation
        // float R0 = pow((AIR_INDEX - this->index) / (AIR_INDEX + this->index), 2.0);
        // float R = R0 + (1 - R0) * pow((1 - dot(I, N)), 5.0);
        // float T = 1 - R;
        // Apply coefficients
        result[0].importance *= 0.1;
        result[1].importance *= 0.9;
    }

    return result;
}

// LAMBERTIAN EMITTER

Color LambertianEmitter::sampleColor(const Ray& incoming, const Ray& exitant) {
    // return this->emittance * this->color;
    return this->color;
}