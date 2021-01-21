#pragma once
#include <glm/glm.hpp>
#include <vector>

typedef glm::vec3 point3;
typedef glm::vec3 colour3;

class Material 
{
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 transmissive;
	glm::vec3 reflective;
	float refraction;
	float shininess;

	Material()
	{
		ambient = glm::vec3(0.0, 0.0, 0.0);
		diffuse = glm::vec3(0.0, 0.0, 0.0);
		specular = glm::vec3(0.0, 0.0, 0.0);
		transmissive = glm::vec3(0.0, 0.0, 0.0);
		reflective = glm::vec3(0.0, 0.0, 0.0);
		refraction = 0.0f;
		shininess = 0.0f;
	}
};

class SceneObject {

private:
	Material material;
	int id;
public:
	SceneObject(Material arg_material, int arg_id) : material(arg_material), id(arg_id) { };
	virtual Material getMaterial() { return material; };
	virtual int getId() { return id; };
};