#include "app.h"
#include <cstdlib>

int main () {
    App app { "OpenGL App", 800, 600 };
    if (!app.init()) {
        return EXIT_FAILURE;
    }
    app.run();
    
    return EXIT_SUCCESS;
}
