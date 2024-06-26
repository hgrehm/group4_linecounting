#pragma once
#include "core.h"
#include "drawable.h"
#include "shader.h"
#include <vector>

class Cube : public Drawable {
private:
	GLuint VAO;
	GLuint VBO_positions, VBO_normals, EBO;

	// cube information
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<unsigned int> indices;

public:
	glm::vec3 cubeMin;
	glm::vec3 cubeMax;
	Cube(glm::vec3 cubeMin = glm::vec3(-1, -1, -1), glm::vec3 cubeMax = glm::vec3(1, 1, 1));
	~Cube();

	void print_world();
	void draw(const glm::mat4& viewProjMtx, Shader* shader);

};