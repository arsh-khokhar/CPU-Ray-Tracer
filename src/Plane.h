#pragma once
#include "SceneObject.h"

class Material;
class Plane : public SceneObject
{
private:
	glm::vec3 position;
	glm::vec3 normal;
public:
	Plane(Material arg_material, glm::vec3 arg_position, glm::vec3 arg_normal, int arg_id);
	glm::vec3 getPosition();
	glm::vec3 getNormal();
	bool attemptHit(point3 d, point3 e, point3& hit, float& t_val, point3& normal_at_hit);
};