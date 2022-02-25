#include "object3D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object3D::Box(
	const std::string& name,
	glm::vec3 leftBottomCorner,
	float length,
	float width,
	float height,
	glm::vec3 color)
{
	glm::vec3 corner = glm::vec3(0, 0, 0);

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner + glm::vec3(0, 0,  0), color),
		VertexFormat(corner + glm::vec3(0, height,  0), color),
		VertexFormat(corner + glm::vec3(length, 0,  0), color),
		VertexFormat(corner + glm::vec3(length, height,  0), color),

		VertexFormat(corner + glm::vec3(0, 0,  width), color),
		VertexFormat(corner + glm::vec3(0, height,  width), color),
		VertexFormat(corner + glm::vec3(length, 0,  width), color),
		VertexFormat(corner + glm::vec3(length, height,  width), color),
	};

	std::vector<unsigned int> indices =
	{
		0, 2, 1,
		2, 3, 1,
		4, 7, 6,
		4, 5, 7,
		2, 6, 7,
		2, 7, 3,
		7, 2, 6,
		1, 5, 4,
		0, 1, 4,
		1, 3, 7,
		1, 7, 5,
		0, 2, 6,
		0, 6, 4
	};

	Mesh* box = new Mesh(name);

	box->InitFromData(vertices, indices);
	return box;
}
