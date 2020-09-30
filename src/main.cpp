#include "dependencies.hpp"

int main() {
    using namespace glm;
    
    // Scene geometry
    // For final implementation:
    // * Floor and ceiling should be white 
    // * Each wall should be a unique color
    std::vector<std::shared_ptr<Triangle>> walls = {    
        // Ceiling
        std::make_shared<Triangle>(Triangle{ vec4(-3.0, 0.0, 5.0, 1.0), vec4( 0.0, 6.0, 5.0, 1.0), vec4( 5.0, 0.0, 5.0, 1.0), WHITE }),
        std::make_shared<Triangle>(Triangle{ vec4( 0.0, 6.0, 5.0, 1.0), vec4(10.0, 6.0, 5.0, 1.0), vec4( 5.0, 0.0, 5.0, 1.0), WHITE }),
        std::make_shared<Triangle>(Triangle{ vec4(10.0, 6.0, 5.0, 1.0), vec4(13.0, 0.0, 5.0, 1.0), vec4( 5.0, 0.0, 5.0, 1.0), WHITE }),
        std::make_shared<Triangle>(Triangle{ vec4(13.0, 0.0, 5.0, 1.0), vec4(10.0,-6.0, 5.0, 1.0), vec4( 5.0, 0.0, 5.0, 1.0), WHITE }),
        std::make_shared<Triangle>(Triangle{ vec4(10.0,-6.0, 5.0, 1.0), vec4( 0.0,-6.0, 5.0, 1.0), vec4( 5.0, 0.0, 5.0, 1.0), WHITE }),
        std::make_shared<Triangle>(Triangle{ vec4( 0.0,-6.0, 5.0, 1.0), vec4(-3.0, 0.0, 5.0, 1.0), vec4( 5.0, 0.0, 5.0, 1.0), WHITE }),
        // Floor
        std::make_shared<Triangle>(Triangle{ vec4(-3.0, 0.0,-5.0, 1.0), vec4( 5.0, 0.0,-5.0, 1.0), vec4( 0.0, 6.0,-5.0, 1.0), WHITE }),
        std::make_shared<Triangle>(Triangle{ vec4( 0.0, 6.0,-5.0, 1.0), vec4( 5.0, 0.0,-5.0, 1.0), vec4(10.0, 6.0,-5.0, 1.0), WHITE }),
        std::make_shared<Triangle>(Triangle{ vec4(10.0, 6.0,-5.0, 1.0), vec4( 5.0, 0.0,-5.0, 1.0), vec4(13.0, 0.0,-5.0, 1.0), WHITE }),
        std::make_shared<Triangle>(Triangle{ vec4(13.0, 0.0,-5.0, 1.0), vec4( 5.0, 0.0,-5.0, 1.0), vec4(10.0,-6.0,-5.0, 1.0), WHITE }),
        std::make_shared<Triangle>(Triangle{ vec4(10.0,-6.0,-5.0, 1.0), vec4( 5.0, 0.0,-5.0, 1.0), vec4( 0.0,-6.0,-5.0, 1.0), WHITE }),
        std::make_shared<Triangle>(Triangle{ vec4( 0.0,-6.0,-5.0, 1.0), vec4( 5.0, 0.0,-5.0, 1.0), vec4(-3.0, 0.0,-5.0, 1.0), WHITE }),
        // Left wall
        std::make_shared<Triangle>(Triangle{ vec4( 0.0, 6.0, 5.0, 1.0), vec4( 0.0, 6.0,-5.0, 1.0), vec4(10.0, 6.0,-5.0, 1.0), RED }),
        std::make_shared<Triangle>(Triangle{ vec4( 0.0, 6.0, 5.0, 1.0), vec4(10.0, 6.0,-5.0, 1.0), vec4(10.0, 6.0, 5.0, 1.0), RED }),
        // Right wall
        std::make_shared<Triangle>(Triangle{ vec4( 0.0,-6.0,-5.0, 1.0), vec4( 0.0,-6.0, 5.0, 1.0), vec4(10.0,-6.0,-5.0, 1.0), GREEN }), 
        std::make_shared<Triangle>(Triangle{ vec4( 0.0,-6.0, 5.0, 1.0), vec4(10.0,-6.0, 5.0, 1.0), vec4(10.0,-6.0,-5.0, 1.0), GREEN }),
        // Front walls
        std::make_shared<Triangle>(Triangle{ vec4(10.0,-6.0, 5.0, 1.0), vec4(13.0, 0.0, 5.0, 1.0), vec4(10.0,-6.0,-5.0, 1.0), BLUE }),
        std::make_shared<Triangle>(Triangle{ vec4(13.0, 0.0, 5.0, 1.0), vec4(13.0, 0.0,-5.0, 1.0), vec4(10.0,-6.0,-5.0, 1.0), BLUE }),
        std::make_shared<Triangle>(Triangle{ vec4(10.0, 6.0, 5.0, 1.0), vec4(13.0, 0.0,-5.0, 1.0), vec4(13.0, 0.0, 5.0, 1.0), CYAN }),
        std::make_shared<Triangle>(Triangle{ vec4(10.0, 6.0, 5.0, 1.0), vec4(10.0, 6.0,-5.0, 1.0), vec4(13.0, 0.0,-5.0, 1.0), CYAN }),
        // Back walls
        std::make_shared<Triangle>(Triangle{ vec4( 0.0, 6.0, 5.0, 1.0), vec4(-3.0, 0.0,-5.0, 1.0), vec4( 0.0, 6.0,-5.0, 1.0), MAGENTA }),
        std::make_shared<Triangle>(Triangle{ vec4( 0.0, 6.0, 5.0, 1.0), vec4(-3.0, 0.0, 5.0, 1.0), vec4(-3.0, 0.0,-5.0, 1.0), MAGENTA }),
        std::make_shared<Triangle>(Triangle{ vec4(-3.0, 0.0, 5.0, 1.0), vec4( 0.0,-6.0,-5.0, 1.0), vec4(-3.0, 0.0,-5.0, 1.0), YELLOW }),
        std::make_shared<Triangle>(Triangle{ vec4(-3.0, 0.0, 5.0, 1.0), vec4( 0.0,-6.0, 5.0, 1.0), vec4( 0.0,-6.0,-5.0, 1.0), YELLOW })
    };

    std::cout << "Running CRaytrace...\n";

    // Create the scene
    Scene scene = Scene{ std::move(walls) };
    // Add scene objects (Tetrahedron -Moa 🐟) 
    scene.addTetrahedron(1.0, 1.0, vec4(7.5, 1.0,  1.0, 1.0), std::move(Color{ 1.0, 0.5, 0.0 }), 0.0f);
    scene.addTetrahedron(1.5, 1.5, vec4(7.5, 1.0, -0.5, 1.0), std::move(Color{ 1.0, 0.0, 0.5 }), 0.0f);
    scene.addTetrahedron(2.5, 2.5, vec4(7.5, 1.0, -3.0, 1.0), std::move(Color{ 0.0, 1.0, 0.5 }), 0.0f);
    // Add point lights for local lighting
    scene.addPointLight({ glm::vec4(5.0, 0.0, 4.5, 1.0), 0.2, Color{ 1.0, 1.0, 1.0 } });
    // scene.addPointLight({ glm::vec4(2.0, 4.0,-4.0, 1.0), 0.8, Color{ 1.0, 0.3, 0.3 } });
    // scene.addPointLight({ glm::vec4(2.0,-4.0,-4.0, 1.0), 0.5, Color{ 0.3, 0.3, 1.0 } });

    // Create the camera
    Camera camera = Camera{ glm::vec4(-2.0, 0.0, 0.0, 1.0), glm::vec4(-1.0, 0.0, 0.0, 1.0), 1 };
    // camera.toggleActiveEye();

    // Render the scene with the camera
    camera.render(scene);
    camera.create_image("test_render.png");

    std::cout << "Finished rendering!\n";

    return 0;
}