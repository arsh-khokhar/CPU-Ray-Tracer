#include "SpotLight.h"

SpotLight::SpotLight(glm::vec3 arg_color, glm::vec3 arg_position, glm::vec3 arg_direction, double arg_cutoff)
	: SceneLight(arg_color),
	position(arg_position),
	direction(arg_direction),
	cutoff(arg_cutoff){}

glm::vec3 SpotLight::getPosition()
{
	return position;
}

glm::vec3 SpotLight::getDirection()
{
	return direction;
}

double SpotLight::getCutoff()
{
	return cutoff;
}