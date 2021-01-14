//
// Created by julex on 03.11.2020.
//

#ifndef BEATSY_UTILS_H
#define BEATSY_UTILS_H

#include <vector>
#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include <Audio.h>
#include <Camera.h>

class WindowWrapper;

class Model {
public:
    Model() = default;

    void setVertices(const std::vector<glm::vec3>& vertices_);
    void setColors(const std::vector<glm::vec3>& color_);
    void setNormals(const std::vector<glm::vec3>& normals_);
    std::vector<glm::vec3>& getVertices();
    [[nodiscard]] const std::vector<glm::vec3>& getColors() const;
    [[nodiscard]] const std::vector<glm::vec3>& getNormals() const;
private:
    std::vector<glm::vec3> vertices{}, normals{};
    std::vector<glm::vec3> colors{};
};


class Mesh {
public:
    Mesh();

    explicit Mesh(const std::shared_ptr<Model>& data_, GLuint texture_);
    void setModel(const glm::mat4& model_);

    void setOffset(size_t indices);

    [[nodiscard]] size_t getOffset() const;
    [[nodiscard]] const std::vector<glm::vec3>& getVertices() const;
    [[nodiscard]] const std::vector<glm::vec3>& getColors() const;
    [[nodiscard]] const std::vector<glm::vec3>& getNormals() const;
    [[nodiscard]] const glm::mat4& getModel() const;
    [[nodiscard]] GLuint getTexture() const;

    void update(double time_elapsed);

protected:
    std::shared_ptr<Model> data;
    glm::mat4 model{};
private:
    size_t indices_offset{};

    GLuint texture{};

};


class Grid : public Mesh {
public:
    explicit Grid(const Audio& audio, float width, int n);

    void update(WindowWrapper& wrapper, Camera& camera, Audio& audio, double time_elapsed);
private:
    [[nodiscard]] size_t getIndex(int x, int y) const
    {
        return y * (n + 1) + x;
    }

    double time_point;
    int n;

    std::vector<float> cummulated_values;
    std::vector<int> range_mapping;

};


#endif //BEATSY_UTILS_H
