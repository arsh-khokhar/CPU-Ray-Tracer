#pragma once
#include "Sphere.h"
#include "Mesh.h"

Mesh::Mesh(Material arg_material, std::vector<Triangle*> arg_triangles, int arg_id)
	: SceneObject(arg_material, arg_id),
	triangles(arg_triangles){}

std::vector<Triangle*> Mesh::getTriangles()
{
	return triangles;
}

bool Mesh::attemptHit(point3 d, point3 e, point3& hit, float& t_val, point3& normal_at_hit)
{
	float min_t_found = 1000000.0f;
	point3 foundHitPoint;
	point3 normalAtHitFound;
	bool wasTriangleHit = false;
	for (int i = 0; i < triangles.size(); i++)
	{
		float temp_t;
		point3 temp_normal;
		point3 hitpoint;
		if (triangles[i]->attemptHit(d, e, hitpoint, temp_t, temp_normal) && temp_t < min_t_found)
		{
			wasTriangleHit = true;
			foundHitPoint = hitpoint;
			min_t_found = temp_t;
			normalAtHitFound = temp_normal;
		}
	}

	if (wasTriangleHit)
	{
		hit = foundHitPoint;
		t_val = min_t_found;
		normal_at_hit = normalAtHitFound;
		return true;
	}
	return false;
}
