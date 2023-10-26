#include "PhysicsObject.h"





PhysicsObject::PhysicsObject(Model* model)
{
	this->model = model;
	acceleration = glm::vec3(0.0f);
	

}

PhysicsObject::PhysicsObject()
{

	acceleration = glm::vec3(0.0f);

}

PhysicsObject::~PhysicsObject()
{
	//delete model;
}

void PhysicsObject::update(float deltatime)
{
	glm::vec3 gravity(0.0f, -9.81f* gravityValue, 0.0f);
	//acceleration = gravity;
	//velocity += acceleration * deltatime;
	model->transform.position += velocity * deltatime;

	if (physicsType == AABB)
	{
		UpdateAABB();
	}
	else
	{
		UpdateSphere();
	}

}
cAABB  PhysicsObject:: UpdateAABB()
{

	//cAABB temp;

	glm::vec3 originalMinV = aabb.minV;
	glm::vec3 originalMaxV = aabb.maxV;
	glm::mat4 transformMatrix = model->transform.getModelMatrix();
	glm::vec4 transformedMinV = transformMatrix * glm::vec4(originalMinV, 1.0f);
	glm::vec4 transformedMaxV = transformMatrix * glm::vec4(originalMaxV, 1.0f);

	updatedAabb =  cAABB(glm::vec3(transformedMinV), glm::vec3(transformedMaxV));
	return updatedAabb;
}

cSphere PhysicsObject::UpdateSphere()
{
	glm::vec3 originalCenter = sphereShape.center;
	float orginalRadius = sphereShape.radius;
	glm::mat4 transformMatrix = model->transform.getModelMatrix();
	glm::vec4 transformedCenter = transformMatrix * glm::vec4(originalCenter, 1.0f);

	glm::mat4 scaleMatrix = glm::mat4(1.0f); // Initialize the scale matrix
	scaleMatrix[0][0] = model->transform.scale.x;
	scaleMatrix[1][1] = model->transform.scale.y;
	scaleMatrix[2][2] = model->transform.scale.z;

	float maxScale = glm::max(model->transform.scale.x, glm::max(model->transform.scale.y, model->transform.scale.z));
	float updatedRadius = orginalRadius * maxScale * 0.15f;

	updatedSphereShape =  cSphere(glm::vec3(transformedCenter), updatedRadius);

	return updatedSphereShape;
}

bool PhysicsObject::checkCollision(const PhysicsObject& other)
{
	switch(physicsType)
	{
	case PhysicsType::AABB:
		if (other.physicsType == AABB)
		{
			return CheckCOllisionAABBvsAABB(updatedAabb, other.updatedAabb);
		}
		else
		{
			return CheckSphereAABBCollision(other.updatedSphereShape, updatedAabb);
		}
		break;

	case PhysicsType::SPHERE:

		if (other.physicsType == AABB)
		{
			return CheckSphereAABBCollision(updatedSphereShape, other.updatedAabb);
		}
		else
		{
			return CheckSpherevsSphereCollision(updatedSphereShape, other.updatedSphereShape);
		}

		break;
	}


	// default supported
	return false;
}

void PhysicsObject::CollisionRes(const PhysicsObject& other)
{
	switch (physicsType)
	{
	case PhysicsType::AABB:
		if (other.physicsType == AABB)
		{
			
		}
		else
		{
			
		}
		break;

	case PhysicsType::SPHERE:

		if (other.physicsType == AABB)
		{
			
		}
		else
		{
			
		}

		break;
	}
}

void PhysicsObject::Initialize(bool isKinematic)
{
	this->isKinematic = isKinematic;
	if (this->isKinematic)
	{
		gravityValue = 0;
	}
	else
	{
		gravityValue = 0.1f;
	}
	if (physicsType == AABB)
	{
		aabb.calculateAABB(model->meshes[0].vertices);

	}
	if (physicsType == SPHERE)
	{
		sphereShape.calculateSphere(model->meshes[0].vertices);
	}
	
}


