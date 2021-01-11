#include <random>

#include "WindowWrapper.h"
#include "Camera.h"
#include "FFT.h"

int main() {
    auto window_size = glm::vec2(1920, 1080);
    WindowWrapper window_wrapper(window_size, "Beatsy Visualizer");
    Camera camera(window_size);
    camera.setProjection(90.0f, 0.1f, 100.0f);
    camera.setView({10, 20, 10}, {0, 0, 0}, {0, 1, 0});

    window_wrapper.bindCamera(&camera);

    WindowWrapper::colorize(0.0, 0.0, 0.0, 1.0);
    window_wrapper.setRenderType(GL_LINES);

    window_wrapper.loadShaders("shaders/vertex.vert", "shaders/fragment.frag");
    window_wrapper.loadScreenShaders("shaders/screen.vert", "shaders/screen.frag");

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> distrib(-100.0f, 100.0f);

    Grid grid(40.0f, 40);
    window_wrapper.bindMesh(grid);

//    GLuint texture = loadBMP_custom("uvtemplate.bmp");
//    auto cube = std::make_shared<Model>("cube.obj");
//    std::unique_ptr<Mesh> meshes[0];
//    for (auto &mesh : meshes)
//    {
//        static int i = 0;
//        mesh = std::make_unique<Mesh>(cube, texture);
//        mesh->setModel(glm::translate(glm::mat4(1.0f), {distrib(gen), distrib(gen), distrib(gen)}));
//        window_wrapper.bindMesh(*mesh);
//        ++i;
//    }

    window_wrapper.bindBuffers();

    double last_time = glfwGetTime();
    do {
        double current_time = glfwGetTime();
        double time_elapsed = current_time - last_time;

//        camera.updateMouse(window_wrapper.getMousePos(), time_elapsed);
        camera.updateKeys(window_wrapper.getKeysPressed(), time_elapsed);
        camera.update();

        grid.update(window_wrapper, camera, time_elapsed);
//        for (auto &mesh : meshes)
//        {
//            mesh->update(time_elapsed);
//        }

        WindowWrapper::clear();

        window_wrapper.draw();

        last_time = current_time;
    } while (!window_wrapper.isClosed());

    window_wrapper.terminate();

    return 0;
}

