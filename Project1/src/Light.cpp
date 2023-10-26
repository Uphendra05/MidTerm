#include "Light.h"


Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 emissive, Camera camera)
{
	this->position = position;
	this->ambient = ambient;
	this->diffuse = diffuse;
	this->specular = specular;
	//this->emissive = emissive;
    this->camera = camera;

}

Light::~Light()
{



}



void Light::SetLightProperties(Shader& shader)
{
    shader.setVec3("light.position", this->position);
    shader.setVec3("viewPos", this->camera.Position);
    shader.setVec3("light.ambient", this->ambient);
    shader.setVec3("light.diffuse", this->diffuse);
    shader.setVec3("light.specular", this->specular);
   // shader.setVec3("light.emissive", this->emissive);



}