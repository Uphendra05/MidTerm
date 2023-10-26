#pragma once
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include <iostream>

#include<glm/glm.hpp>
#include "Model.h"
#include "PhysicsObject.h"
#include <string>
class PlayerInput
{
public:
	PlayerInput();
	~PlayerInput();

	int health = 100;
	PhysicsObject* playerPhysics;
	Model* playerModel;
	std::string playerName = "Player";
	void MovePlayer(GLFWwindow* window, glm::vec3& pos);

	bool Health();

private:

	class InputHandler;

	InputHandler* inputPtr;


};



