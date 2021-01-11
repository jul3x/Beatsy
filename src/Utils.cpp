//
// Created by julex on 07.01.2021.
//

#include <Utils.h>
#include <FFT.h>
#include <WindowWrapper.h>
#include <Colors.h>

void Model::setVertices(const std::vector<glm::vec3>& vertices_) {
    this->vertices = vertices_;
}

void Model::setColors(const std::vector<glm::vec3>& colors_) {
    this->colors = colors_;
}

void Model::setNormals(const std::vector<glm::vec3>& normals_) {
    this->normals = normals_;
}

std::vector<glm::vec3>& Model::getVertices() {
    return this->vertices;
}

const std::vector<glm::vec3>& Model::getColors() const {
    return this->colors;
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

const std::vector<glm::vec3>& Mesh::getColors() const {
    return this->data->getColors();
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
    std::vector<glm::vec3> colors;

    float const_n = width / (float)n;
    float min_coord = - width / 2.0f;

    for (int j=0; j<n + 1; ++j) {
        float z = min_coord + j * const_n;
        for (int i=0; i<n; ++i)
        {
            float x = min_coord + i * const_n;

            rgb color_rgb = hsv2rgb({j / (double)(n + 1) * 180, 1.0, 1.0});

            colors.emplace_back(color_rgb.r, color_rgb.g, color_rgb.b);
            normals.emplace_back(0, 1, 0);
            colors.emplace_back(color_rgb.r, color_rgb.g, color_rgb.b);
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
            rgb color_rgb = hsv2rgb({j / (double)(n + 1) * 180, 1.0, 1.0});

            colors.emplace_back(color_rgb.r, color_rgb.g, color_rgb.b);
            normals.emplace_back(0, 1, 0);
            colors.emplace_back(color_rgb.r, color_rgb.g, color_rgb.b);
            normals.emplace_back(0, 1, 0);
            vertices.emplace_back(x, 0, z);
            vertices.emplace_back(x, 0, z + const_n);
        }
    }
//
//    for (int j=-2*n; j<5*n; ++j) {
//        float z = min_coord + j * const_n;
//        for (int i=-2 * n; i< 5*n + 1; ++i)
//        {
//            float x = min_coord + i * const_n;
//
//            colors.emplace_back(1, 0, 0);
//            normals.emplace_back(0, 1, 0);
//            vertices.emplace_back(x, 0, z);
//        }
//    }

    this->data->setVertices(vertices);
    this->data->setColors(colors);
    this->data->setNormals(normals);

}

void Grid::update(WindowWrapper& wrapper, Camera& camera, double time_elapsed)
{
    static double x = -1.2f;
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
        float b = n / 2.0;
        float c = n / 6.0;
        float A = 0.005 * std::exp(- (i-b)*(i-b) / (2 * c*c));

        for (int j = 0; j <= n; ++j)
        {
            float new_value = A * (std::abs((1.0 + new_values_mapped[j])));
            wrapper.updateVertexY(getIndex(i, j), new_value);

//            wrapper.updateVertexColor(getIndex(i, j), 0.1 + new_value, 0.1 + new_value, 0.1 + new_value);
        }
    }

    wrapper.bindBuffers();
    static glm::vec3 camera_pos = camera.getPosition();
    static std::list<float> values;

    values.push_back(std::get<1>(new_values));

    if (values.size () > 10)
        values.pop_front();

    float mean = 0.0f;
    for (float value : values)
    {
        mean += value;
    }
    mean /= values.size();

    camera.setPosition(camera_pos - camera.getDirection() * 0.002f * mean);
    model = glm::rotate(model, static_cast<float>((0.1 + 0.03 * (1.0 + mean)) * time_elapsed), {0.0, 1, 0});

}
