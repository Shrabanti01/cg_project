#ifndef pyramid_h
#define pyramid_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std;

class Pyramid {
public:
    // Material properties
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // Texture properties
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // Common properties
    float shininess;

    // Vertex Array Objects, Buffers, and Elements
    unsigned int pyramidVAO, lightPyramidVAO, lightTexPyramidVAO;
    unsigned int pyramidVBO, pyramidEBO;

    // Constructors
    Pyramid()
    {
        setUpPyramidVertexDataAndConfigureVertexAttribute();
    }

    Pyramid(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpPyramidVertexDataAndConfigureVertexAttribute();
    }

    Pyramid(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpPyramidVertexDataAndConfigureVertexAttribute();
    }

    // Destructor
    ~Pyramid()
    {
        glDeleteVertexArrays(1, &pyramidVAO);
        glDeleteVertexArrays(1, &lightPyramidVAO);
        glDeleteVertexArrays(1, &lightTexPyramidVAO);
        glDeleteBuffers(1, &pyramidVBO);
        glDeleteBuffers(1, &pyramidEBO);
    }

    // Draw Pyramid with texture
    void drawPyramidWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);

        // Bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);

        // Bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexPyramidVAO);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    }

    // Draw Pyramid with material properties
    void drawPyramidWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightPyramidVAO);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    }

    // Draw Pyramid with a color (no texture, no material properties)
    void drawPyramid(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
        lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
        lightingShader.setVec3("material.specular", glm::vec3(r, g, b));
        lightingShader.setFloat("material.shininess", 32.0f);

        lightingShader.setMat4("model", model);

        glBindVertexArray(pyramidVAO);
        glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
    }

    // Set Materialistic properties
    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    // Set Texture properties
    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }

private:
    // Method to set up vertex data and configure vertex attributes for the pyramid
    void setUpPyramidVertexDataAndConfigureVertexAttribute()
    {
        float pyramid_vertices[] = {
            // Base (Square) vertices
            -1.0f,  0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // A
             1.0f,  0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // B
             1.0f,  0.0f,  1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // C
            -1.0f,  0.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // D

            // Apex vertex
             0.0f,  1.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f  // E (Apex)
        };

        unsigned int pyramid_indices[] = {
            // Base (Square)
            0, 1, 2,
            0, 2, 3,

            // Side triangles
            0, 1, 4, // Side 1 (A-B-E)
            1, 2, 4, // Side 2 (B-C-E)
            2, 3, 4, // Side 3 (C-D-E)
            3, 0, 4  // Side 4 (D-A-E)
        };

        // Create and bind buffers
        glGenVertexArrays(1, &pyramidVAO);
        glGenVertexArrays(1, &lightPyramidVAO);
        glGenVertexArrays(1, &lightTexPyramidVAO);
        glGenBuffers(1, &pyramidVBO);
        glGenBuffers(1, &pyramidEBO);

        glBindVertexArray(lightTexPyramidVAO);

        glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid_vertices), pyramid_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramid_indices), pyramid_indices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // Texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);

        glBindVertexArray(lightPyramidVAO);

        glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        glBindVertexArray(pyramidVAO);

        glBindBuffer(GL_ARRAY_BUFFER, pyramidVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pyramidEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }
};

#endif // PYRAMID_H
