//
//  main.cpp
//  3D Object Drawing
//


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib> // for rand()
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "sphere.h"
#include "spotLight.h"
#include "directionLight.h"
#include "cube.h"
#include "stb_image.h"
#include "curve.h"
#include "sphere.h"
#include "octagon.h"
#include "pyramid.h"

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void drawBridge(Shader& shaderProgram);

void drawClockHand(unsigned int VAO, Shader& shader, glm::mat4 globalTransform, glm::vec3 position, float rotationAngle, glm::vec3 scale);

void drawBirds(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 parentModel, float time);
void drawBase(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);


void drawMain(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);


void drawTreeWithFractiles(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, float L, float H, float W, int N);

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);




// settings
const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 1000;



// camera
Camera camera(glm::vec3(0.0f, 35.0f, 135.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 0.0, eyeY = 1.0, eyeZ = 3.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
//BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);


// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(56.50f,  50.50f,  -58.0f),
    glm::vec3(66.50f,  50.5f,  -58.0f),
    glm::vec3(56.5f,  30.5f,  -58.0f),
    glm::vec3(66.5f,  30.5f,  -58.0f)
};

glm::vec3 spotLightPosition = glm::vec3(56.f, 110.5f, -68.0f);

SpotLight spotlight(

    spotLightPosition.x, spotLightPosition.y, spotLightPosition.z,  // position
    0.0f, -1.0f, 0.0f,   //direction
    0.0f, 0.5f, 0.5f,    // ambient
    0.0f, 1.0f, 1.0f,    // diffuse
    0.0f, 1.0f, 1.0f,    // specular
    0.0032f,   //k_c
    0.00032f,  //k_l
    0.00032f, //k_q
    45,     //light angle
    5       // light number
);

DirectionLight moonlight(

    0.0f, -1.0f, -0.5f,   //direction
    0.031f, 0.04f, 0.045f,    // ambient
    0.031f, 0.04f, 0.45f,    // diffuse
    1.0f, 1.0f, 1.0f,    // specular
    7       // light number
);

DirectionLight daylight(

    0.0f, -1.0f, -0.5f,   //direction
    0.4f, 0.4f, 0.4f,    // ambient
    0.8f, 0.8f, 0.8f,    // diffuse
    1.0f, 1.0f, 1.0f,    // specular
    6       // light number
);

PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.1f, 0.05f, 0.1f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    0.032f,   //k_c
    0.032f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.15f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    0.032f,   //k_c
    0.032f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.1f, 0.1f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    0.032f,   //k_c
    0.032f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.1f, 0.1f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    0.032f,   //k_c
    0.032f,  //k_l
    0.032f, //k_q
    4       // light number
);


// light settings
bool pointLightOn = true;
bool spotLightOn = true;
bool dayLightOn = true;
bool moonLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;


int main()
{

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Initialize all lights to ON in main()
    pointlight1.turnOn();
    pointlight2.turnOn();
    pointlight3.turnOn();
    pointlight4.turnOn();
    spotlight.turnOn();
    daylight.turnOn();
    moonlight.turnOn();


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    Shader waterShader("waterVertexShader.vs", "waterFragmentShader.fs");


    // set up vertex data (and buffer(s)) and configure vertex attributes
    


    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    // Define vertices for the water plane (quad)
    float waterVertices[] = {
        // Positions          // Texture Coords
        -1.0f, 0.0f, -1.0f,   0.0f, 0.0f,
         1.0f, 0.0f, -1.0f,   1.0f, 0.0f,
         1.0f, 0.0f,  1.0f,   1.0f, 1.0f,
        -1.0f, 0.0f,  1.0f,   0.0f, 1.0f
    };
    unsigned int waterIndices[] = {
        0, 1, 2,
        0, 2, 3
    };

    // Vertices for clock hands (thin rectangles)
    float clockHandVertices[] = {
        -0.05f, 0.0f,  0.0f,  0.0f, 0.0f,  // Bottom-left
         0.05f, 0.0f,  0.0f,  1.0f, 0.0f,  // Bottom-right
         0.05f, 1.0f,  0.0f,  1.0f, 1.0f,  // Top-right
        -0.05f, 1.0f,  0.0f,  0.0f, 1.0f   // Top-left
    };

    unsigned int clockHandIndices[] = { 0, 1, 2, 0, 2, 3 };

    unsigned int clockHandVAO, clockHandVBO, clockHandEBO;
    glGenVertexArrays(1, &clockHandVAO);
    glGenBuffers(1, &clockHandVBO);
    glGenBuffers(1, &clockHandEBO);

    glBindVertexArray(clockHandVAO);

    // Bind and set vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, clockHandVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(clockHandVertices), clockHandVertices, GL_STATIC_DRAW);

    // Bind and set element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, clockHandEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(clockHandIndices), clockHandIndices, GL_STATIC_DRAW);

    // Set vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    // Create VAO, VBO, EBO for water
    unsigned int waterVAO, waterVBO, waterEBO;
    glGenVertexArrays(1, &waterVAO);
    glGenBuffers(1, &waterVBO);
    glGenBuffers(1, &waterEBO);

    glBindVertexArray(waterVAO);

    glBindBuffer(GL_ARRAY_BUFFER, waterVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(waterVertices), waterVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, waterEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(waterIndices), waterIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    /*
    //bazierVAO
    bezierVAO = hollowBezier(cntrlPoints.data(), ((unsigned int)cntrlPoints.size() / 3) - 1);
    coordinates.clear();
    normals.clear();
    indices.clear();
    vertices.clear();

    bezierCylinderVAO = hollowBezier(cntrlPointsCylinder.data(), ((unsigned int)cntrlPointsCylinder.size() / 3) - 1);
    coordinates.clear();
    normals.clear();
    indices.clear();
    vertices.clear();

    AbirVAO = hollowBezier(Abir.data(), ((unsigned int)Abir.size() / 3) - 1);
    */


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    string diffuseMapPath = "rsz_11field_image.jpg";
    string specularMapPath = "rsz_11field_image.jpg";

    string diffuseMapPath1 = "sky7.jpg";
    string specularMapPath1 = "sky7.jpg";
    string waterTexturePath = "water.bmp";
    unsigned int waterTextureID = loadTexture(waterTexturePath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    // Texture paths
    std::string buildingWallTexturePath = "building_wall_texture.bmp";
    std::string buildingRoofTexturePath = "building_roof_texture.bmp";

    // Load the textures using the provided texture loading function
    unsigned int buildingWallTextureID = loadTexture(buildingWallTexturePath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int buildingRoofTextureID = loadTexture(buildingRoofTexturePath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);



    string bridgeTexturePath = "wood.bmp";
    unsigned int bridgeTextureID = loadTexture(bridgeTexturePath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    


    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    unsigned int diffMap1 = loadTexture(diffuseMapPath1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap1 = loadTexture(specularMapPath1.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Sphere sphere = Sphere(diffMap1, specMap1, 0, 0, 2, 1);

    Cube texcube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    //Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    diffuseMapPath = "rsz_1texture-grass-field.jpg";
    specularMapPath = "rsz_1texture-grass-field.jpg";


    

    // Load building wall and roof textures
    unsigned int buildingWallTexture = loadTexture("path/to/building_wall.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int buildingRoofTexture = loadTexture("path/to/building_roof.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);


    //diffuse aar spec string 
    diffuseMapPath = "sky.jpg";
    specularMapPath = "sky.jpg";


    diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube texcube2 = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    // Material properties
    glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);   // Ambient reflection coefficients
    glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);   // Diffuse reflection coefficients
    glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);  // Specular reflection coefficients
    float shininess = 32.0f;                           // Shininess for specular highlights


    diffuseMapPath = "BIG-BEN-LOWER-PART.bmp";
    specularMapPath = "BIG-BEN-LOWER-PART.bmp";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE);
    specMap = loadTexture(specularMapPath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE);
    Cube bigBenBaseCube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    bigBenBaseCube.setMaterialisticProperty(ambient, diffuse, specular, shininess);
    //bigBenBaseCube.setMaterialisticProperty(glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), 32.0f);



    diffuseMapPath = "BigBenFace.bmp";
    specularMapPath = "BigBenFace.bmp";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube bigBenClockCube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    bigBenClockCube.setMaterialisticProperty(ambient, diffuse, specular, shininess);



    diffuseMapPath = "1st_Cube.bmp";
    specularMapPath = "1st_Cube.bmp";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube firstCube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    firstCube.setMaterialisticProperty(ambient, diffuse, specular, shininess);


    diffuseMapPath = "2ndCube.bmp";
    specularMapPath = "2ndCube.bmp";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube secondCube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    secondCube.setMaterialisticProperty(ambient, diffuse, specular, shininess);

    diffuseMapPath = "3rd_CUBE.bmp";
    specularMapPath = "3rd_CUBE.bmp";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube thirdCube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    thirdCube.setMaterialisticProperty(ambient, diffuse, specular, shininess);

    diffuseMapPath = "1st_Cube.bmp";
    specularMapPath = "1st_Cube.bmp";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube fourthCube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    fourthCube.setMaterialisticProperty(ambient, diffuse, specular, shininess);



    diffuseMapPath = "1st_Cube.bmp";
    specularMapPath = "1st_Cube.bmp";
    diffMap = loadTexture(diffuseMapPath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    specMap = loadTexture(specularMapPath.c_str(), GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Pyramid pyramid = Pyramid(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);
    pyramid.setMaterialisticProperty(ambient, diffuse, specular, shininess);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);


        // Set point lights if toggled on
        if (pointLightOn) {
            pointlight1.setUpPointLight(lightingShader);
            pointlight2.setUpPointLight(lightingShader);
            pointlight3.setUpPointLight(lightingShader);
            pointlight4.setUpPointLight(lightingShader);
        }

        // Set up the spotlight
        if (spotLightOn) {
            spotlight.setUpSpotLight(lightingShader);
        }

        // Set up directional lights
        if (dayLightOn) {
            daylight.setUpDirectionalLight(lightingShader);
        }
        if (moonLightOn) {
            moonlight.setUpDirectionalLight(lightingShader);
        }


        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 100.0f, 900.0f);
        ////glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);


        //// camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        ////glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);
        // Set projection and view matrices for the shader
        
        // Set global material properties
        lightingShader.setVec3("material.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        lightingShader.setVec3("material.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setFloat("material.shininess", 32.0f);


        
        // Define the scaling factor (you can adjust this based on your scene size)
        float scaleFactor = 150.0f;  // Adjust this value to fit the water to your scene

        // Create a model matrix and apply scaling
        glm::mat4 waterModel = glm::mat4(1.0f);  // Identity matrix
        waterModel = glm::scale(waterModel, glm::vec3(scaleFactor, 1.0f, scaleFactor));  // Scale only in X and Z

        // Use water shader
        waterShader.use();

        // Set uniform matrices (view, projection, model)
        waterShader.setMat4("view", view);
        waterShader.setMat4("projection", projection);
        waterModel = glm::scale(glm::mat4(1.0f), glm::vec3(150.0f, 1.0f, 150.0f)); // Set the scaled model matrix
        waterShader.setMat4("model", waterModel);
        // Set the time uniform for wave animation
        waterShader.setFloat("time", glfwGetTime());

        // Bind the water texture and set uniform for the texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, waterTextureID);  // Bind the water texture
        waterShader.setInt("waterTexture", 0);

        // Draw the water plane
        glBindVertexArray(waterVAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // In the main rendering loop

        glBindTexture(GL_TEXTURE_2D, bridgeTextureID);
        
        lightingShaderWithTexture.use();
        drawBridge(lightingShaderWithTexture);


        float time = glfwGetTime(); // Get the elapsed time since the start of the program
        //drawTreeWithFractiles(cubeVAO, lightingShader, glm::mat4(1.0f), 5.0f, 5.0f, 5.0f, 0); // Draw the tree
        // Draw birds with lightingShader
        time = glfwGetTime();
        drawBirds(cubeVAO, lightingShader, glm::mat4(1.0f), time);



        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translate, rotate, revtranslate, alTogether, next, model, scale;
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(10, 20, 10));
        //rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        //rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1, 0.1, 0.1));
        model = identityMatrix;
        lightingShader.setMat4("model", model);


        // Define the global transformation matrix for the entire structure
        glm::mat4 globalTransform1 = glm::mat4(1.0f);
        // Translate the entire structure 20 units left in X-axis (reduce X translation by 20)
        globalTransform1 = glm::translate(globalTransform1, glm::vec3(-340.0f, 4.0f, -100.0f));
        globalTransform1 = glm::scale(globalTransform1, glm::vec3(1.5f, 2.5f, 1.5f)); // Scale the entire structure

        // Set material properties for each object
        lightingShader.setVec3("material.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
        lightingShader.setVec3("material.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
        lightingShader.setVec3("material.specular", glm::vec3(1.0f, 1.0f, 1.0f));
        lightingShader.setFloat("material.shininess", 32.0f);


        for (int i = 0; i < 100; i++) {       // Buildings in a row
            for (int j = 0; j < 2; j++) {    // Depth of buildings
                glm::mat4 model = glm::mat4(1.0f);

                // Fix the position of the buildings in a grid layout (no movement in Y)
                model = glm::translate(model, glm::vec3(i * 5.0f, 0.0f, j * 10.0f));

                // Generate pseudo-random height using a deterministic formula
                float k = 5.0f + (fmod(sin(i * 3.14f + j * 2.71f) * 100.0f, 10.0f) + 5.0f);

                // Apply scaling for height using k
                model = glm::scale(model, glm::vec3(3.0f, k, 3.0f));

                // Combine the global transformation matrix with the local model matrix
                glm::mat4 finalModel = globalTransform1 * model;

                // Create building object with loaded textures
                Cube building(buildingWallTextureID, buildingRoofTextureID, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

                // Draw the building with texture
                building.drawCubeWithTexture(lightingShaderWithTexture, finalModel);
            }
        }



        // Calculate current time for clock hand rotation
        float currentTime = glfwGetTime();
        float hourAngle = fmod((currentTime / 3600.0f) * 360.0f, 360.0f);   // Hour hand rotates once every 12 hours
        float minuteAngle = fmod((currentTime / 60.0f) * 360.0f, 360.0f);  // Minute hand rotates once every hour

        // Position of the clock center (should match the clock face)
        glm::vec3 clockPosition = glm::vec3(-0.3f, 30.0f, -3.5f);

        // Draw the hour and minute hands
        drawClockHand(clockHandVAO, lightingShaderWithTexture, glm::mat4(1.0f), clockPosition, hourAngle, glm::vec3(0.05f, 3.0f, 1.0f)); // Hour hand
        drawClockHand(clockHandVAO, lightingShaderWithTexture, glm::mat4(1.0f), clockPosition, minuteAngle, glm::vec3(0.03f, 4.0f, 1.0f)); // Minute hand



        // Switch to textured shader (lightingShaderWithTexture)
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        // Set light properties for lightingShaderWithTexture
        if (pointLightOn) {
            pointlight1.setUpPointLight(lightingShaderWithTexture);
            pointlight2.setUpPointLight(lightingShaderWithTexture);
            pointlight3.setUpPointLight(lightingShaderWithTexture);
            pointlight4.setUpPointLight(lightingShaderWithTexture);
        }
        if (spotLightOn) {
            spotlight.setUpSpotLight(lightingShaderWithTexture);
        }
        if (dayLightOn) {
            daylight.setUpDirectionalLight(lightingShaderWithTexture);
        }
        if (moonLightOn) {
            moonlight.setUpDirectionalLight(lightingShaderWithTexture);
        }

        //scale = glm::scale(identityMatrix, glm::vec3(4.0, 4.0, 4.0));
        //dome2.drawBezierCurve(lightingShader, scale);

        // Define the global transformation matrix for the entire structure
        glm::mat4 globalTransform = glm::mat4(1.0f);
        globalTransform = glm::translate(globalTransform, glm::vec3(50.0f, 5.0f, -70.0f)); // Translate the entire structure
        globalTransform = glm::scale(globalTransform, glm::vec3(1.5f, 1.5f, 1.5f)); // Scale the entire structure

        
        //Draw BIG BEN 
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
        model = glm::scale(model, glm::vec3(6.3f, 30.0f, 5.0f)); // Tall base part
        bigBenBaseCube.drawCubeWithTexture(lightingShaderWithTexture, globalTransform * model);

        // Draw Big Ben Clock Part
        model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, 30.0f, -3.5f)); // Translate above the base 
        model = glm::scale(model, glm::vec3(7.0f, 7.0f, 7.0f)); // Clock part 
        bigBenClockCube.drawCubeWithTexture(lightingShaderWithTexture, globalTransform* model);

        // Draw Big Ben another small cube
        //1st cube
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 37.0f, -2.5f));
        model = glm::scale(model, glm::vec3(6.3f, 2.0f, 5.0f)); // Tall base part
        firstCube.drawCubeWithTexture(lightingShaderWithTexture, globalTransform* model);

        //2nd cube
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.50f, 39.0f, -2.5f));
        model = glm::scale(model, glm::vec3(5.2f, 1.0f, 5.0f)); // Tall base part
        secondCube.drawCubeWithTexture(lightingShaderWithTexture, globalTransform* model);

        //3rd cube
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 40.0f, -2.5f));
        model = glm::scale(model, glm::vec3(4.1f, 1.0f, 5.0f)); // Tall base part
        secondCube.drawCubeWithTexture(lightingShaderWithTexture, globalTransform* model);

        //4th cube
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.5f, 41.0f, -2.5f));
        model = glm::scale(model, glm::vec3(3.0f, 1.0f, 5.0f)); // Tall base part
        secondCube.drawCubeWithTexture(lightingShaderWithTexture, globalTransform* model);

        //5th cube
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.5f, 42.0f, -2.5f));
        model = glm::scale(model, glm::vec3(3.0f, 2.0f, 5.0f)); // Tall base part
        thirdCube.drawCubeWithTexture(lightingShaderWithTexture, globalTransform* model);

        //PYRAMID
        model = glm::translate(glm::mat4(1.0f), glm::vec3(3.0f, 44.0f, 0.0f)); // Translate above the clock part 
        model = glm::scale(model, glm::vec3(1.6f, 6.0f, 2.6f)); // Pyramid shape 
        pyramid.drawPyramidWithTexture(lightingShaderWithTexture, globalTransform* model);



        //Drawing tree using fractiles
        translate = glm::translate(identityMatrix, glm::vec3(-15.0, 12.0, -18.0));
        model = translate;
        drawTreeWithFractiles(cubeVAO, lightingShaderWithTexture, model, 0, 0, 10, 0);

        

        


        




        glm::mat4 modelForSphere = glm::mat4(1.0f);
        modelForSphere = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        modelForSphere = glm::scale(modelForSphere, glm::vec3(290.0f, 290.0f, 290.0f));
        sphere.drawSphereWithTexture(lightingShaderWithTexture, modelForSphere);

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 4; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        model = glm::mat4(1.0f);
        model = glm::translate(model, spotLightPosition);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        lightingShaderWithTexture.use();

        if (pointLightOn) {
            pointlight1.setUpPointLight(lightingShader);
            pointlight2.setUpPointLight(lightingShader);
            pointlight3.setUpPointLight(lightingShader);
            pointlight4.setUpPointLight(lightingShader);
        }

        pointlight1.setUpPointLight(lightingShaderWithTexture);
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        pointlight4.setUpPointLight(lightingShaderWithTexture);

        spotlight.setUpSpotLight(lightingShaderWithTexture);

        moonlight.setUpDirectionalLight(lightingShaderWithTexture);
        daylight.setUpDirectionalLight(lightingShaderWithTexture);



       

        

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    glfwTerminate();
    return 0;
}







void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}



void drawClockHand(unsigned int VAO, Shader& shader, glm::mat4 globalTransform, glm::vec3 position, float rotationAngle, glm::vec3 scale) {
    // Activate the shader
    shader.use();

    // Calculate the model matrix for the clock hand
    glm::mat4 model = glm::translate(globalTransform, position);  // Translate to clock center
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis
    model = glm::scale(model, scale);  // Scale the clock hand

    // Pass the model matrix to the shader
    shader.setMat4("model", model);

    // Bind the VAO and draw the clock hand
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}



void drawMain(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 revtranslate = glm::mat4(1.0f);
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 rotate = glm::mat4(1.0f);

    //front
    

    //rotate = glm::rotate(identityMatrix, glm::radians(45), glm::vec3(0.0f, .0f, 0.0f));
    rotate = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    translate = glm::translate(identityMatrix, glm::vec3(7.5, 0.0, 0.0));
    revtranslate = glm::translate(identityMatrix, glm::vec3(-17.0, 0.0, -3.5));
    model = alTogether * revtranslate * rotate * translate;
   // drawSideFrame(cubeVAO, lightingShader, model);


}

void drawBridge(Shader& shaderProgram) {
    glm::mat4 model;

    // Global scale and position (optional if needed)
    glm::mat4 globalTransform = glm::mat4(1.0f);
    globalTransform = glm::translate(globalTransform, glm::vec3(0.0f, -1.0f, -10.0f)); // Position the bridge
    globalTransform = glm::scale(globalTransform, glm::vec3(1.0f, 1.0f, 1.0f));        // Scale the entire structure

    // Flat top of the bridge
    Cube flatTopCube = Cube(); // Assuming Cube() is a predefined object for cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-130.0f, 10.0f, 0.0f)); // Position above the pillars
    model = glm::scale(model, glm::vec3(300.0f, 5.0f, 5.0f));    // Length: 20, Height: 1, Width: 5
    flatTopCube.drawCubeWithTexture(shaderProgram, globalTransform * model);

    
}





void drawBirds(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 parentModel, float time)
{
    glm::mat4 model = glm::mat4(1.0f); // Initialize the model matrix for the bird
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);

    // Bird movement parameters
    float birdSpeed = 1.0f;  // Controls how fast the bird flies
    float birdAltitude = 60.0f; // Controls how high the bird flies

    // Calculate the bird's position based on time for animation
    float birdPathX = sin(time * birdSpeed) * 10.0f; // Bird moves along the x-axis in a sine wave pattern
    float birdPathZ = cos(time * birdSpeed) * 10.0f; // Bird moves along the z-axis

    // First bird - Body (using a cube as body)
    translate = glm::translate(glm::mat4(1.0f), glm::vec3(birdPathX, birdAltitude, birdPathZ));
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.3f, 1.0f)); // Body (elongated cube)
    model = parentModel * translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.7f, 0.7f, 0.7f);  // Draw the body with a gray color

    // First bird - Wings (using two smaller cubes as wings)
    translate = glm::translate(glm::mat4(1.0f), glm::vec3(birdPathX - 0.3f, birdAltitude + 0.2f, birdPathZ)); // Wing position
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.05f, 0.6f)); // Left wing (thin cube)
    model = parentModel * translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.6f, 0.6f, 0.6f);  // Left wing

    translate = glm::translate(glm::mat4(1.0f), glm::vec3(birdPathX + 0.3f, birdAltitude + 0.2f, birdPathZ)); // Right wing position
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.05f, 0.6f)); // Right wing (thin cube)
    model = parentModel * translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.6f, 0.6f, 0.6f);  // Right wing

    // Second bird - Body (same approach as first bird)
    birdPathX = sin((time + 3.0f) * birdSpeed) * 10.0f; // Offset by 3 seconds for different path
    birdPathZ = cos((time + 3.0f) * birdSpeed) * 10.0f; // Offset by 3 seconds for different path
    translate = glm::translate(glm::mat4(1.0f), glm::vec3(birdPathX, birdAltitude + 1.0f, birdPathZ)); // Higher altitude for second bird
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f, 0.3f, 1.0f)); // Body (elongated cube)
    model = parentModel * translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.7f, 0.7f, 0.7f);  // Draw the second bird's body

    // Second bird - Wings (using two smaller cubes as wings)
    translate = glm::translate(glm::mat4(1.0f), glm::vec3(birdPathX - 0.3f, birdAltitude + 1.2f, birdPathZ)); // Left wing position
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.05f, 0.6f)); // Left wing (thin cube)
    model = parentModel * translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.6f, 0.6f, 0.6f);  // Left wing

    translate = glm::translate(glm::mat4(1.0f), glm::vec3(birdPathX + 0.3f, birdAltitude + 1.2f, birdPathZ)); // Right wing position
    scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f, 0.05f, 0.6f)); // Right wing (thin cube)
    model = parentModel * translate * scale;
    drawCube(cubeVAO, lightingShader, model, 0.6f, 0.6f, 0.6f);  // Right wing
}



void drawBase(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    float baseHeight = 4.0;
    float width = 20;
    float length = -40;

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 identityMatrix = glm::mat4(1.0f);



    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 mirror = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(0.0, -0.5, 0.0));
    model = alTogether * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.7, 0.7, 0.7);

    mirror = glm::scale(identityMatrix, glm::vec3(-1.0, 1.0, 1.0));
    model = mirror * model;
    drawCube(cubeVAO, lightingShader, model, 0.7, 0.7, 0.7);
}


void drawTreeWithFractiles(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether, float L, float H, float W, int N)
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 next = glm::mat4(1.0f);
    glm::mat4 rotate = glm::mat4(1.0f);

    // Color definitions
    glm::vec3 trunkColor = glm::vec3(0.54f, 0.27f, 0.07f);  // Brown (lower portion)
    glm::vec3 branchColor = glm::vec3(1.0f, 0.75f, 0.8f);    // Pink (upper portion)

    if (N == 0) {
        // Increase the initial size of the tree trunk
        float length = 1.0f;  // Increase initial length
        float height = 8.0f;  // Increase initial height
        float width = 1.0f;   // Increase initial width

        float mvx = length * 0.05;
        float mvy = height - height * 0.1;
        float mvz = width * 0.05;

        translate = glm::translate(identityMatrix, glm::vec3(mvx, mvy, mvz));
        next = translate * alTogether;

        scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
        model = alTogether * scale;
        drawCube(cubeVAO, lightingShader, model, trunkColor.r, trunkColor.g, trunkColor.b); // Use brown color for trunk
        drawTreeWithFractiles(cubeVAO, lightingShader, next, length, height, width, N + 1);
    }

    else if (N > 0 && N < 3) {
        // Increase the size of the branches
        float length = L * 0.8;  // Decrease scaling factor to grow tree parts
        float height = H * 0.8;
        float width = W * 0.8;

        // Set color based on recursion level (N)
        glm::vec3 color = (N < 2) ? branchColor : trunkColor;  // Pink for branches, brown for trunk

        scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
        rotate = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = alTogether * rotate * scale;
        drawCube(cubeVAO, lightingShader, model, color.r, color.g, color.b);

        scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
        rotate = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = alTogether * rotate * scale;
        drawCube(cubeVAO, lightingShader, model, color.r, color.g, color.b);

        scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
        rotate = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = alTogether * rotate * scale;
        drawCube(cubeVAO, lightingShader, model, color.r, color.g, color.b);

        scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
        rotate = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = alTogether * rotate * scale;
        drawCube(cubeVAO, lightingShader, model, color.r, color.g, color.b);

        scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
        model = alTogether * scale;
        drawCube(cubeVAO, lightingShader, model, color.r, color.g, color.b);

        float mvx = length * 0.3 + height * 0.7071;
        float mvy = height * 0.7071;
        float mvz = width * 0.3;
        translate = glm::translate(identityMatrix, glm::vec3(mvx, mvy, mvz));
        next = translate * alTogether;
        drawTreeWithFractiles(cubeVAO, lightingShader, next, length, height, width, N + 1);

        mvx = length * 0.3;
        mvy = height * 0.7071;
        mvz = width * 0.3 - height * 0.7071;
        translate = glm::translate(identityMatrix, glm::vec3(mvx, mvy, mvz));
        next = translate * alTogether;
        drawTreeWithFractiles(cubeVAO, lightingShader, next, length, height, width, N + 1);

        mvx = length * 0.3 - height * 0.7071;
        mvy = height * 0.7071;
        mvz = width * 0.3;
        translate = glm::translate(identityMatrix, glm::vec3(mvx, mvy, mvz));
        next = translate * alTogether;
        drawTreeWithFractiles(cubeVAO, lightingShader, next, length, height, width, N + 1);

        mvx = length * 0.3;
        mvy = height * 0.7071;
        mvz = width * 0.3 + height * 0.7071;
        translate = glm::translate(identityMatrix, glm::vec3(mvx, mvy, mvz));
        next = translate * alTogether;
        drawTreeWithFractiles(cubeVAO, lightingShader, next, length, height, width, N + 1);

        mvx = length * 0.3;
        mvy = height;
        mvz = width * 0.3;
        translate = glm::translate(identityMatrix, glm::vec3(mvx, mvy, mvz));
        next = translate * alTogether;
        drawTreeWithFractiles(cubeVAO, lightingShader, next, length, height, width, N + 1);
    }

    else if (N == 3) {
        // Final scaling adjustment for the upper branches
        float length = L * 0.8;  // Again using a larger scaling factor
        float height = H * 0.8;
        float width = W * 0.8;

        glm::vec3 color = trunkColor;  // Use brown for the final trunk part

        scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
        rotate = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = alTogether * rotate * scale;
        drawCube(cubeVAO, lightingShader, model, color.r, color.g, color.b);

        scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
        rotate = glm::rotate(identityMatrix, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = alTogether * rotate * scale;
        drawCube(cubeVAO, lightingShader, model, color.r, color.g, color.b);

        scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
        rotate = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = alTogether * rotate * scale;
        drawCube(cubeVAO, lightingShader, model, color.r, color.g, color.b);

        scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
        rotate = glm::rotate(identityMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = alTogether * rotate * scale;
        drawCube(cubeVAO, lightingShader, model, color.r, color.g, color.b);

        scale = glm::scale(identityMatrix, glm::vec3(length, height, width));
        model = alTogether * scale;
        drawCube(cubeVAO, lightingShader, model, color.r, color.g, color.b);
    }
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        camera.ProcessKeyboard(YAWR, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        camera.ProcessKeyboard(YAWL, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        camera.ProcessKeyboard(PITCHU, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        camera.ProcessKeyboard(PITCHD, deltaTime);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }


    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        if (specularToggle)
        {

            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();

            specularToggle = !specularToggle;
        }
        else
        {

            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }

    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        if (diffuseToggle)
        {

            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {

            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }

    else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        if (ambientToggle)
        {

            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {

            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }

    else if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        if (spotLightOn)
        {

            spotlight.turnOff();
            spotLightOn = !spotLightOn;
        }
        else
        {
            spotlight.turnOn();
            spotLightOn = !spotLightOn;
        }
    }

    else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        if (dayLightOn)
        {

            daylight.turnOff();
            dayLightOn = false;
        }
        else
        {
            daylight.turnOn();
            dayLightOn = true;
        }
    }

    else if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        if (moonLightOn)
        {

            moonlight.turnOff();
            moonLightOn = !moonLightOn;
        }
        else
        {
            moonlight.turnOn();
            moonLightOn = !moonLightOn;
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
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
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    //camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


