#pragma once
#include "core.h"
#include "drawable.h"
#include <vector>
#include <iostream>
#include "cube.h"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 diffuse;
	glm::vec2 texCoords;
	glm::ivec4 boneIndices;
	glm::vec4 boneWeights;

	Vertex() : boneIndices(0), boneWeights(0.0f) {}

	void addBoneData(int boneIndex, float weight)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (boneWeights[i] == 0.0f)
			{
				boneIndices[i] = boneIndex;
				boneWeights[i] = weight;
				return;
			}
		}
	}
};

struct Texture
{
	unsigned int id;
	std::string type;
};

class Mesh
{
public:
	Cube* hitbox;
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		void draw(const glm::mat4 &viewProjMtx, Shader *shader);

		setupMesh();
	}

	void draw(const glm::mat4 &viewProjMtx, Shader *shader);

private:
	// Render data
	unsigned int VAO, VBO, EBO;
	void setupMesh();
};