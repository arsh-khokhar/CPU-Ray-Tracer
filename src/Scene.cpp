#include "Scene.h"

Scene::Scene(double arg_fov, glm::vec3 arg_background) :
	field_of_view(arg_fov),
	background(arg_background) {}

Scene::Scene()
{
	field_of_view = 60;
	background = colour3(0, 0, 0);
}

double Scene::getFieldOfView()
{
	return field_of_view;
}

glm::vec3 Scene::getBackground()
{
	return background;
}

std::vector<SceneObject*> Scene::getObjects()
{
	return objects;
}

std::vector<Plane*> Scene::getPlanes()
{
	return plane_objects;
}

std::vector<Sphere*> Scene::getSpheres()
{
	return sphere_objects;
}

std::vector<Mesh*> Scene::getMeshes()
{
	return mesh_objects;
}

void Scene::addPlane(Plane* toAdd)
{
	plane_objects.push_back(toAdd);
}

void Scene::addSphere(Sphere* toAdd)
{
	sphere_objects.push_back(toAdd);
}

void Scene::addMesh(Mesh* toAdd)
{
	mesh_objects.push_back(toAdd);
}

void Scene::addObject(SceneObject* toAdd)
{
	objects.push_back(toAdd);
}

std::vector<SceneLight*> Scene::getLights()
{
	return lights;
}

std::vector<AmbientLight*> Scene::getAmbientLights()
{
	return ambient_lights;
}

std::vector<DirectionalLight*> Scene::getDirectionallights()
{
	return directional_lights;
}

std::vector<PointLight*> Scene::getPointLights()
{
	return point_lights;
}

std::vector<SpotLight*> Scene::getSpotLights()
{
	return spot_lights;
}

void Scene::addAmbientLight(AmbientLight* toAdd)
{
	ambient_lights.push_back(toAdd);
}

void Scene::addDiectionalLight(DirectionalLight* toAdd)
{
	directional_lights.push_back(toAdd);
}

void Scene::addPointLight(PointLight* toAdd)
{
	point_lights.push_back(toAdd);
}

void Scene::addSpotLight(SpotLight* toAdd)
{
	spot_lights.push_back(toAdd);
}

void Scene::addLight(SceneLight* toAdd)
{
	lights.push_back(toAdd);
}
