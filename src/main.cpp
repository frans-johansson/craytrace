#include "craytrace.hpp"

int main() {
    using namespace glm;
    
    // Scene geometry
    // For final implementation:
    // * Floor and ceiling should be white 
    // * Each wall should be a unique color
    std::vector<Triangle> walls = {    
        // Ceiling
        Triangle{ vec4(-3.0, 0.0, 5.0, 1.0), vec4( 0.0, 6.0, 5.0, 1.0), vec4( 5.0, 0.0, 5.0, 1.0), Color{ 1.0, 1.0, 1.0 } },
        Triangle{ vec4( 0.0, 6.0, 5.0, 1.0), vec4(10.0, 6.0, 5.0, 1.0), vec4( 5.0, 0.0, 5.0, 1.0), Color{ 0.9, 0.9, 0.9 } },
        Triangle{ vec4(10.0, 6.0, 5.0, 1.0), vec4(13.0, 0.0, 5.0, 1.0), vec4( 5.0, 0.0, 5.0, 1.0), Color{ 0.7, 0.7, 0.7 } },
        Triangle{ vec4(13.0, 0.0, 5.0, 1.0), vec4(10.0,-6.0, 5.0, 1.0), vec4( 5.0, 0.0, 5.0, 1.0), Color{ 0.5, 0.5, 0.5 } },
        Triangle{ vec4(10.0,-6.0, 5.0, 1.0), vec4( 0.0,-6.0, 5.0, 1.0), vec4( 5.0, 0.0, 5.0, 1.0), Color{ 0.3, 0.3, 0.3 } },
        Triangle{ vec4( 0.0,-6.0, 5.0, 1.0), vec4(-3.0, 0.0, 5.0, 1.0), vec4( 5.0, 0.0, 5.0, 1.0), Color{ 0.2, 0.2, 0.2 } },
        // Floor
        Triangle{ vec4(-3.0, 0.0,-5.0, 1.0), vec4( 5.0, 0.0,-5.0, 1.0), vec4( 0.0, 6.0,-5.0, 1.0), Color{ 1.0, 1.0, 1.0 } },
        Triangle{ vec4( 0.0, 6.0,-5.0, 1.0), vec4( 5.0, 0.0,-5.0, 1.0), vec4(10.0, 6.0,-5.0, 1.0), Color{ 0.9, 0.9, 0.9 } },
        Triangle{ vec4(10.0, 6.0,-5.0, 1.0), vec4( 5.0, 0.0,-5.0, 1.0), vec4(13.0, 0.0,-5.0, 1.0), Color{ 0.7, 0.7, 0.7 } },
        Triangle{ vec4(13.0, 0.0,-5.0, 1.0), vec4( 5.0, 0.0,-5.0, 1.0), vec4(10.0,-6.0,-5.0, 1.0), Color{ 0.5, 0.5, 0.5 } },
        Triangle{ vec4(10.0,-6.0,-5.0, 1.0), vec4( 5.0, 0.0,-5.0, 1.0), vec4( 0.0,-6.0,-5.0, 1.0), Color{ 0.3, 0.3, 0.3 } },
        Triangle{ vec4( 0.0,-6.0,-5.0, 1.0), vec4( 5.0, 0.0,-5.0, 1.0), vec4(-3.0, 0.0,-5.0, 1.0), Color{ 0.2, 0.2, 0.2 } },
        // Left wall
        Triangle{ vec4( 0.0, 6.0, 5.0, 1.0), vec4( 0.0, 6.0,-5.0, 1.0), vec4(10.0, 6.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.0 } },
        Triangle{ vec4( 0.0, 6.0, 5.0, 1.0), vec4(10.0, 6.0,-5.0, 1.0), vec4(10.0, 6.0, 5.0, 1.0), Color{ 0.8, 0.0, 0.0 } },
        // Right wall
        Triangle{ vec4( 0.0,-6.0,-5.0, 1.0), vec4( 0.0,-6.0, 5.0, 1.0), vec4(10.0,-6.0,-5.0, 1.0), Color{ 0.0, 1.0, 0.0 } }, 
        Triangle{ vec4( 0.0,-6.0, 5.0, 1.0), vec4(10.0,-6.0, 5.0, 1.0), vec4(10.0,-6.0,-5.0, 1.0), Color{ 0.0, 0.8, 0.0 } },
        // Front walls
        Triangle{ vec4(10.0,-6.0, 5.0, 1.0), vec4(13.0, 0.0, 5.0, 1.0), vec4(10.0,-6.0,-5.0, 1.0), Color{ 0.0, 0.0, 1.0 } },
        Triangle{ vec4(13.0, 0.0, 5.0, 1.0), vec4(13.0, 0.0,-5.0, 1.0), vec4(10.0,-6.0,-5.0, 1.0), Color{ 0.0, 0.0, 0.8 } },
        Triangle{ vec4(10.0, 6.0, 5.0, 1.0), vec4(13.0, 0.0,-5.0, 1.0), vec4(13.0, 0.0, 5.0, 1.0), Color{ 0.0, 1.0, 1.0 } },
        Triangle{ vec4(10.0, 6.0, 5.0, 1.0), vec4(10.0, 6.0,-5.0, 1.0), vec4(13.0, 0.0,-5.0, 1.0), Color{ 0.0, 1.0, 0.8 } },
        // Back walls
        Triangle{ vec4( 0.0, 6.0, 5.0, 1.0), vec4(-3.0, 0.0,-5.0, 1.0), vec4( 0.0, 6.0,-5.0, 1.0), Color{ 1.0, 0.6, 0.0 } },
        Triangle{ vec4( 0.0, 6.0, 5.0, 1.0), vec4(-3.0, 0.0, 5.0, 1.0), vec4(-3.0, 0.0,-5.0, 1.0), Color{ 1.0, 1.0, 1.0 } },
        Triangle{ vec4(-3.0, 0.0, 5.0, 1.0), vec4( 0.0,-6.0,-5.0, 1.0), vec4(-3.0, 0.0,-5.0, 1.0), Color{ 1.0, 1.0, 1.0 } },
        Triangle{ vec4(-3.0, 0.0, 5.0, 1.0), vec4( 0.0,-6.0, 5.0, 1.0), vec4( 0.0,-6.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } }
    };

    std::cout << "Running CRaytrace...\n";

    // Create the scene
    Scene scene = Scene{ walls };
    // Add scene objects (Tetrahedron -Moa 🐟) 
    scene.addTetrahedron(1.0, 1.0, vec4(5.0, 1.5, -1.0, 1.0), std::move(Color{ 1.0, 0.5, 0.0 }));
    scene.addTetrahedron(1.5, 1.5, vec4(3.5, 0.5, -0.5, 1.0), std::move(Color{ 1.0, 0.0, 0.5 }));
    scene.addTetrahedron(2.5, 2.5, vec4(6.0, -1.5, -1.5, 1.0), std::move(Color{ 0.6, 1.0, 0.5 }));

    // Create the camera
    Camera camera = Camera{ glm::vec4(-2.0, 0.0, 0.0, 1.0), glm::vec4(-1.0, 0.0, 0.0, 1.0), 1 };
    camera.toggleActiveEye();

    // Render the scene with the camera
    camera.render(scene);
    camera.create_image("test_render.png");

    std::cout << "Finished rendering!\n";

    return 0;
}