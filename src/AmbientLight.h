#pragma once
#include "SceneLight.h"

class AmbientLight : public SceneLight
{
public:
	AmbientLight(glm::vec3 arg_color);
};