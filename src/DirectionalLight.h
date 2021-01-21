#pragma once
#include "SceneLight.h"

class DirectionalLight : public SceneLight
{
private:
	glm::vec3 direction;
public:
	DirectionalLight(glm::vec3 arg_color, glm::vec3 arg_direction);
	glm::vec3 getDirection();
};