#pragma once
#include "Sphere.h"

Sphere::Sphere(Material arg_material, glm::vec3 arg_center, float arg_radius, int arg_id)
	: SceneObject(arg_material, arg_id),
	radius(arg_radius),
	center_position(arg_center) {}

float Sphere::getRadius()
{
	return radius;
}

glm::vec3 Sphere::getCenter()
{
	return center_position;
}

bool Sphere::attemptHit(point3 d, point3 e, point3& hit, float& t_val, point3& normal_at_hit)
{
	float disc = std::pow(glm::dot(d, e - center_position), 2.0) - (glm::dot(d, d) * (glm::dot(e - center_position, e - center_position) - std::pow(radius, 2.0)));
	if (disc < 0)
	{
		return false;
	}

	float rest = glm::dot(-d, e - center_position) / glm::dot(d, d);
	float sqrtdisc = sqrt(disc);
	float t;
	if (disc > 0)
	{
		float t1 = (rest + sqrtdisc) / glm::dot(d, d);
		float t2 = (rest - sqrtdisc) / glm::dot(d, d);

		if (t1 < t2)
		{
			t = t1;
		}
		else
		{
			t = t2;
		}
	}
	else
	{
		t = rest;
	}

	hit = e + (float)t * d;
	t_val = t;
	normal_at_hit = glm::normalize(hit - center_position);
	return true;
}