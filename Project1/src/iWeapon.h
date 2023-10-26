#pragma once

class iWeapon
{
public:
	iWeapon() = default;
	virtual ~iWeapon() {};


	virtual void CreateRobots(int i) = 0;
	virtual int AmmoCount() = 0;





private:


};
