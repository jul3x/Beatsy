#include <cmath>
#include <vector>
#include <map>

#include <glm/glm.hpp>

#include <string.h> // for memcmp


// TODO - comparator may be a problem

bool isNearlyEqual(float x, float y, float err);

struct PackedVertex{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
	bool operator<(const PackedVertex& that) const{
//		return memcmp((void*)this, (void*)&that, sizeof(PackedVertex))>0;
        static constexpr float ERR = 0.01;

		return (position.x + ERR < that.position.x) || (isNearlyEqual(position.x, that.position.x, ERR) && position.y + ERR < that.position.y) ||
                (isNearlyEqual(position.x, that.position.x, ERR) && isNearlyEqual(position.y, that.position.y, ERR) && position.z + ERR < that.position.z) ;
	};
};

bool getSimilarVertexIndex_fast( 
	PackedVertex & packed, 
	std::map<PackedVertex,unsigned short> & VertexToOutIndex,
	unsigned short & result
);

void indexVBO(const std::vector<glm::vec3> & in_vertices,
              const std::vector<glm::vec3> & in_colors,
              const std::vector<glm::vec3> & in_normals,
              std::vector<unsigned short> & out_indices,
              std::vector<glm::vec3> & out_vertices,
              std::vector<glm::vec3> & out_colors,
              std::vector<glm::vec3> & out_normals);
