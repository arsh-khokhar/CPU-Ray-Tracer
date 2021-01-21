#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(glm::vec3 arg_color, glm::vec3 arg_direction)
	: SceneLight(arg_color),
	direction(arg_direction) {}

glm::vec3 DirectionalLight::getDirection()
{
	return direction;
}