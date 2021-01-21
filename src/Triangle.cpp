#pragma once
#include "Triangle.h"
#include <glm/glm.hpp>

Triangle::Triangle(point3 a, point3 b, point3 c)
	: a(a),
	b(b),
	c(c) {}

bool Triangle::attemptHit(point3 d, point3 e, point3& hit, float& t_val, point3& normal_at_hit)
{
	glm::vec3 normal = glm::normalize(glm::cross(c - b, a - c));
	if (glm::dot(normal, d) == 0)
	{
		return false;
	}

	double t;
	t = glm::dot(normal, a - e) / glm::dot(normal, d);

	if (t < 0)
	{
		return false;
	}

	glm::vec3 temp_hit = e + (float)t * d;

	float dotProd_one = glm::dot(glm::cross(b - a, temp_hit - a), normal);
	float dotProd_two = glm::dot(glm::cross(c - b, temp_hit - b), normal);
	float dotProd_three = glm::dot(glm::cross(a - c, temp_hit - c), normal);

	if((dotProd_one > 0 && dotProd_two > 0 && dotProd_three > 0) || (dotProd_one < 0 && dotProd_two < 0 && dotProd_three < 0))
	{
		hit = temp_hit;
		t_val = t;
		normal_at_hit = normal;
		return true;
	}
	return false;
}