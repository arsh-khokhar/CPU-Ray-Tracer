#pragma once
#include "SceneObject.h"

class Material;
class Sphere : public SceneObject
{
private:
	float radius;
	glm::vec3 center_position;
public:
	Sphere(Material arg_material, glm::vec3 arg_center, float arg_radius, int arg_id);
	glm::vec3 getCenter();
	float getRadius();
	bool attemptHit(point3 d, point3 e, point3& hit, float& t_val, point3& normal_at_hit);
};