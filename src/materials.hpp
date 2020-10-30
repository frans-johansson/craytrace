#pragma once
#include "dependencies.hpp"

struct Material {
    Color color;
    double absorption; // MAX_ABSORPTION * (double)(glm::length(color) / glm::length(WHITE))
    double emittance = 0.0;
    double localLightContribution = LOCAL_LIGHTING_CONTRIBUTION;
    bool blocksLight = true;

    Material(Color _color)
    : color{_color}, absorption{MIN_ABSORPTION + (MAX_ABSORPTION - MIN_ABSORPTION) * (double)(glm::length(color) / glm::length(WHITE))} {}

    virtual std::vector<Ray> nextRayBranch(const Ray& incoming);
    Ray perfectRefraction(const Ray& incoming); 
    Ray perfectReflection(const Ray& incoming);
    Ray innerReflection(const Ray& incoming);
    Ray diffuseReflection(const Ray& incoming);

    virtual bool rayAbsorbed(const Ray &ray) {
        double absorb = ((double)rand()) / RAND_MAX;
        return absorb > this->absorption;
    }
    
    virtual double sampleBRDF(const Ray& incoming, const Ray& exitant) {
        return 1.0;
    }
    virtual Color sampleColor(const Ray& ray, const Ray& exitant) {
        return this->color;
    };
};

struct DiffuseLambertian : Material {
    double rho;

    DiffuseLambertian(Color _color, double _rho)
    : Material(_color), rho{_rho} {}

    double sampleBRDF(const Ray& ray, const Ray& exitant);
};

struct DiffuseOrenNayar : Material {
    double rho;
    double sig;

    DiffuseOrenNayar(Color _color, double _rho, double _sig)
    : Material(_color), rho{_rho}, sig{_sig} {}

    double sampleBRDF(const Ray& ray, const Ray& exitant);
};

struct PerfectReflector : Material {
    PerfectReflector()
    : Material(BLACK) {
        // this->localLightContribution = glm::pow(LOCAL_LIGHTING_CONTRIBUTION, PIXEL_SAMPLES);
        this->localLightContribution = LOCAL_LIGHTING_CONTRIBUTION / 10.0;
    }

    std::vector<Ray> nextRayBranch(const Ray& incoming);
};

struct PerfectRefractor : Material {
    double index;
    double maxAngle;

    PerfectRefractor(double _index)
    : Material(WHITE), index{_index}, maxAngle{asin(AIR_INDEX / _index)} {
        this->localLightContribution = 0.0;
        this->blocksLight = false;
    }

    std::vector<Ray> nextRayBranch(const Ray& incoming);
};

struct LambertianEmitter : Material {
    LambertianEmitter(Color _color, double _emittance)
    : Material(_color) {
        this->emittance = _emittance / M_PI;
    }

    std::vector<Ray> nextRayBranch(const Ray& incoming) {
        return {};
    }
};

struct MaterialFactory {
    static std::unique_ptr<DiffuseLambertian> makeDiffuseLambertian(Color _color, double _rho = 0.2);
    static std::unique_ptr<DiffuseOrenNayar> makeDiffuseOrenNayar(Color _color, double _rho = 0.2, double _sig = 0.5);
    static std::unique_ptr<PerfectReflector> makePerfectReflector();
    static std::unique_ptr<PerfectRefractor> makePerfectRefractor(double index = 1.5);
    static std::unique_ptr<LambertianEmitter> makeLambertianEmitter(Color _color, double _emittance);
};
