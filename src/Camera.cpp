//
// Created by julex on 07.01.2021.
//

#include <Camera.h>

Camera::Camera(const glm::vec2& window_size_) : window_size(window_size_) {}

void Camera::setProjection(float fov, float display_start, float display_end) {
    projection = glm::perspective(glm::radians(fov), window_size.x / window_size.y, display_start, display_end);
}

void Camera::setView(const glm::vec3& pos, const glm::vec3& look_at, const glm::vec3& rotation) {
    view = glm::lookAt(pos, look_at, rotation);
}

const glm::mat4& Camera::getProjection() const {
    return projection;
}

const glm::mat4& Camera::getView() const {
    return view;
}

const glm::vec3& Camera::getPosition() const {
    return position;
}

void Camera::updateMouse(const glm::vec2& mouse, double time_elapsed) {
    horizontal_angle += time_elapsed * mouse_speed * (window_size.x / 2.0 - mouse.x);
    vertical_angle += time_elapsed * mouse_speed * (window_size.y / 2.0 - mouse.y);
    direction = glm::vec3(std::cos(vertical_angle) * std::sin(horizontal_angle),
                          std::sin(vertical_angle),
                          std::cos(vertical_angle) * std::cos(horizontal_angle));

    right = glm::vec3(std::sin(horizontal_angle - 3.14 / 2.0),
                      0, std::cos(horizontal_angle - 3.14 / 2.0));
    up = glm::cross(right, direction);
}

void Camera::updateKeys(const std::list<GLuint>& keys, double time_elapsed) {
    for (auto key : keys)
    {
        switch (key)
        {
            case GLFW_KEY_W:
                position += float(time_elapsed * speed) * direction;
                break;
            case GLFW_KEY_S:
                position -= float(time_elapsed * speed) * direction;
                break;
            case GLFW_KEY_D:
                position += float(time_elapsed * speed) * right;
                break;
            case GLFW_KEY_A:
                position -= float(time_elapsed * speed) * right;
                break;
            default:
                break;
        }
    }
}

void Camera::update() {
    view = glm::lookAt(position,position + direction, up);
}
