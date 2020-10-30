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

std::unique_ptr<PerfectRefractor> MaterialFactory::makePerfectRefractor(double index) {
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

    // double trans[] = {
    //     1.0, 0.0, 0.0, -incoming.end.x,
    //     0.0, 1.0, 0.0, -incoming.end.y,
    //     0.0, 0.0, 1.0, -incoming.end.z,
    //     0.0, 0.0, 0.0, 1.0,
    // };
    // double rot[] = {
    //     X.x,  X.y,  X.z,  0.0,
    //     Y.x,  Y.y,  Y.z,  0.0,
    //     Z.x,  Z.y,  Z.z,  0.0,
    //     0.0, 0.0, 0.0, 1.0,
    // };

Ray Material::diffuseReflection(const Ray& incoming) {
    // Local coordinate system
    using namespace glm;
    dvec3 Z = incoming.target->calculateNormal(incoming.end);
    dvec3 X = normalize(incoming.direction - dot(incoming.direction, Z) * Z);
    dvec3 Y = normalize(cross(-X, Z));
    // Transformation matrix for local coordinates
    double rot[] = {
        X.x,  X.y,  X.z,  0.0,
        Y.x,  Y.y,  Y.z,  0.0,
        Z.x,  Z.y,  Z.z,  0.0,
        0.0,  0.0,  0.0,  1.0,
    };
    mat4 M = make_mat4(rot); 
    // Monte carlo method to randomly select ray direction in local hemispherical coordinates
    double phi = 2 * M_PI * rand() / RAND_MAX;
    double theta = asin(sqrt(((double) rand()) / RAND_MAX));
    // Cartesian local coordinates for the direction
    double x = cos(phi) * sin(theta);
    double y = sin(phi) * sin(theta);
    double z = cos(theta);
    dvec3 localDir = dvec3(x, y, z);
    // Transform direction to world coordinates
    dvec3 worldDir = inverse(transpose(M)) * dvec4(localDir, 1.0);

    // Offset starting point with some bias to avoid self-intersections
    // Try offsetting along worldDir rather than surface normal?
    dvec3 offsetStart = incoming.offsetEndPoint();
    // dvec3 offsetStart = incoming.end;

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

// DIFFUSE OREN-NAYAR

double DiffuseOrenNayar::sampleBRDF(const Ray& incoming, const Ray& exitant) {
    // Spherical coordinates of directions
    double phi_in    = atan(incoming.direction.y / incoming.direction.x);
    double phi_out   = atan(exitant.direction.y / exitant.direction.x);
    double theta_in  = atan(pow(incoming.direction.y, 2.0) + pow(incoming.direction.x, 2.0) / incoming.direction.z);
    double theta_out = atan(pow(exitant.direction.y, 2.0) + pow(exitant.direction.x, 2.0) / exitant.direction.z);
    
    double sig2 = pow(sig, 2.0);
    double A = 1 - sig2 / (2 * (sig2 + 0.33));
    double B = 0.45 * sig2 / (sig2 + 0.09);
    double alpha = glm::max(theta_in, theta_out);
    double beta = glm::min(theta_in, theta_out);

    return this->rho * (A + B * glm::max(0.0, cos(phi_in - phi_out)) * sin(alpha) * sin(beta)) / M_PI;
}

// PERFECT REFLECTOR

std::vector<Ray> PerfectReflector::nextRayBranch(const Ray& incoming) {
    return { this->perfectReflection(incoming) };
}

// PERFECT REFRACTOR

std::vector<Ray> PerfectRefractor::nextRayBranch(const Ray& incoming) {
    using namespace glm;
    std::vector<Ray> result;
    double indexRatio = AIR_INDEX / this->index;
    dvec3 I = incoming.direction;
    dvec3 N = incoming.target->calculateNormal(incoming.end);

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
        dvec3 refractedDirection = indexRatio * I + N * (double)(-indexRatio * dot(I, N) - sqrt(1 - pow(indexRatio, 2.0) * (1 - pow(dot(I, N), 2.0))));
        result.emplace_back(Ray{ incoming.end, refractedDirection, incoming.importance });
        // Schlicks equation
        double R0 = pow((AIR_INDEX - this->index) / (AIR_INDEX + this->index), 2.0);
        double R = R0 + (1 - R0) * pow((1 - dot(I, N)), 5.0);
        double T = 1 - R;
        // Apply coefficients
        result[0].importance *= R;
        result[1].importance *= T;
    }

    return result;
}
