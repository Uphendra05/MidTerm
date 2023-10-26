#pragma once
#include "Shader.h"
#include "Camera.h"


class Light
{

private:

    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 emissive;
    Camera camera;

public:

    Light(glm::vec3 position, glm::vec3 ambient,
        glm::vec3 diffuse,
        glm::vec3 specular,
        glm::vec3 emissive, Camera camera
    );

    ~Light();


    void SetLightProperties(Shader& shader);
    


};