//
// Created by julex on 03.11.2020.
//

#ifndef BEATSY_UTILS_H
#define BEATSY_UTILS_H

#include <glm/gtc/matrix_transform.hpp>

class Mesh {
public:
    Mesh() : model(glm::mat4(1.0f)) {}

    void setVertices(const std::vector<GLfloat>& vertices_) {
        this->vertices = vertices_;
    }

    void setColors(const std::vector<GLfloat>& colors_) {
        this->colors = colors_;
    }

    void setModel(const glm::mat4& model_) {
        this->model = model_;
    }

    const std::vector<GLfloat>& getVertices() const {
        return this->vertices;
    }

    const std::vector<GLfloat>& getColors() const {
        return this->colors;
    }

    const glm::mat4& getModel() const {
        return this->model;
    }

    void update() {
        model = glm::rotate(model, 0.001f, {0.0, 1, 0});
    }

private:
    std::vector<GLfloat> vertices{}, colors{};

    glm::mat4 model;
};


#endif //BEATSY_UTILS_H
