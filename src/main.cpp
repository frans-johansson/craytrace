#include "dependencies.hpp"

int main() {
    using namespace glm;

    // Seed the random number generator
    // srand(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    
    // Scene geometry
    // For final implementation:
    // * Floor and ceiling should be white 
    // * Each wall should be a unique color
    std::vector<std::shared_ptr<Triangle>> walls = {    
        // Ceiling
        std::make_shared<Triangle>(Triangle{ dvec3(-3.0, 0.0, 5.0), dvec3( 0.0, 6.0, 5.0), dvec3( 5.0, 0.0, 5.0), MaterialFactory::makeDiffuseLambertian(WHITE, 0.4) }),
        std::make_shared<Triangle>(Triangle{ dvec3( 0.0, 6.0, 5.0), dvec3(10.0, 6.0, 5.0), dvec3( 5.0, 0.0, 5.0), MaterialFactory::makeDiffuseLambertian(WHITE, 0.4) }),
        std::make_shared<Triangle>(Triangle{ dvec3(10.0, 6.0, 5.0), dvec3(13.0, 0.0, 5.0), dvec3( 5.0, 0.0, 5.0), MaterialFactory::makeDiffuseLambertian(WHITE, 0.4) }),
        std::make_shared<Triangle>(Triangle{ dvec3(13.0, 0.0, 5.0), dvec3(10.0,-6.0, 5.0), dvec3( 5.0, 0.0, 5.0), MaterialFactory::makeDiffuseLambertian(WHITE, 0.4) }),
        std::make_shared<Triangle>(Triangle{ dvec3(10.0,-6.0, 5.0), dvec3( 0.0,-6.0, 5.0), dvec3( 5.0, 0.0, 5.0), MaterialFactory::makeDiffuseLambertian(WHITE, 0.4) }),
        std::make_shared<Triangle>(Triangle{ dvec3( 0.0,-6.0, 5.0), dvec3(-3.0, 0.0, 5.0), dvec3( 5.0, 0.0, 5.0), MaterialFactory::makeDiffuseLambertian(WHITE, 0.4) }),
        // Floor
        std::make_shared<Triangle>(Triangle{ dvec3(-3.0, 0.0,-5.0), dvec3( 5.0, 0.0,-5.0), dvec3( 0.0, 6.0,-5.0), MaterialFactory::makeDiffuseLambertian(WHITE, 0.4) }),
        std::make_shared<Triangle>(Triangle{ dvec3( 0.0, 6.0,-5.0), dvec3( 5.0, 0.0,-5.0), dvec3(10.0, 6.0,-5.0), MaterialFactory::makeDiffuseLambertian(WHITE, 0.4) }),
        std::make_shared<Triangle>(Triangle{ dvec3(10.0, 6.0,-5.0), dvec3( 5.0, 0.0,-5.0), dvec3(13.0, 0.0,-5.0), MaterialFactory::makeDiffuseLambertian(WHITE, 0.4) }),
        std::make_shared<Triangle>(Triangle{ dvec3(13.0, 0.0,-5.0), dvec3( 5.0, 0.0,-5.0), dvec3(10.0,-6.0,-5.0), MaterialFactory::makeDiffuseLambertian(WHITE, 0.4) }),
        std::make_shared<Triangle>(Triangle{ dvec3(10.0,-6.0,-5.0), dvec3( 5.0, 0.0,-5.0), dvec3( 0.0,-6.0,-5.0), MaterialFactory::makeDiffuseLambertian(WHITE, 0.4) }),
        std::make_shared<Triangle>(Triangle{ dvec3( 0.0,-6.0,-5.0), dvec3( 5.0, 0.0,-5.0), dvec3(-3.0, 0.0,-5.0), MaterialFactory::makeDiffuseLambertian(WHITE, 0.4) }),
        // Left wall
        std::make_shared<Triangle>(Triangle{ dvec3( 0.0, 6.0, 5.0), dvec3( 0.0, 6.0,-5.0), dvec3(10.0, 6.0,-5.0), MaterialFactory::makeDiffuseLambertian(RED, 0.9) }), // RED
        std::make_shared<Triangle>(Triangle{ dvec3( 0.0, 6.0, 5.0), dvec3(10.0, 6.0,-5.0), dvec3(10.0, 6.0, 5.0), MaterialFactory::makeDiffuseLambertian(RED, 0.9) }),
        // Right wall
        std::make_shared<Triangle>(Triangle{ dvec3( 0.0,-6.0,-5.0), dvec3( 0.0,-6.0, 5.0), dvec3(10.0,-6.0,-5.0), MaterialFactory::makeDiffuseLambertian(MAGENTA, 0.9) }), 
        std::make_shared<Triangle>(Triangle{ dvec3( 0.0,-6.0, 5.0), dvec3(10.0,-6.0, 5.0), dvec3(10.0,-6.0,-5.0), MaterialFactory::makeDiffuseLambertian(MAGENTA, 0.9) }),
        // Front walls
        std::make_shared<Triangle>(Triangle{ dvec3(10.0,-6.0, 5.0), dvec3(13.0, 0.0, 5.0), dvec3(10.0,-6.0,-5.0), MaterialFactory::makeDiffuseLambertian(BLUE, 0.9) }), // BLUE
        std::make_shared<Triangle>(Triangle{ dvec3(13.0, 0.0, 5.0), dvec3(13.0, 0.0,-5.0), dvec3(10.0,-6.0,-5.0), MaterialFactory::makeDiffuseLambertian(BLUE, 0.9) }),
        std::make_shared<Triangle>(Triangle{ dvec3(10.0, 6.0, 5.0), dvec3(13.0, 0.0,-5.0), dvec3(13.0, 0.0, 5.0), MaterialFactory::makeDiffuseLambertian(CYAN, 0.9) }), // CYAN
        std::make_shared<Triangle>(Triangle{ dvec3(10.0, 6.0, 5.0), dvec3(10.0, 6.0,-5.0), dvec3(13.0, 0.0,-5.0), MaterialFactory::makeDiffuseLambertian(CYAN, 0.9) }),
        // Back walls
        std::make_shared<Triangle>(Triangle{ dvec3( 0.0, 6.0, 5.0), dvec3(-3.0, 0.0,-5.0), dvec3( 0.0, 6.0,-5.0), MaterialFactory::makeDiffuseLambertian(ORANGE, 0.9) }), // ORANGE
        std::make_shared<Triangle>(Triangle{ dvec3( 0.0, 6.0, 5.0), dvec3(-3.0, 0.0, 5.0), dvec3(-3.0, 0.0,-5.0), MaterialFactory::makeDiffuseLambertian(ORANGE, 0.9) }),
        std::make_shared<Triangle>(Triangle{ dvec3(-3.0, 0.0, 5.0), dvec3( 0.0,-6.0,-5.0), dvec3(-3.0, 0.0,-5.0), MaterialFactory::makeDiffuseLambertian(YELLOW, 0.9) }), // YELLOW
        std::make_shared<Triangle>(Triangle{ dvec3(-3.0, 0.0, 5.0), dvec3( 0.0,-6.0, 5.0), dvec3( 0.0,-6.0,-5.0), MaterialFactory::makeDiffuseLambertian(YELLOW, 0.9) })
    };

    std::cout << "Running CRaytrace...\n";

    // Create the scene
    Scene scene = Scene{ std::move(walls) };
    
    // Add scene objects (Tetrahedron -Moa 🐟)
    // scene.addTetrahedron(1.0, 1.0, dvec3(7.5, 1.0,  1.0, 1.0), Color{ 1.0, 0.5, 0.0 });
    // scene.addTetrahedron(1.5, 1.5, dvec3(7.5, 1.0, -0.5, 1.0), Color{ 1.0, 0.0, 0.5 }, SurfaceType::MIRROR);
    // scene.addTetrahedron(1.0, 2.5, dvec3(4.0, -2.3, -4.5), std::move(MaterialFactory::makeDiffuseOrenNayar(GREEN)));
    // scene.addTetrahedron(0.8, 2.0, dvec3(3.5,  0.3, -5.0, 1.0), YELLOW);
    // scene.addTetrahedron(0.3, 2.0, dvec3(7.0, -3.5, -1.0, 1.0), Color{ 0.2, 0.8, 1.0 });

    // scene.addBox(4.0, 0.8, 4.0, dvec3(4.0, -2.2, -4.5), std::move(MaterialFactory::makeDiffuseLambertian(GREEN)));
    // scene.addBox(2.0, 2.0, 2.0, dvec3(5.0, -1.0, -5.5), std::move(MaterialFactory::makePerfectRefractor()));

    // Add schene objects (Sphere ⚽)
    // scene.addSphere(3.0, dvec3(4.4, 2.8, -2.5), std::move(MaterialFactory::makeDiffuseLambertian(WHITE)));
    // scene.addSphere(1.5, dvec3(7.5, -3.5, -2.8), std::move(MaterialFactory::makePerfectReflector()));
    // scene.addSphere(2.5, dvec3(8.2, 0.0, -1.0), std::move(MaterialFactory::makeDiffuseOrenNayar(YELLOW)));
    
    scene.addSphere(2.5, dvec3(8.2, 0.0, -1.0), std::move(MaterialFactory::makePerfectRefractor()));

    // Add point lights for local lighting
    // scene.addPointLight({ glm::dvec3(5.0, 0.0, 4.5, 1.0), 1.0, WHITE });
    // scene.addPointLight({ glm::dvec3(2.0, 4.0,-4.0, 1.0), 0.8, WHITE });
    // scene.addPointLight({ glm::dvec3(2.0,-4.0,-4.0, 1.0), 0.8, WHITE });
    // scene.addPointLight({ glm::dvec3(2.0, 4.0,-4.0, 1.0), 0.8, Color{ 1.0, 0.3, 0.3 } });
    // scene.addPointLight({ glm::dvec3(2.0,-4.0,-4.0, 1.0), 0.5, Color{ 0.3, 0.3, 1.0 } });

    // Add area lights
    auto l1 = std::make_shared<Triangle>(Triangle{ 
        dvec3(5.5, 0.5, 4.9),
        dvec3(4.5,-0.5, 4.9),
        dvec3(4.5, 0.5, 4.9),
        MaterialFactory::makeLambertianEmitter(WHITE, 1.0 * M_PI) 
    });
    auto l2 = std::make_shared<Triangle>(Triangle{ 
        dvec3(5.5, 0.5, 4.9),
        dvec3(5.5,-0.5, 4.9),
        dvec3(4.5,-0.5, 4.9),
        MaterialFactory::makeLambertianEmitter(WHITE, 1.0 * M_PI)
    });
    scene.addAreaLight(l1);
    scene.addAreaLight(l2);
    // scene.addAreaLight(Triangle{ dvec3(5.5, 5.0, 4.5, 1.0), dvec3(4.5, 4.0, 4.5, 1.0), dvec3(4.5, 5.0, 4.5, 1.0), WHITE }, 1.0);
    // scene.addAreaLight(Triangle{ dvec3(5.5, 5.0, 4.5, 1.0), dvec3(5.5, 4.0, 4.5, 1.0), dvec3(4.5, 4.0, 4.5, 1.0), WHITE }, 1.0);
    // scene.addAreaLight(Triangle{ dvec3(5.5,-5.0, 4.5, 1.0), dvec3(4.5,-5.0, 4.5, 1.0), dvec3(4.5,-4.0, 4.5, 1.0), WHITE }, 1.0);
    // scene.addAreaLight(Triangle{ dvec3(5.5,-5.0, 4.5, 1.0), dvec3(4.5,-4.0, 4.5, 1.0), dvec3(5.5,-4.0, 4.5, 1.0), WHITE }, 1.0);

    // Create the camera
    Camera camera = Camera{ glm::dvec3(-2.0, 0.0, 0.0), glm::dvec3(-1.0, 0.0, 0.0), PIXEL_SAMPLES };

    camera.toggleActiveEye();

    // Start timing
    auto start = std::chrono::high_resolution_clock::now();

    // Render the scene with the camera
    #ifdef NOTHREAD
    camera.render(scene, 0, IMAGE_SIZE, 0, IMAGE_SIZE);
    #else
    std::thread t1(&Camera::render, &camera, scene, 0, IMAGE_SIZE/2, 0, IMAGE_SIZE/2);
    std::thread t2(&Camera::render, &camera, scene, IMAGE_SIZE/2, IMAGE_SIZE, 0, IMAGE_SIZE/2);
    std::thread t3(&Camera::render, &camera, scene, 0, IMAGE_SIZE/2, IMAGE_SIZE/2, IMAGE_SIZE);
    std::thread t4(&Camera::render, &camera, scene, IMAGE_SIZE/2, IMAGE_SIZE, IMAGE_SIZE/2, IMAGE_SIZE);

    // Wait for all threads to finish before creating the image
    t1.join(); t2.join(); t3.join(); t4.join();
    #endif

    // Stop timing and display total execution time
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
    std::cout << "Finished rendering! Total rendering time: " << duration << "s\n";

    // Create filename using ostringstream
    std::ostringstream oss;
    oss << MIN_ABSORPTION * 100 << "-" << MAX_ABSORPTION * 100 << "pab_" << PIXEL_SAMPLES << "sp_" << duration << "s.png";
    camera.create_image(oss.str().c_str());

    return 0;
}