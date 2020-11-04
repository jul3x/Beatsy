//
// Created by julex on 04.11.2020.
//

#ifndef BEATSY_CAMERA_H
#define BEATSY_CAMERA_H

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


class Camera {
public:
    explicit Camera(const glm::vec2& window_size_) : window_size(window_size_) {}

    void setProjection(float fov, float display_start, float display_end) {
        projection = glm::perspective(glm::radians(fov), window_size.x / window_size.y, display_start, display_end);
    }

    void setView(const glm::vec3& pos, const glm::vec3& look_at, const glm::vec3& rotation) {
        view = glm::lookAt(pos, look_at, rotation);
    }

    const glm::mat4& getProjection() const {
        return projection;
    }

    const glm::mat4& getView() const {
        return view;
    }

    const glm::vec3& getPosition() const {
        return position;
    }

    void updateMouse(const glm::vec2& mouse, double time_elapsed) {
        horizontal_angle += time_elapsed * mouse_speed * (window_size.x / 2.0 - mouse.x);
        vertical_angle += time_elapsed * mouse_speed * (window_size.y / 2.0 - mouse.y);
        direction = glm::vec3(std::cos(vertical_angle) * std::sin(horizontal_angle),
                              std::sin(vertical_angle),
                              std::cos(vertical_angle) * std::cos(horizontal_angle));

        right = glm::vec3(std::sin(horizontal_angle - 3.14 / 2.0),
                          0, std::cos(horizontal_angle - 3.14 / 2.0));
        up = glm::cross(right, direction);
    }

    void updateKeys(const std::list<GLuint>& keys, double time_elapsed) {
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

    void update() {
        view = glm::lookAt(position,position + direction, up);
    }

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
