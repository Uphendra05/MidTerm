
#include "PlayyerInput.h"



class PlayerInput::InputHandler
{

public: 

	void _PlayerMove(GLFWwindow* window, glm::vec3& pos, Model* playerModel)
	{
		
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			pos.z += 0.1f;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			pos.z -= 0.1f;

		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			pos.x += 0.1f;

		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			pos.x -= 0.1f;

		}
		playerModel->transform.position = pos;
	}
 

};

PlayerInput::PlayerInput()
{
	

}

PlayerInput::~PlayerInput()
{

	delete inputPtr;

}

void PlayerInput::MovePlayer(GLFWwindow* window, glm::vec3& pos)
{
	
	inputPtr->_PlayerMove(window,pos, playerPhysics->model);
	
}

bool PlayerInput::Health()
{
	if (health <= 0)
	{
		return false;
	}
	else
	{
		return true;
	}

	
}
