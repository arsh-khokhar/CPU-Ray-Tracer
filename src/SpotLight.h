#pragma once
#include "SceneLight.h"

class SpotLight : public SceneLight
{
private:
	glm::vec3 position;
	glm::vec3 direction;
	double cutoff;
public:
	SpotLight(glm::vec3 arg_color, glm::vec3 arg_position, glm::vec3 arg_direction, double arg_cutoff );
	glm::vec3 getPosition();
	glm::vec3 getDirection();
	double getCutoff();
};