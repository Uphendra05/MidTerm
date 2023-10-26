#pragma once
#include "iWeapon.h"
#include <iostream>
#include "model.h"
#include <vector>

std::vector<Model*> robotModels;
std::vector<PhysicsObject*> physicsMeshes;
class Robot: public iWeapon
{
public:
	Robot();
	virtual ~Robot();

	void CreateRobots(int i) override;
	 int AmmoCount() override;
	 Model* robotModel;
	 
	 int RobotID;
	 int friendID;
	
private:

	
	int laserGunAmmo = 10;
};

Robot::Robot()
{
	
}

Robot::~Robot()
{
	delete robotModel;
}

inline void Robot::CreateRobots(int i)
{
	
	robotModel = new Model((char*)"Models/Player/Robot_Eye_CentreNew.ply");
	robotModel->transform.position = glm::vec3(2.5f + (i * 5.0f), 2.5, 1.0f);
	robotModel->transform.scale = glm::vec3(0.015);
	robotModel->meshes = std::vector<Mesh>(robotModel->meshes.begin(), robotModel->meshes.end());
	robotModels.push_back(robotModel);
	PhysicsObject* robot1Phys = new PhysicsObject(robotModel);
	robot1Phys->physicsType = AABB;
	robot1Phys->Initialize(true);
	physicsMeshes.push_back(robot1Phys);
}

inline int Robot::AmmoCount()
{

	return laserGunAmmo;

}
