#pragma once
#include "Robot.h"
#include "iWeapon.h"

#include <vector>


class WeaponFactory
{
public:
	WeaponFactory();
	~WeaponFactory();


	iWeapon* CreateWeapon(int weaponID);



private:
	std::vector< iWeapon* > weaponsList;

};

WeaponFactory::WeaponFactory()
{
}

WeaponFactory::~WeaponFactory()
{
}

inline iWeapon* WeaponFactory::CreateWeapon(int weaponID)
{
	
	if (weaponID == 1)
	{
		
		return new Robot();
	}

	
	return nullptr;

}
