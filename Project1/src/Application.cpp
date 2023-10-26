


#include "Shader.h"
#include <glad/glad.h>


#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "Application.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Camera.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"
#include "VertexArray.h"
#include  "model.h"
#include "PhysicsObject.h"
#include "material.h"
#include "Light.h"
#include "PlayyerInput.h"
#include "WeaponFactory.h"
#include "Robot.h"
#include "Transform.h"
#include <map>
#include <random>
#include <algorithm>
#include <queue>
#include <cmath>
#include <limits>
#include "RobotDirection.h";

/*TIPS and notes

* Shaders does not need to be stored in stack or heap as they are mostly read only and used in compile time. what you can do is
  link the shaders and once its job is done in the graphics card you should delete the shaders as its not necessary anymore

* Always abstract classes properly to maintain proper code.

* Use uniforms in the shader whenever you want to change the values for the shader like moving the triangle etc
*
* Always check if you binded every vertex array correctly that you created, remember the day you tried to make a diffuse light.
*
*
*



*/

Model* tempBullet;
void Instantiate(Model* model);

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
bool checkingPair = false;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        Instantiate(tempBullet);

    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        checkingPair = true;
    }

}

std::vector<int> getRandomPair( std::vector<int>& values) 


{
    std::random_device rd;
    std::mt19937 rng(rd());

    std::uniform_int_distribution<int> uni(0, values.size() - 1);
    int firstIndex = uni(rng);
    int firstValue = values[firstIndex];
    values.erase(values.begin() + firstIndex);

    std::uniform_int_distribution<int> uni2(0, values.size() - 1);
    int secondIndex = uni2(rng);
    int secondValue = values[secondIndex];
    values.erase(values.begin() + secondIndex);

    return { firstValue, secondValue };
}

const unsigned int SCR_WIDTH = 2000;
const unsigned int SCR_HEIGHT = 2000;


Camera camera(glm::vec3(23.0f, 15.0f, 70.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


float deltaTime = 0.0f;
float lastFrame = 0.0f;

 std::vector<Model*> loadedModels;
extern std::vector<Model*> robotModels ;
extern std::vector<PhysicsObject*> physicsMeshes;

std::vector<std::pair<Model*, Model*>> getRandomPairs(const std::vector<Model*>& models) {
    std::random_device rd;
    std::mt19937 rng(rd());

    std::vector<std::pair<Model*, Model*>> pairs;

    if (models.size() % 2 != 0) {
        std::cout << "The number of models must be even for pairing." << std::endl;
        return pairs;
    }

    std::vector<Model*> availableModels = models;

    while (!availableModels.empty()) {
        std::uniform_int_distribution<size_t> uni(0, availableModels.size() - 1);
        size_t firstIndex = uni(rng);
        Model* firstModel = availableModels[firstIndex];
        availableModels.erase(availableModels.begin() + firstIndex);

        uni = std::uniform_int_distribution<size_t>(0, availableModels.size() - 1);
        size_t secondIndex = uni(rng);
        Model* secondModel = availableModels[secondIndex];
        availableModels.erase(availableModels.begin() + secondIndex);

        pairs.push_back({ firstModel, secondModel });
    }

    return pairs;
}




const char OBSTACLE = 'X';
const char OPEN_SPACE = ' ';
const char START = 'S';
const char END = 'E';

std::vector<std::string> readTerrainFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<std::string> terrain;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            terrain.push_back(line);
        }
        file.close();
    }

    return terrain;
}

std::vector<std::vector<char>> parseTerrain(const std::vector<std::string>& terrain) {
    std::vector<std::vector<char>> grid;

    for (const std::string& row : terrain) {
        std::vector<char> gridRow;
        for (char cell : row) {
            gridRow.push_back(cell);
        }
        grid.push_back(gridRow);
    }

    return grid;
}




int main()
{

    std::vector<std::string> terrain = readTerrainFromFile("Models/Terrain/Berzerk_Level_1211808.txt");
    std::vector<std::vector<char>> grid = parseTerrain(terrain);

    
    

    
   

   




    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);




    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "KAIZOKU", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }



    glEnable(GL_DEPTH_TEST);



    

    Shader ModelShader("Shaders/Model.vert", "Shaders/Model.frag");


    Model* Terrain = new Model((char*)"Models/Terrain/Berzerk_Level_Ouput.ply");
    loadedModels.push_back(Terrain);
    Terrain->transform.position = glm::vec3(0.0f,0.0f,0.0f);

    Model* BOX = new Model((char*)"Models/BOX/box.obj");
    BOX->transform.position = glm::vec3(5.0f, 5.0f, 1);
    BOX->transform.scale = glm::vec3(0.5f, 5.0f, 0.0f);
    loadedModels.push_back(BOX);

    PhysicsObject* boxPhys= new PhysicsObject(BOX);
    boxPhys->physicsType = AABB;
    boxPhys->Initialize(true);
    physicsMeshes.push_back(boxPhys);

    WeaponFactory* factory = nullptr;
    Robot robot;
    std::vector<int> tempRobots = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    for (size_t i = 0; i < 10; i++)
    {

        factory->CreateWeapon(1)->CreateRobots(i);

    }
    std::vector<std::pair<Model*, Model*>> randomPairs = getRandomPairs(robotModels);

  
  

   
    //Robot2->transform.scale = glm::vec3(0.01f);
    Material material( 128.0f);

   

    float vertices[] = {
        // positions          // normals           // texture coords
   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    // first, configure the cube's VAO (and VBO)



    Renderer render;
  
    
    unsigned int VBO, lightCubeVAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

  /*  unsigned int VBO2, lightCubeVAO2;
    glGenBuffers(1, &VBO2);
    glGenVertexArrays(1, &lightCubeVAO2);
    glBindVertexArray(lightCubeVAO2);

    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);*/



    float lightSize = 0.5f;
    float xPos = 0.0f;
    float yPos = 0.0f;
    float zPos = 0.0f;

    float lightX = 0.0f;
    float lightY = 0.3f;
    float lightZ = 10.0f;

    float rotX = 1.0f;
    float rotY = 0.0f;
    float rotZ = 0.0f;

    float color[3] = { 1.0f, 1.0f, 1.0f };
    bool isWireFrame = false;

 
    float yAxis = 10;

    glm::vec3 collisionPosition = glm::vec3(0.0f);
    

    double lastTime = glfwGetTime();

  /*PhysicsObject* backpackPhysics = new PhysicsObject(backpack);
    backpackPhysics->physicsType = SPHERE;
    backpackPhysics->Initialize(true);
    physicsMeshes.push_back(backpackPhysics);

    PhysicsObject* speherePhysics = new PhysicsObject(Ball);
    speherePhysics->physicsType = SPHERE;
    speherePhysics->Initialize(true);
    physicsMeshes.push_back(speherePhysics);*/
   

    glm::vec3 playerPos = glm::vec3(0.0f, 0.0f, 0.0f);

  /*  PlayerInput player;
    player.playerModel = backpack;
    player.playerPhysics = backpackPhysics;*/


    bool isCollided = false;
   

    while (!glfwWindowShouldClose(window))
    {
        RandomDirection randomDirection;

      
        

       

        //player.MovePlayer(window, playerPos);


        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

       // system("pause");
        if (checkingPair)
        {
            for (size_t i = 0; i < randomPairs.size(); ++i)
            {
                if (glm::distance(randomPairs[i].first->transform.position, randomPairs[i].second->transform.position) >= 2.5f)
                {
                    glm::vec3 direction = glm::normalize(randomPairs[i].second->transform.position - randomPairs[i].first->transform.position);

                    float speed = 2.5f;

                    float t = speed * deltaTime;

                    glm::vec3 newPosition = randomPairs[i].first->transform.position + direction * t;

                    if (!isCollided)
                    {
                        randomPairs[i].first->transform.position = newPosition;
                    }
                    else
                    {
                        randomDirection = GetRandomDirection(); 
                        switch (randomDirection) {
                        case RandomDirection::Up:
                            newPosition = randomPairs[i].first->transform.position + direction.y * t;

                            std::cout << "Moving Up" << std::endl;
                            break;
                        case RandomDirection::Down:
                            newPosition = randomPairs[i].first->transform.position - direction.y * t;
                            std::cout << "Moving Down" << std::endl;
                            break;
                        
                        }

                        randomPairs[i].first->transform.position = newPosition;

                        
                       

                        
                        
                    }
                }


            }

        }

        
      
        processInput(window);

    
        render.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        {

            static float f = 0.0f;
            static int counter = 0;


            //add a Title to your GUI layout
            ImGui::Begin("Media Player Lite!");
            // ImGui::SetWindowFontScale(2.0f);
            ImGui::SetWindowSize(ImVec2(800, 800));

            //add a intro text
            ImGui::Text("KAIZOKU ENGINE");

            ImGui::NewLine();
            ImGui::PushItemWidth(100);
            ImGui::SliderFloat("##X Position", &xPos, -10.0f, 10.0f, "X: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10);
            ImGui::SliderFloat("##Y Position", &yPos, -10.0f, 10.0f, "Y: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10);
            ImGui::SliderFloat("##Z Position", &zPos, -10.0f, 10.0f, "Z: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10); ImGui::Text("POSITION");

            ImGui::NewLine();
            ImGui::PushItemWidth(100);
            ImGui::SliderFloat("##X Pos", &lightX, -10.0f, 10.0f, "X: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10);
            ImGui::SliderFloat("##Y Pos", &lightY, -10.0f, 10.0f, "Y: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10);
            ImGui::SliderFloat("##Z Pos", &lightZ, -100.0f, 10.0f,"Z: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10); ImGui::Text("LIGHT POSITION");

            ImGui::NewLine();
            ImGui::PushItemWidth(100);
            ImGui::SliderFloat("##X Rot", &rotX, 0.0f, 1.0f, "X: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10);
            ImGui::SliderFloat("##Y Rot", &rotY, 0.0f, 1.0f, "Y: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10);
            ImGui::SliderFloat("##Z Rot", &rotZ, 0.0f, 1.0f, "Z: %.1f", ImGuiSliderFlags_AlwaysClamp);
            ImGui::SameLine(0, 10); ImGui::Text("OBJECT POSITION");



            ImGui::NewLine();
            ImGui::PushItemWidth(150);
            ImGui::SliderFloat("SCALE", &lightSize, 0.0f, 10.0f);

            ImGui::NewLine();
            ImGui::PushItemWidth(400);
            ImGui::ColorEdit3("LIGHT COLOR", color);

            ImGui::NewLine();
            ImGui::Checkbox("WIREFRAME", &isWireFrame);
            // ImGui::Checkbox("Loop Audio", );

            // ImGui::ColorPicker3("LightColor", 1.0f, 1.0f, 1.0f);





             //framerate
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();

        }


         ModelShader.use();

         glm::mat4 projection2 = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
         glm::mat4 view2 = camera.GetViewMatrix();
         ModelShader.setMat4("projection", projection2);
         ModelShader.setMat4("view", view2);

     

         for (size_t i = 0; i < loadedModels.size(); i++)
         {
            
             loadedModels[i]->Draw(ModelShader);
         }

         for (size_t i = 0; i < robotModels.size(); i++)
         {

             robotModels[i]->Draw(ModelShader);
         }

         for (size_t i = 0; i < physicsMeshes.size(); i++)
         {
             physicsMeshes[i]->update(deltaTime);

         }

         for (size_t i = 0; i < physicsMeshes.size(); i++)
         {


             for (size_t j = 0; j < physicsMeshes.size(); j++)
             {
                 if (i == j)
                 {
                     continue;
                 }
                 else
                 {
                     if (physicsMeshes[i]->physicsType == AABB && physicsMeshes[j]->physicsType == AABB)
                     {


                         if (CheckCOllisionAABBvsAABB(physicsMeshes[i]->UpdateAABB(),
                             physicsMeshes[j]->UpdateAABB()))
                         {
                             
                             isCollided = true;
                             std::cout << "Collision........" << std::endl;

                             


                             //if playerbullet

                             //physicsMeshes[j].model->isVisible = false;

                         }


                     }
                 }



             }
         }






        

         if (isWireFrame)
         {
             glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
         }
         else
         {
             glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


         }

        
         
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

   
     glDeleteVertexArrays(1, &lightCubeVAO);


    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;


}






void processInput(GLFWwindow* window)
{


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    bool ispressed = false;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        ispressed = true;

    }
    else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
    {
        ispressed = false;


    }

    if (ispressed)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.ProcessKeyboard(RIGHT, deltaTime);
    }




}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

int posCount = 0;
void Instantiate(Model* model)
{

    posCount++;
    Model* temp = new Model();
    temp->modelName = model->modelName;
    temp->transform.position = glm::vec3(2.5f + (posCount * 5.0f), 2.5, 1.0f);
    temp->transform.scale = glm::vec3(0.025);
    temp->meshes = std::vector<Mesh>(model->meshes.begin(), model->meshes.end());
    loadedModels.push_back(temp);
    


}


