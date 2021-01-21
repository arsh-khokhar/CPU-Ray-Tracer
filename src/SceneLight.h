#pragma once
#include <glm/glm.hpp>

class SceneLight {
private:
	glm::vec3 color;
	bool valid = false;
public:
	explicit SceneLight(glm::vec3 arg_color) : color(arg_color){};
	virtual glm::vec3 getIntensity() { return color; }
};