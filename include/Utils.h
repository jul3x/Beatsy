//
// Created by julex on 03.11.2020.
//

#ifndef BEATSY_UTILS_H
#define BEATSY_UTILS_H

#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include <ObjLoader.h>
#include <Texture.h>

class WindowWrapper;

class Model {
public:
    Model() = default;

    explicit Model(const std::string& obj_path);
    void loadObject(const std::string& obj_path);
    void setVertices(const std::vector<glm::vec3>& vertices_);
    void setUV(const std::vector<glm::vec2>& uv_);
    void setNormals(const std::vector<glm::vec3>& normals_);
    std::vector<glm::vec3>& getVertices();
    const std::vector<glm::vec2>& getUV() const;
    const std::vector<glm::vec3>& getNormals() const;
private:
    std::vector<glm::vec3> vertices{}, normals{};
    std::vector<glm::vec2> uv{};
};


class Mesh {
public:
    Mesh();

    explicit Mesh(const std::shared_ptr<Model>& data_, GLuint texture_);
    void setModel(const glm::mat4& model_);

    void setOffset(size_t indices);

    size_t getOffset() const;
    const std::vector<glm::vec3>& getVertices() const;
    const std::vector<glm::vec2>& getUV() const;
    const std::vector<glm::vec3>& getNormals() const;
    const glm::mat4& getModel() const;
    GLuint getTexture() const;

    void update();

protected:
    std::shared_ptr<Model> data;
private:
    size_t indices_offset{};

    glm::mat4 model{};
    GLuint texture{};

};


class Grid : public Mesh {
public:
    explicit Grid(float width, int n);

    void update(WindowWrapper& wrapper, double time_elapsed);
private:
    size_t getIndex(int x, int y) const
    {
        return y * (n + 1) + x;
    }

    float width;
    int n;

};


#endif //BEATSY_UTILS_H
