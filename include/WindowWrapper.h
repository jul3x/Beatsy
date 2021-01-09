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
#include "VBOIndexer.h"


class WindowWrapper {
public:
    WindowWrapper(const glm::vec2& size, const std::string& title);

    void setRenderType(GLuint render_t);
    
    void bindCamera(Camera* camera_);

    void updateVertexX(size_t index, float x);

    void updateVertexY(size_t index, float y);

    void updateVertexZ(size_t index, float z);

    static void colorize(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 0.0f);

    static void clear();

    void draw();

    void terminate();

    bool isClosed();

    void bindMesh(Mesh& mesh);

    void bindBuffers();

    void loadShaders(const std::string& vert, const std::string& frag);

    void loadScreenShaders(const std::string& vert, const std::string& frag);

    glm::vec2 getMousePos() const;

    std::list<GLuint> getKeysPressed() const;

private:
    GLFWwindow* window{nullptr};
    GLuint vertex_array{};
    GLuint shader{}, screen_shader{};
    GLuint frame_buffer{}, tex_color_buffer{}, depth_render_buffer{}, vao_frame_buffer{}, vertices_frame_buffer{};

    glm::vec2 window_size{};
    Camera* camera{nullptr};

    std::vector<Mesh*> meshes;
    std::vector<unsigned short> indices;
    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> uvs;
    GLuint vertex_buffer{}, uv_buffer{}, normal_buffer{}, element_buffer{};

    GLuint mvp_uniform{}, view_uniform{}, model_uniform{}, light_uniform{}, texture_uniform{};
    GLuint frame_texture_uniform{};
    GLuint render_type{GL_TRIANGLES};
};

#endif //BEATSY_WINDOWWRAPPER_H
