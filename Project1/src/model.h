#pragma once
#ifndef MODEL_H
#define MODEL_H



#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "Transform.h"

#include "mesh.h"
#include "Shader.h"

namespace d
{
	struct AABB
	{
		glm::vec3 origin;
		glm::vec3 half_extent;
		glm::vec3 min;
		glm::vec3 max;

		inline AABB()
		{
			origin = glm::vec3(0);
			half_extent = glm::vec3(0);
			min = glm::vec3(0);
			max = glm::vec3(0);

		}

		inline AABB(glm::vec3 min, glm::vec3 max)
		{
			set_min_max(min, max);
		}

		inline void set_min_max(glm::vec3 min, glm::vec3 max)
		{
			this->min = min;
			this->max = max;
			origin = (max + min) / 2.0f;
			half_extent = glm::abs(max - origin);
		}

		inline void set_half_extents_origin(glm::vec3 origin, glm::vec3 half_extent)
		{
			this->origin = origin;
			this->half_extent = half_extent;

			min = origin - half_extent;
			max = origin + half_extent;
		}

		inline bool check_aabb_intersection(const AABB& b2)const
		{
			return	(
				fabs(origin.x - b2.origin.x) < half_extent.x + b2.half_extent.x &&
				fabs(origin.y - b2.origin.y) < half_extent.y + b2.half_extent.y &&
				fabs(origin.z - b2.origin.z) < half_extent.z + b2.half_extent.z
				);
		}
	};
}

class Model
{
public:


    std::vector<Texture> textures_loaded;	
    std::vector<Mesh>    meshes;
    std::string directory;
    std::string modelPath;
    Transform transform;

    int offset;
    std::string modelName; //if needed 
    float size;

    bool gammaCorrection;


    Model();
    Model( std::string const& path );
    
    void Draw(Shader& shader);

private:

    void loadModel(std::string const& path);    
    void processNode(aiNode* node, const aiScene* scene);   
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);   
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
   
    bool isTextureFlipped;
};




#endif