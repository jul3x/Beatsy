//
// Created by julex on 07.01.2021.
//

#include <Utils.h>
#include <FFT.h>
#include <WindowWrapper.h>

Model::Model(const std::string& obj_path) {
    loadObject(obj_path);
}

void Model::loadObject(const std::string& obj_path) {
    loadObj(obj_path.c_str(), this->vertices, this->uv, this->normals);
}

void Model::setVertices(const std::vector<glm::vec3>& vertices_) {
    this->vertices = vertices_;
}

void Model::setUV(const std::vector<glm::vec2>& uv_) {
    this->uv = uv_;
}

void Model::setNormals(const std::vector<glm::vec3>& normals_) {
    this->normals = normals_;
}

std::vector<glm::vec3>& Model::getVertices() {
    return this->vertices;
}

const std::vector<glm::vec2>& Model::getUV() const {
    return this->uv;
}

const std::vector<glm::vec3>& Model::getNormals() const {
    return this->normals;
}


Mesh::Mesh() : model(glm::mat4(1.0f)) {}

Mesh::Mesh(const std::shared_ptr<Model>& data_, GLuint texture_) {
    model = glm::mat4(1.0f);
    data = data_;
    texture = texture_;
}

void Mesh::setModel(const glm::mat4& model_) {
    this->model = model_;
}

void Mesh::setOffset(size_t indices) {
    indices_offset = indices;
}

size_t Mesh::getOffset() const {
    return indices_offset;
}

const std::vector<glm::vec3>& Mesh::getVertices() const {
    return this->data->getVertices();
}

const std::vector<glm::vec2>& Mesh::getUV() const {
    return this->data->getUV();
}

const std::vector<glm::vec3>& Mesh::getNormals() const {
    return this->data->getNormals();
}

const glm::mat4& Mesh::getModel() const {
    return this->model;
}

GLuint Mesh::getTexture() const {
    return this->texture;
}

void Mesh::update(double time_elapsed) {
    model = glm::rotate(model, static_cast<float>(time_elapsed), {0.0, 1, 0});
}


Grid::Grid(float width, int n) {
    this->width = width;
    this->n = n;
    this->data = std::make_shared<Model>();

    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec2> uvs;

    float const_n = width / (float)n;
    float min_coord = - width / 2.0f;

    for (int j=0; j<n + 1; ++j) {
        float z = min_coord + j * const_n;
        for (int i=0; i<n; ++i)
        {
            float x = min_coord + i * const_n;

            uvs.emplace_back(0, 0);
            normals.emplace_back(0, 1, 0);
            uvs.emplace_back(0, 0);
            normals.emplace_back(0, 1, 0);
            vertices.emplace_back(x, 0, z);
            vertices.emplace_back(x + const_n, 0, z);
        }
    }

    for (int j=0; j<n; ++j) {
        float z = min_coord + j * const_n;
        for (int i=0; i<n + 1; ++i)
        {
            float x = min_coord + i * const_n;

            uvs.emplace_back(0, 0);
            normals.emplace_back(0, 1, 0);
            uvs.emplace_back(0, 0);
            normals.emplace_back(0, 1, 0);
            vertices.emplace_back(x, 0, z);
            vertices.emplace_back(x, 0, z + const_n);
        }
    }

    this->data->setVertices(vertices);
    this->data->setUV(uvs);
    this->data->setNormals(normals);

}

void Grid::update(WindowWrapper& wrapper, double time_elapsed)
{
    static double x = -1.0f;
    x += time_elapsed;

    if (x <= 0.0f)
        return;

    std::tuple<const std::vector<float>&, float> new_values = applyFFT(x);
    static std::vector<float> new_values_mapped(n+1);

    static std::vector<int> range_mapping;

    if (range_mapping.empty())
    {
        double fac = std::log(22500) / std::log(n);
        for (int i = 0; i <= n; ++i)
        {
            range_mapping.push_back(std::pow(i, fac));
        }
    }

    for (int i = 0; i < n; ++i)
    {
        size_t frame_size = 44100 / 4 / n;
        frame_size = 10;
        new_values_mapped[i] = 0.0f;

        for (int j = range_mapping[i]; j < range_mapping[i+1]; ++j)
            new_values_mapped[i] += std::get<0>(new_values)[j];

        new_values_mapped[i] = std::abs(new_values_mapped[i] / frame_size);

    }

    for (int i = 0; i <= n; ++i)
    {
        float b = n / 3.0;
        float c = n / 6.0;
        float A = 0.005 * std::exp(- (i-b)*(i-b) / (2 * c*c));

        for (int j = 0; j <= n; ++j)
        {

            wrapper.updateVertexY(getIndex(i, j) + this->getOffset(), A * (std::abs((1.0 + new_values_mapped[j]))));
        }
    }

    wrapper.bindBuffers();

    model = glm::rotate(model, static_cast<float>((0.1 + 0.1 * (1.0 + std::get<1>(new_values))) * time_elapsed), {0.0, 1, 0});
}
