#pragma once
#include "iWeapon.h"
#include <iostream>
#include "model.h"
#include <vector>
#include <glm/glm.hpp>


class PhysicsObject;

extern std::vector<Model*> robotModels;
extern std::vector<PhysicsObject*> physicsMeshes;
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


extern glm::vec3 GGetRandomDirection();