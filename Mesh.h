#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Shader;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture
{
	unsigned int ID;
	std::string Type; // diffuse or specular
	std::string Path;
};

class Mesh
{
public:
	static std::string DiffuseTextureName;
	static std::string SpecularTextureName;
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(const Shader& shader);

private:
	unsigned int VAO, VBO, EBO;

	void setupMesh();
};

