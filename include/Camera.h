//
// Created by julex on 04.11.2020.
//

#ifndef BEATSY_CAMERA_H
#define BEATSY_CAMERA_H

#include <iostream>
#include <list>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


class Camera {
public:
    explicit Camera(const glm::vec2& window_size_);
    void setProjection(float fov, float display_start, float display_end);
    void setView(const glm::vec3& pos, const glm::vec3& look_at, const glm::vec3& rotation);
    const glm::mat4& getProjection() const;
    const glm::mat4& getView() const;
    const glm::vec3& getPosition() const;
    void updateMouse(const glm::vec2& mouse, double time_elapsed);
    void updateKeys(const std::list<GLuint>& keys, double time_elapsed);
    void update();

private:
    glm::mat4 projection{1.0f}, view{1.0f};

    glm::vec2 window_size;
    glm::vec3 position{0, 0, 5};

    glm::vec3 direction, up, right;
    double horizontal_angle{3.14};
    double vertical_angle{0.0};

    double speed{30.0};
    double mouse_speed{2.0};

};

#endif //BEATSY_CAMERA_H
