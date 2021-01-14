//
// Created by julex on 07.01.2021.
//

#include <Utils.h>
#include <Audio.h>
#include <WindowWrapper.h>
#include <Colors.h>
#include <Config.h>

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


Grid::Grid(const Audio& audio, float width, int n) {
    this->n = n;
    this->data = std::make_shared<Model>();
    this->time_point = CONF("audio")["offset"].asFloat();

    this->cummulated_values.resize(n+1);

    double fac = std::log(audio.getSampleRate() / 2) / std::log(n);
    for (int i = 0; i <= n; ++i)
    {
        range_mapping.push_back(std::pow(i, fac));
    }

    std::vector<glm::vec3> vertices, normals;
    std::vector<glm::vec3> colors;

    float const_n = width / (float)n;
    float min_coord = - width / 2.0f;

    for (int j=0; j<n + 1; ++j) {
        float z = min_coord + j * const_n;
        for (int i=0; i<n; ++i)
        {
            float x = min_coord + i * const_n;
            rgb color_rgb;
            if (CONF("grid")["r"].asFloat() == -1)
                color_rgb = hsv2rgb({j / (double)(n + 1) * 180, 1.0, 1.0});
            else
                color_rgb = {CONF("grid")["r"].asFloat(), CONF("grid")["g"].asFloat(), CONF("grid")["b"].asFloat()};

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
            rgb color_rgb;
            if (CONF("grid")["r"].asFloat() == -1)
                color_rgb = hsv2rgb({j / (double)(n + 1) * 180, 1.0, 1.0});
            else
                color_rgb = {CONF("grid")["r"].asFloat(), CONF("grid")["g"].asFloat(), CONF("grid")["b"].asFloat()};

            colors.emplace_back(color_rgb.r, color_rgb.g, color_rgb.b);
            normals.emplace_back(0, 1, 0);
            colors.emplace_back(color_rgb.r, color_rgb.g, color_rgb.b);
            normals.emplace_back(0, 1, 0);
            vertices.emplace_back(x, 0, z);
            vertices.emplace_back(x, 0, z + const_n);
        }
    }

    if (CONF("grid")["border"].asString() == "const") {
        for (int j=-2*n; j<5*n; ++j) {
            float z = min_coord + j * const_n;
            for (int i=-2 * n; i< 5*n + 1; ++i)
            {
                rgb color_rgb;
                if (CONF("grid")["r"].asFloat() == -1)
                    color_rgb = hsv2rgb({ std::min(std::max(0, j) / (double)(n + 1) * 180, 360.0), 1.0, 1.0});
                else
                    color_rgb = {CONF("grid")["r"].asFloat(), CONF("grid")["g"].asFloat(), CONF("grid")["b"].asFloat()};
                float x = min_coord + i * const_n;

                colors.emplace_back(color_rgb.r, color_rgb.g, color_rgb.b);
                normals.emplace_back(0, 1, 0);
                vertices.emplace_back(x, 0, z);
            }
        }
    }

    this->data->setVertices(vertices);
    this->data->setColors(colors);
    this->data->setNormals(normals);

}

void Grid::update(WindowWrapper& wrapper, Camera& camera, Audio& audio, double time_elapsed)
{
    time_point += time_elapsed;

    std::tuple<const std::vector<float>&, float> new_values = audio.getAnalizedData(time_point);

    for (int i = 0; i < n; ++i)
    {
        size_t frame_size = 10;
        cummulated_values[i] = 0.0f;

        for (int j = range_mapping[i]; j < range_mapping[i+1]; ++j)
            cummulated_values[i] += std::get<0>(new_values)[j];

        cummulated_values[i] = std::abs(cummulated_values[i] / frame_size);
    }

    for (int i = 0; i <= n; ++i)
    {
        float b = n / 2.0;
        float c = n * CONF("audio")["spread_factor"].asFloat();
        float A = CONF("audio")["height"].asFloat() * std::exp(- (i-b)*(i-b) / (2 * c*c));

        for (int j = 0; j <= n; ++j)
        {
            float new_value = A * (std::abs((1.0 + cummulated_values[j])));
            wrapper.updateVertexY(getIndex(i, j), new_value);

            if (CONF("grid")["border"].asString() == "fade") {
                rgb color_rgb;
                if (CONF("grid")["r"].asFloat() == -1)
                    color_rgb = hsv2rgb({ std::min(std::max(0, j) / (double)(n + 1) * 180, 360.0), 1.0, new_value});
                else
                    color_rgb = {new_value * CONF("grid")["r"].asFloat(),
                                 new_value * CONF("grid")["g"].asFloat(),
                                 new_value * CONF("grid")["b"].asFloat()};
                wrapper.updateVertexColor(getIndex(i, j), color_rgb.r,  color_rgb.g,  color_rgb.b);
            }
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

    if (CONF("camera")["lock"].asBool())
    {
        camera.setPosition(camera_pos - camera.getDirection() * CONF("camera")["movement"]["zoom_speed"].asFloat() * mean);
        model = glm::rotate(model, static_cast<float>((CONF("camera")["movement"]["base_rot_speed"].asFloat() +
           CONF("camera")["movement"]["rot_speed"].asFloat() * (1.0 + mean)) * time_elapsed), {0.0, 1, 0});
    }
}
