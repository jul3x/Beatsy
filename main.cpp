#include <random>

#include "WindowWrapper.h"
#include "Shader.h"
#include "Camera.h"
#include "FFT.h"

int main() {
    auto window_size = glm::vec2(1000, 700);
    WindowWrapper window_wrapper(window_size, "Beatsy Visualizer");
    Camera camera(window_size);
    camera.setProjection(90.0f, 0.1f, 100.0f);
    camera.setView({3, 3, 3}, {0, 0, 0}, {0, 1, 0});

    window_wrapper.bindCamera(&camera);

    WindowWrapper::colorize(0.02, 0.02, 0.02, 0);
    window_wrapper.setRenderType(GL_LINES);

    window_wrapper.loadShaders("shaders/vertex.vert", "shaders/fragment.frag");
    window_wrapper.loadScreenShaders("shaders/screen.vert", "shaders/screen.frag");

    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<int> distrib(-100.0f, 100.0f);

//    GLuint texture = loadBMP_custom("uvtemplate.bmp");
//    GLuint texture_2 = loadBMP_custom("uvtemplate_2.bmp");
//    auto cube = std::make_shared<Model>("cube.obj");
//    auto chair = std::make_shared<Model>("FinalBaseMesh.obj");
//    std::unique_ptr<Mesh> meshes[1000];
//    for (auto &mesh : meshes)
//    {
//        static int i = 0;
//        mesh = std::make_unique<Mesh>(i % 2 == 0 ? cube : chair, i % 2 == 0 ? texture : texture_2);
//        mesh->setModel(glm::translate(glm::mat4(1.0f), {distrib(gen), distrib(gen), distrib(gen)}));
//        window_wrapper.bindMesh(*mesh);
//        ++i;
//    }

    Grid grid(20);
    window_wrapper.bindMesh(grid);


    window_wrapper.bindBuffers();

    {
        applyFFT();
    }

    double last_time = glfwGetTime();
    do {
        double current_time = glfwGetTime();
        double time_elapsed = current_time - last_time;
//        printf("%lf FPS\n", 1.0 / time_elapsed);

        camera.updateMouse(window_wrapper.getMousePos(), time_elapsed);
        camera.updateKeys(window_wrapper.getKeysPressed(), time_elapsed);
        camera.update();

//        for (auto &mesh : meshes)
//        {
//            mesh->update();
//        }

        WindowWrapper::clear();

        window_wrapper.draw();

        last_time = current_time;
    } while (!window_wrapper.isClosed());

    window_wrapper.terminate();

    return 0;
}

