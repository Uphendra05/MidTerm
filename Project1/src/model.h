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