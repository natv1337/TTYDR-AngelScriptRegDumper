#include <stdio.h>

#include <switch.h>
#include <dmntcht.h>

#include "Application.hpp"

int main(int argc, char** argv) {
    Application* app = new Application();
    app->initialize();
    app->run();
    app->uninitialize();

    delete app;
    return 0;
}
