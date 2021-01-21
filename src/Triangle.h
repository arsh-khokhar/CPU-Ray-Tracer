#pragma once
#include <glm/glm.hpp>

typedef glm::vec3 point3;

class Triangle
{
private:
	point3 a;
	point3 b;
	point3 c;
public:
	Triangle(point3 a, point3 b, point3 c);
	bool attemptHit(point3 d, point3 e, point3& hit, float& t_val, point3& normal_at_hit);
};