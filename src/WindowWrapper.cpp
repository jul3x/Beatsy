//
// Created by julex on 07.01.2021.
//

#include <WindowWrapper.h>

WindowWrapper::WindowWrapper(const glm::vec2& size, const std::string& title) : window_size(size) {
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW\n");
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);
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

    glGenFramebuffers(1, &frame_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
    glGenTextures(1, &tex_color_buffer);
    glBindTexture(GL_TEXTURE_2D, tex_color_buffer);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_color_buffer, 0);
    glGenRenderbuffers(1, &depth_render_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depth_render_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, size.x, size.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_render_buffer);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_color_buffer, 0);

    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Framebuffer init error!");


    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

    glGenBuffers(1, &vertex_buffer);
    glGenBuffers(1, &color_buffer);
    glGenBuffers(1, &normal_buffer);
    glGenBuffers(1, &element_buffer);

    glGenVertexArrays(1, &vao_frame_buffer);
    glBindVertexArray(vao_frame_buffer);

    static const GLfloat g_quad_vertex_buffer_data[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            1.0f,  1.0f, 0.0f,
    };

    glGenBuffers(1, &vertices_frame_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_frame_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_quad_vertex_buffer_data), g_quad_vertex_buffer_data, GL_STATIC_DRAW);
}

void WindowWrapper::setRenderType(GLuint render_t) {
    render_type = render_t;
}

void WindowWrapper::bindCamera(Camera* camera_) {
    camera = camera_;
}

void WindowWrapper::updateVertexX(size_t index, float x)
{
    vertices[index].x = x;
}

void WindowWrapper::updateVertexY(size_t index, float y)
{
    vertices[index].y = y;
}

void WindowWrapper::updateVertexZ(size_t index, float z)
{
    vertices[index].z = z;
}

void WindowWrapper::colorize(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void WindowWrapper::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void WindowWrapper::draw() {
    if (camera == nullptr) {
        throw std::runtime_error("Camera not initialized!");
    }

    glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(vertex_array);
    glEnable(GL_DEPTH_TEST);
    glUseProgram(shader);
    glViewport(0, 0, window_size.x, window_size.y);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);

    glUniformMatrix4fv(view_uniform, 1, GL_FALSE, &camera->getView()[0][0]);
    glUniform3f(light_uniform, camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
    for (auto & mesh : meshes)
    {
        glm::mat4 mvp = camera->getProjection() * camera->getView() * mesh->getModel();
        glUniformMatrix4fv(mvp_uniform, 1, GL_FALSE, &mvp[0][0]);
        glUniformMatrix4fv(model_uniform, 1, GL_FALSE, &mesh->getModel()[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mesh->getTexture());
        glUniform1i(mesh->getTexture(), 0);

        glDrawElements(this->render_type, mesh->getVertices().size(), GL_UNSIGNED_SHORT, (void*)mesh->getOffset());
    }

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindVertexArray(vao_frame_buffer);
    glViewport(0, 0, window_size.x, window_size.y);
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glDisable(GL_DEPTH_TEST);
    glUseProgram(screen_shader);
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, tex_color_buffer);
    glUniform1i(frame_texture_uniform, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_frame_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void WindowWrapper::terminate() {
    glDeleteProgram(shader);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &color_buffer);
    glDeleteBuffers(1, &normal_buffer);
    glDeleteBuffers(1, &element_buffer);
    glDeleteBuffers(1, &tex_color_buffer);
    glDeleteBuffers(1, &depth_render_buffer);
    glDeleteBuffers(1, &vao_frame_buffer);
    glDeleteBuffers(1, &vertices_frame_buffer);
    glDeleteVertexArrays(1, &vao_frame_buffer);
    glDeleteVertexArrays(1, &vertex_array);
    glDeleteFramebuffers(1, &frame_buffer);
    glfwTerminate();
}

bool WindowWrapper::isClosed() {
    return glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(window) != 0;
}

void WindowWrapper::bindMesh(Mesh& mesh) {
    mesh.setOffset(indices.size() * sizeof(unsigned short));
    meshes.emplace_back(&mesh);

    indexVBO(mesh.getVertices(), mesh.getColors(), mesh.getNormals(), indices, vertices, colors, normals);
}

void WindowWrapper::bindBuffers() {
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, color_buffer);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);
}

void WindowWrapper::loadShaders(const std::string& vert, const std::string& frag) {
    shader = LoadShaders(vert, frag);
    glUseProgram(shader);
    mvp_uniform = glGetUniformLocation(shader, "MVP");
    light_uniform = glGetUniformLocation(shader, "LightPosition_worldspace");
    view_uniform = glGetUniformLocation(shader, "V");
    model_uniform = glGetUniformLocation(shader, "M");
    texture_uniform = glGetUniformLocation(shader, "myTextureSampler");
}

void WindowWrapper::loadScreenShaders(const std::string& vert, const std::string& frag) {
    screen_shader = LoadShaders(vert, frag);
    glUseProgram(screen_shader);
    frame_texture_uniform = glGetUniformLocation(screen_shader, "renderedTexture");
}

glm::vec2 WindowWrapper::getMousePos() const {
    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);
    glfwSetCursorPos(window, window_size.x / 2.0f, window_size.y / 2.0f);
    return {x_pos, y_pos};
}

std::list<GLuint> WindowWrapper::getKeysPressed() const {
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

void WindowWrapper::updateVertexColor(size_t index, float r, float g, float b)
{
    colors[index] = glm::vec3(r, g, b);
}
