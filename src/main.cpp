#include "craytrace.hpp"

int main() {
    using namespace glm;
    
    // Scene geometry
    // For final implementation:
    // * Floor and ceiling should be white 
    // * Each wall should be a unique color
    std::vector<Triangle> walls = {    
        // Ceiling
        Triangle{ dvec4(-3.0, 0.0, 5.0, 1.0), dvec4( 0.0, 6.0, 5.0, 1.0), dvec4( 5.0, 0.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4( 0.0, 5.0, 5.0, 1.0), dvec4(10.0, 6.0, 5.0, 1.0), dvec4( 5.0, 0.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(10.0, 6.0, 5.0, 1.0), dvec4(13.0, 0.0, 5.0, 1.0), dvec4( 5.0, 0.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(10.0,-6.0, 5.0, 1.0), dvec4(13.0, 0.0, 5.0, 1.0), dvec4( 5.0, 0.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4( 0.0,-6.0, 5.0, 1.0), dvec4(10.0,-6.0, 5.0, 1.0), dvec4( 5.0, 0.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(-3.0, 0.0, 5.0, 1.0), dvec4( 0.0,-6.0, 5.0, 1.0), dvec4( 5.0, 0.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        // Floor 
        Triangle{ dvec4(-3.0, 0.0,-5.0, 1.0), dvec4( 0.0, 6.0,-5.0, 1.0), dvec4( 5.0, 0.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4( 0.0, 5.0,-5.0, 1.0), dvec4(10.0, 6.0,-5.0, 1.0), dvec4( 5.0, 0.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(10.0, 6.0,-5.0, 1.0), dvec4(13.0, 0.0,-5.0, 1.0), dvec4( 5.0, 0.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(10.0,-6.0,-5.0, 1.0), dvec4(13.0, 0.0,-5.0, 1.0), dvec4( 5.0, 0.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4( 0.0,-6.0,-5.0, 1.0), dvec4(10.0,-6.0,-5.0, 1.0), dvec4( 5.0, 0.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(-3.0, 0.0,-5.0, 1.0), dvec4( 0.0,-6.0,-5.0, 1.0), dvec4( 5.0, 0.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        // Left and righ walls
        Triangle{ dvec4( 0.0,-6.0,-5.0, 1.0), dvec4( 0.0,-6.0, 5.0, 1.0), dvec4(10.0,-6.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4( 0.0,-6.0,-5.0, 1.0), dvec4( 0.0,-6.0, 5.0, 1.0), dvec4(10.0,-6.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4( 0.0, 6.0,-5.0, 1.0), dvec4( 0.0, 6.0, 5.0, 1.0), dvec4(10.0, 6.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4( 0.0, 6.0,-5.0, 1.0), dvec4( 0.0, 6.0, 5.0, 1.0), dvec4(10.0, 6.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        // Front walls
        Triangle{ dvec4(13.0, 0.0,-5.0, 1.0), dvec4(13.0, 0.0, 5.0, 1.0), dvec4(10.0, 6.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(13.0, 0.0,-5.0, 1.0), dvec4(13.0, 0.0, 5.0, 1.0), dvec4(10.0, 6.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(13.0, 0.0,-5.0, 1.0), dvec4(13.0, 0.0, 5.0, 1.0), dvec4(10.0,-6.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(13.0, 0.0,-5.0, 1.0), dvec4(13.0, 0.0, 5.0, 1.0), dvec4(10.0,-6.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        // Back wall
        Triangle{ dvec4(-3.0, 0.0,-5.0, 1.0), dvec4(-3.0, 0.0, 5.0, 1.0), dvec4( 0.0, 6.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(-3.0, 0.0,-5.0, 1.0), dvec4(-3.0, 0.0, 5.0, 1.0), dvec4( 0.0, 6.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(-3.0, 0.0,-5.0, 1.0), dvec4(-3.0, 0.0, 5.0, 1.0), dvec4( 0.0,-6.0,-5.0, 1.0), Color{ 1.0, 0.0, 0.5 } },
        Triangle{ dvec4(-3.0, 0.0,-5.0, 1.0), dvec4(-3.0, 0.0, 5.0, 1.0), dvec4( 0.0,-6.0, 5.0, 1.0), Color{ 1.0, 0.0, 0.5 } }
    };

    // Create the scene
    Scene scene = Scene{ walls };


    std::cout << "Welcome to CRaytracer\n";
    return 0;
}