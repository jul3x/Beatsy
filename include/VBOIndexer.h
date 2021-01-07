#include <cmath>
#include <vector>
#include <map>

#include <glm/glm.hpp>

#include <string.h> // for memcmp


// TODO - comparator may be a problem

struct PackedVertex{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
	bool operator<(const PackedVertex& that) const{
		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
	};
};

bool getSimilarVertexIndex_fast( 
	PackedVertex & packed, 
	std::map<PackedVertex,unsigned short> & VertexToOutIndex,
	unsigned short & result
);

void indexVBO(const std::vector<glm::vec3> & in_vertices,
              const std::vector<glm::vec2> & in_uvs,
              const std::vector<glm::vec3> & in_normals,
              std::vector<unsigned short> & out_indices,
              std::vector<glm::vec3> & out_vertices,
              std::vector<glm::vec2> & out_uvs,
              std::vector<glm::vec3> & out_normals);
