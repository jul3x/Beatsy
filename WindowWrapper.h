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

#include "Camera.h"
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
//        glEnable(GL_CULL_FACE);
        glDepthFunc(GL_LESS);
    }

    void bindCamera(Camera* camera_) {
        camera = camera_;
    }

    static void colorize(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f) {
        glClearColor(r, g, b, a);
    }

    static void clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void draw() {
        if (camera == nullptr) {
            throw std::runtime_error("Camera not initialized!");
        }

        for (size_t i = 0; i < meshes.size(); ++i)
        {
            glm::mat4 mvp = camera->getProjection() * camera->getView() * meshes[i]->getModel();

            glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &mvp[0][0]);
            glUniformMatrix4fv(model_uniform, 1, GL_FALSE, &meshes[i]->getModel()[0][0]);
            glUniformMatrix4fv(view_uniform, 1, GL_FALSE, &camera->getView()[0][0]);
            glUniform3f(light_uniform, camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, meshes[i]->getTexture());
            glUniform1i(meshes[i]->getTexture(), 0);

            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[i]);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

            glEnableVertexAttribArray(1);
            glBindBuffer(GL_ARRAY_BUFFER, uv_buffers[i]);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

            glEnableVertexAttribArray(2);
            glBindBuffer(GL_ARRAY_BUFFER, normal_buffers[i]);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

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

        for (auto buffer : uv_buffers)
        {
            glDeleteBuffers(1, &buffer);
        }

        for (auto buffer : normal_buffers)
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
        uv_buffers.emplace_back();
        normal_buffers.emplace_back();

        glGenBuffers(1, &vertex_buffers.back());
        glGenBuffers(1, &uv_buffers.back());
        glGenBuffers(1, &normal_buffers.back());

        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers.back());
        glBufferData(GL_ARRAY_BUFFER, mesh.getVertices().size() * sizeof(glm::vec3), &mesh.getVertices()[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, uv_buffers.back());
        glBufferData(GL_ARRAY_BUFFER, mesh.getUV().size() * sizeof(glm::vec2), &mesh.getUV()[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, normal_buffers.back());
        glBufferData(GL_ARRAY_BUFFER, mesh.getNormals().size() * sizeof(glm::vec3), &mesh.getNormals()[0], GL_STATIC_DRAW);
    }

    void loadShaders(const std::string& vert, const std::string& frag) {
        shader = LoadShaders(vert, frag);
        glUseProgram(shader);
        mvp_uniform = glGetUniformLocation(shader, "MVP");
        light_uniform = glGetUniformLocation(shader, "LightPosition_worldspace");
        view_uniform = glGetUniformLocation(shader, "V");
        model_uniform = glGetUniformLocation(shader, "M");
        texture_uniform = glGetUniformLocation(shader, "myTextureSampler");
    }

    glm::vec2 getMousePos() const {
        double x_pos, y_pos;
        glfwGetCursorPos(window, &x_pos, &y_pos);
        glfwSetCursorPos(window, window_size.x / 2.0f, window_size.y / 2.0f);
        return {x_pos, y_pos};
    }

    std::list<GLuint> getKeysPressed() const {
        std::list<GLuint> key_list;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            key_list.push_back(GLFW_KEY_W);
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            key_list.push_back(GLFW_KEY_S);
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            key_list.push_back(GLFW_KEY_D);
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            key_list.push_back(GLFW_KEY_A);
        }

        return key_list;
    }

private:
    GLFWwindow* window{nullptr};
    GLuint vertex_array{};
    GLuint shader{};

    glm::vec2 window_size{};
    Camera* camera{nullptr};

    std::vector<Mesh*> meshes;
    std::vector<GLuint> vertex_buffers, uv_buffers, normal_buffers;

    GLuint mvp_uniform, view_uniform, model_uniform, light_uniform, texture_uniform;
};

#endif //BEATSY_WINDOWWRAPPER_H
