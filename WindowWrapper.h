//
// Created by julex on 03.11.2020.
//

#ifndef BEATSY_WINDOWWRAPPER_H
#define BEATSY_WINDOWWRAPPER_H

#include <string>
#include <stdexcept>
#include <vector>
#include <list>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


#include "Shader.h"
#include "Utils.h"


class WindowWrapper {
public:
    WindowWrapper(const glm::vec2& size, const std::string& title) : window_size(size) {
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialize GLFW\n");
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
        if (window == nullptr)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to open GLFW window.\n");
        }

        glfwMakeContextCurrent(window);

        glewExperimental = true;
        if (glewInit() != GLEW_OK)
        {
            glfwTerminate();
            throw std::runtime_error("Failed to initialize GLEW\n");
        }

        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

        glGenVertexArrays(1, &vertex_array);
        glBindVertexArray(vertex_array);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
    }

    static void colorize(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f) {
        glClearColor(r, g, b, a);
    }

    static void clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void setProjection(float fov, float display_start, float display_end) {
        projection = glm::perspective(glm::radians(fov), window_size.x / window_size.y, display_start, display_end);
    }

    void setView(const glm::vec3& pos, const glm::vec3& look_at, const glm::vec3& rotation) {
        view = glm::lookAt(pos, look_at, rotation);
    }

    void draw() {
        glUseProgram(shader);

        for (size_t i = 0; i < meshes.size(); ++i)
        {
            glm::mat4 mvp = projection * view * meshes[i]->getModel();
            GLuint matrix_uniform = glGetUniformLocation(shader, "MVP");
            glUniformMatrix4fv(matrix_uniform, 1, GL_FALSE, &mvp[0][0]);

            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[i]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, color_buffers[i]);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

            glDrawArrays(GL_TRIANGLES, 0, meshes[i]->getVertices().size());

            glDisableVertexAttribArray(0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void terminate() {
        glDeleteProgram(shader);

        for (auto buffer : vertex_buffers)
        {
            glDeleteBuffers(1, &buffer);
        }

        for (auto buffer : color_buffers)
        {
            glDeleteBuffers(1, &buffer);
        }

        glDeleteVertexArrays(1, &vertex_array);
        glfwTerminate();
    }

    bool isClosed() {
        return glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(window) != 0;
    }

    void bindMesh(Mesh& mesh) {
        meshes.emplace_back(&mesh);
        vertex_buffers.emplace_back();
        color_buffers.emplace_back();

        glGenBuffers(1, &vertex_buffers.back());
        glGenBuffers(1, &color_buffers.back());

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers.back());
        glBufferData(GL_ARRAY_BUFFER, mesh.getVertices().size() * sizeof(GLfloat), &mesh.getVertices()[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, color_buffers.back());
        glBufferData(GL_ARRAY_BUFFER, mesh.getColors().size() * sizeof(GLfloat), &mesh.getColors()[0], GL_STATIC_DRAW);


    }

    void loadShaders(const std::string& vert, const std::string& frag) {
        shader = LoadShaders(vert, frag);
    }

private:
    GLFWwindow* window{nullptr};
    GLuint vertex_array{};
    GLuint shader{};

    glm::vec2 window_size{};
    glm::mat4 projection{}, view{};

    std::vector<Mesh*> meshes;
    std::vector<GLuint> vertex_buffers, color_buffers;
};

#endif //BEATSY_WINDOWWRAPPER_H
