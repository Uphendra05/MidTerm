#pragma once
#include "Model.h"


enum PhysicsType
{
	SPHERE,
	AABB
};


struct cAABB
{

	cAABB()
	{
	}
	cAABB(glm::vec3 minV, glm::vec3 maxV)
	{
		this->minV = minV;
		this->maxV = maxV;
	}
	~cAABB() {}
	glm::vec3 minV;
	glm::vec3 maxV;
	void calculateAABB(std::vector<Vertex>& vertices)
	{

		if (vertices.size()==0)
		{
			minV = glm::vec3(0);
			maxV = glm::vec3(0);
			return;
		}

		/*minV = vertices[0].vPos;
		maxV = vertices[0].vPos;*/

		for (const Vertex& vertex : vertices)
		{
			minV.x = glm::min(minV.x,vertex.vPos.x);   // min of x
			minV.y = glm::min(minV.y,vertex.vPos.y);    // min of y
			minV.z = glm::min(minV.z,vertex.vPos.z);   // min of z
			maxV.x = glm::max(maxV.x, vertex.vPos.x);   // max of x
			maxV.y = glm::max(maxV.y, vertex.vPos.y);    // max of y
			maxV.z = glm::max(maxV.z, vertex.vPos.z);   // max of z

		}

		std::cout << "MinV : " << minV.x << " , " << minV.y << " , " << minV.z << std::endl;
		std::cout << "MaxV : " << maxV.x << " , " << maxV.y << " , " << maxV.z << std::endl;
	}
};


struct cSphere
{
	cSphere(glm::vec3 position, float radius)
	{
		this->center = position;
		this->radius = radius;
	}
	cSphere()
	{

	}

	~cSphere() {}
	glm::vec3 center;
	float radius;
	void calculateSphere(std::vector<Vertex>& vertices)
	{
		glm::vec3 minV;
		glm::vec3 maxV;
		if (vertices.empty())
		{
			center = glm::vec3(0);
			radius = 0;
			return;
		}

		minV = vertices[0].vPos;
		maxV = vertices[0].vPos;
		for (const Vertex& vertex : vertices)
		{
			minV.x =glm::min(minV.x, vertex.vPos.x);   // min of x
			minV.y =glm::min(minV.y, vertex.vPos.y);    // min of y
			minV.z =glm::min(minV.z, vertex.vPos.z);   // min of z
				
			maxV.x =glm::max(maxV.x, vertex.vPos.x);   // max of x
			maxV.y =glm::max(maxV.y, vertex.vPos.y);    // max of y
			maxV.z =glm::max(maxV.z, vertex.vPos.z);   // max of z

		}

		float maxDistance = 0;

		for (const Vertex& vertex : vertices)
		{
			float distance = glm::length(vertex.vPos - center);
			if (distance > maxDistance) {
				maxDistance = distance;
			}
		}

		center = (minV + maxV) * 0.5f;
		radius = maxDistance;

		std::cout << "Sphere Centre : " << center.x << " , " << center.y << " , " << center.z << std::endl;
		std::cout << "Radius : " << radius<< std::endl;
	}

};



class PhysicsObject
{
public:
	PhysicsObject();
	PhysicsObject(Model* model);
	~PhysicsObject();

	Model* model;
	PhysicsType physicsType;
	bool isKinematic;
	void update(float deltatime);
	cAABB UpdateAABB();
	cSphere UpdateSphere();
	bool checkCollision(const PhysicsObject& other);
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float gravityValue;
	void Initialize(bool isKinematic);
	cAABB aabb;
	cSphere sphereShape;


};

static bool CheckCOllisionAABBvsAABB(const cAABB& a, const cAABB& b)
{
	if (a.maxV[0] < b.minV[0] || a.minV[0] > b.maxV[0]) return false;
	if (a.maxV[1] < b.minV[1] || a.minV[1] > b.maxV[1]) return false;
	if (a.maxV[2] < b.minV[2] || a.minV[2] > b.maxV[2]) return false;

	return true;
}
static bool CheckSpherevsSphereCollision(const cSphere& sphere1, const cSphere& sphere2)
{
	float distance = glm::distance(sphere1.center, sphere2.center);
	float sumRadii = sphere1.radius + sphere2.radius;
	return distance <= sumRadii;
}

static bool CheckSphereAABBCollision(const cSphere& sphere, const cAABB& aabb) {
	glm::vec3 closestPoint;

	// Find the closest point on the AABB to the sphere
	closestPoint.x = glm::max(aabb.minV.x, glm::min(sphere.center.x, aabb.maxV.x));
	closestPoint.y = glm::max(aabb.minV.y, glm::min(sphere.center.y, aabb.maxV.y));
	closestPoint.z = glm::max(aabb.minV.z, glm::min(sphere.center.z, aabb.maxV.z));

	// Calculate squared distance between the sphere center and the closest point on the AABB
	float lengthSquared = glm::dot(closestPoint - sphere.center, closestPoint - sphere.center);

	// Check for collision
	return lengthSquared < (sphere.radius * sphere.radius);
}

