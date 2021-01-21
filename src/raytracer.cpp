// The JSON library allows you to reference JSON arrays like C++ vectors and JSON objects like C++ maps.

#include "raytracer.h"

#include <iostream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Scene.h"
#include "SceneObject.h"
#include "Sphere.h"
#include "Plane.h"

#include "SceneLight.h"
#include "AmbientLight.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "json.hpp"

using json = nlohmann::json;

const char *PATH = "scenes/";

double fov = 60;
colour3 background_colour(0, 0, 0);
int id = 0;
int recursion_depth = 20;
int trans_recursion_depth = 20;
json scene;

Scene currScene;

void buildScene(json scene_json);
void addSceneObject(json object_json);
void addSceneLight(json light_json);
SceneObject* findNearestObject(const point3& e, const point3& s, point3& final_hitPoint, point3& final_hitNormal, float& final_t);
void calculateLighting(SceneObject* object, const point3& viewer_pos, const point3& hitPoint, const point3& hitNormal, colour3& final_colour);
void reflect(SceneObject* object, const point3& ray_dir, const point3& hitPoint, const point3& hitNormal, colour3& reflected_colour);
void transmit(SceneObject* object, const point3& ray_dir, const point3& hitPoint, const point3& hitNormal, colour3& transmitted_colour);
template<typename Base, typename T>
inline bool instanceof(const T*) {
	return std::is_base_of<Base, T>::value;
}

json find(json &j, const std::string key, const std::string value) {
	json::iterator it;
	for (it = j.begin(); it != j.end(); ++it) {
		if (it->find(key) != it->end()) {
			if ((*it)[key] == value) {
				return *it;
			}
		}
	}
	return json();
}

glm::vec3 vector_to_vec3(const std::vector<float> &v) {
	return glm::vec3(v[0], v[1], v[2]);
}

void choose_scene(char const *fn) {
	if (fn == NULL) {
		std::cout << "Using default input file " << PATH << "c.json\n";
		fn = "h";
	}

	std::cout << "Loading scene " << fn << std::endl;
	
	std::string fname = PATH + std::string(fn) + ".json";
	std::fstream in(fname);
	if (!in.is_open()) {
		std::cout << "Unable to open scene file " << fname << std::endl;
		exit(EXIT_FAILURE);
	}
	
	in >> scene;
	
	buildScene(scene);
}

bool trace(const point3 &e, const point3 &s, colour3 &colour, bool pick) 
{
	point3 hitPoint;
	point3 hitNormal;
	float hitT;
	SceneObject* nearestObject = findNearestObject(e, s, hitPoint, hitNormal, hitT);
	
	if (nearestObject == nullptr)
	{
		return false;
	}

	calculateLighting(nearestObject, e, hitPoint, hitNormal, colour);

	reflect(nearestObject, s - e, hitPoint, hitNormal, colour);
	transmit(nearestObject, s - e, hitPoint, hitNormal, colour);
	return true;
}

SceneObject* findNearestObject(const point3& e, const point3& s, point3& final_hitPoint, point3& final_hitNormal, float& final_t)
{
	float min_t_found = 10000000.0f;
	point3 nearestHitPoint;
	point3 normalAtHit;
	SceneObject* toReturn = nullptr;
	std::vector<SceneObject*> objects = currScene.getObjects();
	for (int i = 0; i < objects.size(); i++)
	{
		float temp_t;
		point3 hitpoint;
		point3 temp_normal;
		if (dynamic_cast<Sphere*>(objects[i]) != NULL)
		{
			Sphere* sphere = dynamic_cast<Sphere*>(objects[i]);
			if (sphere->attemptHit(s - e, e, hitpoint, temp_t, temp_normal) && temp_t < min_t_found)
			{
				min_t_found = temp_t;
				toReturn = sphere;
				nearestHitPoint = hitpoint;
				normalAtHit = temp_normal;
			}
		}
		else if (dynamic_cast<Plane*>(objects[i]) != NULL)
		{
			Plane* plane = dynamic_cast<Plane*>(objects[i]);
			if (plane->attemptHit(s - e, e, hitpoint, temp_t, temp_normal) && temp_t < min_t_found)
			{
				min_t_found = temp_t;
				toReturn = plane;
				nearestHitPoint = hitpoint;
				normalAtHit = temp_normal;
			}
		}
		else if (dynamic_cast<Mesh*>(objects[i]) != NULL)
		{
			Mesh* mesh = dynamic_cast<Mesh*>(objects[i]);
			if (mesh->attemptHit(s - e, e, hitpoint, temp_t, temp_normal) && temp_t < min_t_found)
			{
				min_t_found = temp_t;
				toReturn = mesh;
				nearestHitPoint = hitpoint;
				normalAtHit = temp_normal;
			}
		}
	}

	final_hitNormal = glm::normalize(normalAtHit);
	final_hitPoint = nearestHitPoint;
	final_t = min_t_found;
	return toReturn;
}

void calculateLighting(SceneObject* object, const point3& viewer_pos,const point3& hitPoint,const point3& hitNormal, colour3& final_colour)
{
	if (object == nullptr)
	{
		return;
	}
	glm::vec3 Ka = object->getMaterial().ambient;
	glm::vec3 Kd = object->getMaterial().diffuse;
	glm::vec3 Ks = object->getMaterial().specular;
	float alpha = object->getMaterial().shininess;

	colour3 ambient_colour;
	colour3 diffuse_colour;
	colour3 specular_colour;

	std::vector<SceneLight*> lights = currScene.getLights();

	SceneObject* foundShadowedObj = nullptr;
	point3 foundHitPos;
	point3 foundHitNormal;

	for (int i = 0; i < lights.size(); i++)
	{
		if (dynamic_cast<AmbientLight*>(lights[i]) != NULL)
		{
			AmbientLight* ambientLight = dynamic_cast<AmbientLight*>(lights[i]);
			ambient_colour += ambientLight->getIntensity() * Ka;
		}
		else if (dynamic_cast<DirectionalLight*>(lights[i]) != NULL)
		{
			bool isShadowed = false;
			DirectionalLight* directionalLight = dynamic_cast<DirectionalLight*>(lights[i]);
			point3 L = glm::normalize(-directionalLight->getDirection());
			
			point3 newHitPos;
			point3 newHitNormal;
			float checkT;
			SceneObject* checkShadowingObj = findNearestObject(hitPoint, hitPoint + directionalLight->getDirection(), newHitPos, newHitNormal, checkT);
			
			if (checkShadowingObj != nullptr && dynamic_cast<Plane*>(checkShadowingObj) == NULL && checkShadowingObj->getId() != object->getId())
			{
				isShadowed = true;
				foundShadowedObj = checkShadowingObj;
				foundHitPos = newHitPos;
				foundHitNormal = newHitNormal;
			}

			if (!isShadowed)
			{
				diffuse_colour += glm::clamp(directionalLight->getIntensity() * Kd * glm::max(glm::dot(hitNormal, L), 0.0f), colour3(0, 0, 0), colour3(1, 1, 1));
				glm::vec3 R = glm::normalize(2 * glm::dot(hitNormal, L) * hitNormal - L);
				glm::vec3 V = glm::normalize(viewer_pos - hitPoint);
				specular_colour += glm::clamp(directionalLight->getIntensity() * Ks * glm::pow(glm::dot(R, V), alpha), colour3(0, 0, 0), colour3(1, 1, 1));
			}
		}
		else if (dynamic_cast<PointLight*>(lights[i]) != NULL)
		{
			bool  isShadowed = false;
			PointLight* pointLight = dynamic_cast<PointLight*>(lights[i]);
			point3 L = glm::normalize(pointLight->getPosition() - hitPoint);

			point3 newHitPos;
			point3 newHitNormal;
			float checkT;
			SceneObject* checkShadowingObj = findNearestObject(hitPoint, pointLight->getPosition(), newHitPos, newHitNormal, checkT);

			if (checkShadowingObj != nullptr && dynamic_cast<Plane*>(checkShadowingObj) == NULL && checkShadowingObj->getId() != object->getId())
			{
				isShadowed = true;
				foundShadowedObj = checkShadowingObj;
				foundHitPos = newHitPos;
				foundHitNormal = newHitNormal;
			}

			if (!isShadowed)
			{
				diffuse_colour += glm::clamp(pointLight->getIntensity() * Kd * glm::max(glm::dot(hitNormal, L), 0.0f), colour3(0, 0, 0), colour3(1, 1, 1));
				glm::vec3 R = glm::normalize(2 * glm::dot(hitNormal, L) * hitNormal - L);
				glm::vec3 V = glm::normalize(viewer_pos - hitPoint);
				specular_colour += glm::clamp(pointLight->getIntensity() * Ks * glm::pow(glm::dot(R, V), alpha), colour3(0,0,0), colour3(1,1,1));
			}
		}
		else if (dynamic_cast<SpotLight*>(lights[i]) != NULL)
		{
			bool isShadowed = false;
			SpotLight* spotLight = dynamic_cast<SpotLight*>(lights[i]);
			point3 L = glm::normalize(spotLight->getPosition() - hitPoint);
			point3 newHitPos;
			point3 newHitNormal;
			float checkT;
			SceneObject* checkShadowingObj = findNearestObject(hitPoint, spotLight->getPosition(), newHitPos, newHitNormal, checkT);
			
			if (checkShadowingObj != nullptr && dynamic_cast<Plane*>(checkShadowingObj) == NULL && checkShadowingObj->getId() != object->getId())
			{
				isShadowed = true;
				foundShadowedObj = checkShadowingObj;
				foundHitPos = newHitPos;
				foundHitNormal = newHitNormal;
			}

			if (!isShadowed && glm::dot(L, spotLight->getDirection()) > glm::cos(glm::radians(spotLight->getCutoff())))
			{
				diffuse_colour += glm::clamp(spotLight->getIntensity() * Kd * glm::max(glm::dot(hitNormal, L), 0.0f), colour3(0, 0, 0), colour3(1, 1, 1));
				glm::vec3 R = glm::normalize(2 * glm::dot(hitNormal, L) * hitNormal - L);
				glm::vec3 V = glm::normalize(viewer_pos - hitPoint);
				specular_colour += glm::clamp(spotLight->getIntensity() * Ks * glm::pow(glm::dot(R, V), alpha), colour3(0, 0, 0), colour3(1, 1, 1));
			}
		}
	}

	final_colour = glm::clamp(ambient_colour + diffuse_colour + specular_colour, colour3(0,0,0), colour3(1,1,1));
}
void transmit(SceneObject* object, const point3& ray_dir, const point3& hitPoint, const point3& hitNormal, colour3& transmitted_colour)
{
	if (object == nullptr)
	{
		return;
	}
	glm::vec3 transmissitivity = object->getMaterial().transmissive;
	if (glm::length(transmissitivity) == 0 || trans_recursion_depth <= 0)
	{
		return;
	}

	point3 newHitPos;
	point3 newHitNormal;
	float newTVal;
	SceneObject* newObject;
	glm::vec3 R = glm::normalize(2 * glm::dot(hitNormal, ray_dir) * hitNormal - ray_dir);
	newObject = findNearestObject(hitPoint, R, newHitPos, newHitNormal, newTVal);
	colour3 transmitted;
	calculateLighting(newObject, R, newHitPos, newHitNormal, transmitted);
	trans_recursion_depth--;
	transmit(newObject, R, newHitPos, newHitNormal, transmitted);
	colour3 initial_colour = transmitted_colour;
	transmitted_colour *= transmissitivity;
	transmitted_colour += transmitted * (colour3(1,1,1) - transmissitivity);
}

void reflect(SceneObject* object, const point3& ray_dir, const point3& hitPoint, const point3& hitNormal, colour3& reflected_colour)
{
	if (object == nullptr)
	{
		return;
	}
	glm::vec3 reflectivity = object->getMaterial().reflective;
	if (glm::length(reflectivity) == 0 || recursion_depth <= 0)
	{
		return;
	}

	point3 newHitPos;
	point3 newHitNormal;
	float newTVal;
	SceneObject* newObject;
	glm::vec3 R = glm::normalize(2 * glm::dot(hitNormal, ray_dir) * hitNormal - ray_dir);
	newObject = findNearestObject(hitPoint, R, newHitPos, newHitNormal, newTVal);
	colour3 reflected;
	calculateLighting(newObject, R, newHitPos, newHitNormal, reflected);
	recursion_depth--;
	reflect(newObject, R, newHitPos, newHitNormal, reflected);
	reflected_colour += reflected;
}

void buildScene(json scene_json)
{
	json camera = scene_json["camera"];
	// these are optional parameters (otherwise they default to the values initialized earlier)
	if (camera.find("field") != camera.end()) {
		fov = camera["field"];
		std::cout << "Setting fov to " << fov << " degrees.\n";
	}
	if (camera.find("background") != camera.end()) {
		background_colour = vector_to_vec3(camera["background"]);
		std::cout << "Setting background colour to " << glm::to_string(background_colour) << std::endl;
	}

	currScene = Scene(fov, background_colour);

	json& objects = scene["objects"];
	for (json::iterator o_it = objects.begin(); o_it != objects.end(); ++o_it) {
		json& object = *o_it;
		addSceneObject(object);
	}

	json& lights = scene["lights"];
	for (json::iterator l_it = lights.begin(); l_it != lights.end(); ++l_it) {
		json& light = *l_it;
		addSceneLight(light);
	}
}

void addSceneObject(json object_json)
{
	Material m;
	json& material_json = object_json["material"];
	if (material_json.find("ambient") != material_json.end())
	{
		m.ambient = vector_to_vec3(material_json["ambient"]);
	}
	if (material_json.find("diffuse") != material_json.end())
	{
		m.diffuse = vector_to_vec3(material_json["diffuse"]);
	}
	if (material_json.find("specular") != material_json.end())
	{
		m.specular = vector_to_vec3(material_json["specular"]);
	}
	if (material_json.find("transmissive") != material_json.end())
	{
		m.transmissive = vector_to_vec3(material_json["transmissive"]);
	}
	if (material_json.find("reflective") != material_json.end())
	{
		m.reflective = vector_to_vec3(material_json["reflective"]);
	}
	if (material_json.find("refraction") != material_json.end())
	{
		m.refraction = float(material_json["refraction"]);
	}
	if (material_json.find("shininess") != material_json.end())
	{
		m.shininess = float(material_json["shininess"]);
	}

	if (object_json["type"] == "sphere")
	{
		glm::vec3 center = vector_to_vec3(object_json["position"]);
		float radius = float(object_json["radius"]);
		currScene.addObject(new Sphere(m, center, radius, id++));
	}

	else if (object_json["type"] == "plane")
	{
		glm::vec3 position = vector_to_vec3(object_json["position"]);
		glm::vec3 normal = vector_to_vec3(object_json["normal"]);
		currScene.addObject(new Plane(m, position, normal, id++));
	}

	else if (object_json["type"] == "mesh")
	{
		std::vector<Triangle*> mesh_triangles;

		json& triangles = object_json["triangles"];
		for (json::iterator it = triangles.begin(); it != triangles.end(); ++it) {
			json& triangle = *it;
			mesh_triangles.push_back(new Triangle(vector_to_vec3(triangle[0]), vector_to_vec3(triangle[1]), vector_to_vec3(triangle[2])));
		}
		currScene.addObject(new Mesh(m, mesh_triangles, id++));
	}
}

void addSceneLight(json light_json)
{
	glm::vec3 colour = vector_to_vec3(light_json["color"]);
	if (light_json["type"] == "ambient")
	{
		currScene.addLight(new AmbientLight(colour));
	}
	else if (light_json["type"] == "directional")
	{
		glm::vec3 direction = vector_to_vec3(light_json["direction"]);
		currScene.addLight(new DirectionalLight(colour, direction));
	}
	else if (light_json["type"] == "point")
	{
		glm::vec3 position = vector_to_vec3(light_json["position"]);
		currScene.addLight(new PointLight(colour, position));
	}
	else if (light_json["type"] == "spot")
	{
		glm::vec3 direction = vector_to_vec3(light_json["direction"]);
		glm::vec3 position = vector_to_vec3(light_json["position"]);
		double cutoff = double(light_json["cutoff"]);
		currScene.addLight(new SpotLight(colour, position, direction, cutoff));
	}
}