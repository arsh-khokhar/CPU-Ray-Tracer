#include "Plane.h"

Plane::Plane(Material arg_material, glm::vec3 arg_position, glm::vec3 arg_normal, int arg_id)
	: SceneObject(arg_material, arg_id),
	position(arg_position),
	normal(glm::normalize(arg_normal)) {}

glm::vec3 Plane::getPosition()
{
	return position;
}

glm::vec3 Plane::getNormal()
{
	return glm::normalize(normal);
}

bool Plane::attemptHit(point3 d, point3 e, point3& hit, float& t_val, point3& normal_at_hit)
{
	if (glm::dot(normal, d) == 0)
	{
		return false;
	}

	double t;
	t = glm::dot(normal, position - e) / glm::dot(normal, d);
	
	if(t < 0)
	{
		return false;
	}

	hit = e + (float)t * d;
	t_val = t;
	normal_at_hit = glm::normalize(normal);
	return true;
}