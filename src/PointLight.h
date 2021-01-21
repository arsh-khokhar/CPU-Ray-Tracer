#pragma once
#include "SceneLight.h"

class PointLight : public SceneLight
{
private:
	glm::vec3 position;
public:
	PointLight(glm::vec3 arg_color, glm::vec3 arg_position);
	glm::vec3 getPosition();
};