#pragma once
#include "SceneObject.h"
#include "Triangle.h"

class Material;
class Mesh : public SceneObject
{
private:
	std::vector<Triangle*> triangles;
public:
	Mesh(Material arg_material, std::vector<Triangle*> arg_triangles, int arg_id);
	std::vector<Triangle*> getTriangles();
	bool attemptHit(point3 d, point3 e, point3& hit, float& t_val, point3& normal_at_hit);
};