#include "PointLight.h"

PointLight::PointLight(glm::vec3 arg_color, glm::vec3 arg_position) 
	: SceneLight(arg_color), 
	position(arg_position){}

glm::vec3 PointLight::getPosition()
{
	return position;
}