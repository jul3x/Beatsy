//
// Created by julex on 03.11.2020.
//

#ifndef BEATSY_UTILS_H
#define BEATSY_UTILS_H

#include <glm/gtc/matrix_transform.hpp>

#include "ObjLoader.h"
#include "Texture.h"

class Mesh {
public:
    Mesh() : model(glm::mat4(1.0f)) {}

    explicit Mesh(const std::string& obj_path, GLuint texture_) {
        loadObject(obj_path, texture_);
    }

    void loadObject(const std::string& obj_path, GLuint texture_) {
        loadObj(obj_path.c_str(), this->vertices, this->uv, this->normals);
        texture = texture_;
    }

    void setVertices(const std::vector<glm::vec3>& vertices_) {
        this->vertices = vertices_;
    }

    void setUV(const std::vector<glm::vec2>& uv_) {
        this->uv = uv_;
    }

    void setNormals(const std::vector<glm::vec3>& normals_) {
        this->normals = normals_;
    }

    void setModel(const glm::mat4& model_) {
        this->model = model_;
    }

    const std::vector<glm::vec3>& getVertices() const {
        return this->vertices;
    }

    const std::vector<glm::vec2>& getUV() const {
        return this->uv;
    }

    const std::vector<glm::vec3>& getNormals() const {
        return this->normals;
    }

    const glm::mat4& getModel() const {
        return this->model;
    }

    GLuint getTexture() const {
        return this->texture;
    }

    void update() {
        model = glm::rotate(model, 0.001f, {0.0, 1, 0});
    }

private:
    std::vector<glm::vec3> vertices{}, normals{};
    std::vector<glm::vec2> uv{};

    glm::mat4 model{};
    GLuint texture{};

};


#endif //BEATSY_UTILS_H
