#include <random>
#include <fstream>

#include "Config.h"
#include "WindowWrapper.h"
#include "Camera.h"
#include "Audio.h"

int main(int argc, char** argv) {
    if (argc != 2)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "./Beatsy <path-to-config-file> (e.g. ./Beatsy conf.j3x)" << std::endl;

        return 0;
    }

    Config::getInstance().initialize(argv[1]);

    auto window_size = glm::vec2(CONF("window")["x"].asInt(), CONF("window")["y"].asInt());

    WindowWrapper window_wrapper(window_size, "Beatsy Visualizer");
    Camera camera(window_size);
    camera.setProjection(CONF("camera")["fov"].asFloat(), CONF("camera")["display_start"].asFloat(), CONF("camera")["display_end"].asFloat());
    camera.setView({CONF("camera")["pos"]["x"].asFloat(), CONF("camera")["pos"]["y"].asFloat(), CONF("camera")["pos"]["z"].asFloat()},
            {0, 0, 0}, {0, 1, 0});

    window_wrapper.bindCamera(&camera);

    WindowWrapper::colorize(CONF("bg")["r"].asFloat(), CONF("bg")["g"].asFloat(), CONF("bg")["b"].asFloat(), 1.0);

    if (CONF("grid")["type"].asString() == "points")
        window_wrapper.setRenderType(GL_POINTS);
    else
        window_wrapper.setRenderType(GL_LINES);

    window_wrapper.loadShaders("shaders/vertex.vert", "shaders/fragment.frag");

    if (CONF("window")["blur"].asBool())
        window_wrapper.loadScreenShaders("shaders/screen.vert", "shaders/screen_blur.frag");
    else
        window_wrapper.loadScreenShaders("shaders/screen.vert", "shaders/screen.frag");

    Audio audio(CONF("audio")["path"].asString());
    Grid grid(audio, CONF("grid")["size"].asFloat(), CONF("grid")["count"].asInt());
    window_wrapper.bindMesh(grid);

    window_wrapper.bindBuffers();

    double last_time = glfwGetTime();
    do {
        double current_time = glfwGetTime();
        double time_elapsed = current_time - last_time;

        if (!CONF("camera")["lock"].asBool())
        {
            camera.updateMouse(window_wrapper.getMousePos(), time_elapsed);
            camera.updateKeys(window_wrapper.getKeysPressed(), time_elapsed);
        }
        camera.update();

        grid.update(window_wrapper, camera, audio, time_elapsed);

        WindowWrapper::clear();

        window_wrapper.draw();

        last_time = current_time;
    } while (!window_wrapper.isClosed());

    window_wrapper.terminate();

    return 0;
}

