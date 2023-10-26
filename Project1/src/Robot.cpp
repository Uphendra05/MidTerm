
#include "Robot.h"
#include "PhysicsObject.h"

glm::vec3 GGetRandomDirection()
{
	float x = (((rand() % 1000) - 1) / 1000.0f) * 2.0f - 1.0f;
	float y = (((rand() % 1000) - 1) / 1000.0f) * 2.0f - 1.0f;

	return glm::normalize(glm::vec3(x, y, 0.0f));
}

Robot::Robot()
{

}

Robot::~Robot()
{
	delete robotModel;
}

inline int Robot::AmmoCount()
{

	return laserGunAmmo;

}


void Robot::CreateRobots(int i)
{

	robotModel = new Model((char*)"Models/Player/Robot_Eye_CentreNew.ply");
	robotModel->transform.position = glm::vec3(2.5f + (i * 5.0f), 2.5, 1.0f);
	robotModel->transform.scale = glm::vec3(0.015);
	robotModel->meshes = std::vector<Mesh>(robotModel->meshes.begin(), robotModel->meshes.end());
	robotModels.push_back(robotModel);
	PhysicsObject* robot1Phys = new PhysicsObject(robotModel);
	robot1Phys->physicsType = SPHERE;
	robot1Phys->Initialize(true);
	float mul = (float)(1.0f + rand() % 3);
	robot1Phys->velocity = GGetRandomDirection() * mul;

	physicsMeshes.push_back(robot1Phys);

}
