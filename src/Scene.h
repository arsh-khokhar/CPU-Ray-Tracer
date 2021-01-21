#include <glm/glm.hpp>
#include <vector>
#include "SceneObject.h"
#include "SceneLight.h"
#include "Plane.h"
#include "Sphere.h"
#include "Mesh.h"
#include "AmbientLight.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "SpotLight.h"

class Scene {
private:
	double field_of_view;
	glm::vec3 background;
	
	std::vector<SceneObject*> objects;
	std::vector<Plane*> plane_objects;
	std::vector<Sphere*> sphere_objects;
	std::vector<Mesh*> mesh_objects;

	std::vector<SceneLight*> lights;
	std::vector<AmbientLight*> ambient_lights;
	std::vector<DirectionalLight*> directional_lights;
	std::vector<PointLight*> point_lights;
	std::vector<SpotLight*> spot_lights;

public:
	Scene();
	Scene(double arg_fov, glm::vec3 arg_background);
	double getFieldOfView();
	glm::vec3 getBackground();
	
	std::vector<SceneObject*> getObjects();
	std::vector<Plane*> getPlanes();
	std::vector<Sphere*> getSpheres();
	std::vector<Mesh*> getMeshes();

	std::vector<SceneLight*> getLights();
	std::vector<AmbientLight*> getAmbientLights();
	std::vector<DirectionalLight*> getDirectionallights();
	std::vector<PointLight*> getPointLights();
	std::vector<SpotLight*> getSpotLights();

	void addPlane(Plane* toAdd);
	void addSphere(Sphere* toAdd);
	void addMesh(Mesh* toAdd);
	void addObject(SceneObject* toAdd);
	
	void addAmbientLight(AmbientLight* toAdd);
	void addDiectionalLight(DirectionalLight* toAdd);
	void addPointLight(PointLight* toAdd);
	void addSpotLight(SpotLight* toAdd);
	void addLight(SceneLight* toAdd);
};