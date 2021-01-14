//
// Created by julex on 07.01.2021.
//

#include <VBOIndexer.h>

bool isNearlyEqual(float x, float y, float err)
{
    return std::abs(x - y) < err;
}

bool getSimilarVertexIndex(const PackedVertex &packed, std::map<PackedVertex, unsigned short> &vertices, unsigned short &result)
{
    auto it = vertices.find(packed);
    if (it == vertices.end())
    {
        return false;
    }
    else
    {
        result = it->second;
        return true;
    }
}

void indexVBO(const std::vector<glm::vec3> & in_vertices, const std::vector<glm::vec3> & in_colors, const std::vector<glm::vec3> & in_normals,
              std::vector<unsigned short> & out_indices,
              std::vector<glm::vec3> & out_vertices, std::vector<glm::vec3> & out_colors, std::vector<glm::vec3> & out_normals) {
    static std::map<PackedVertex, unsigned short> vertices_map;

    for (unsigned int i=0; i < in_vertices.size(); ++i)
    {
        PackedVertex packed = {in_vertices[i], in_colors[i], in_normals[i]};

        unsigned short index;
        if (getSimilarVertexIndex(packed, vertices_map, index))
        {
            out_indices.push_back(index);
        }
        else
        {
            out_vertices.push_back(in_vertices[i]);
            out_colors.push_back(in_colors[i]);
            out_normals.push_back(in_normals[i]);

            auto new_index = static_cast<unsigned short>(out_vertices.size() - 1);
            out_indices.push_back(new_index);
            vertices_map[packed] = new_index;
        }
    }
}