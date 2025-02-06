//
//  cube.h
//  test
//
//  Created by Nazirul Hasan on 4/10/23.
//

#ifndef cube_h
#define cube_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

using namespace std;

class Cube {
public:

    // materialistic property
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // texture property
    float TXmin = 0.0f;
    float TXmax = 1.0f;
    float TYmin = 0.0f;
    float TYmax = 1.0f;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // common property
    float shininess;

    // constructors
    Cube()
    {
        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    Cube(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;

        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    Cube(unsigned int dMap, unsigned int sMap, float shiny, float textureXmin, float textureYmin, float textureXmax, float textureYmax)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
        this->TXmin = textureXmin;
        this->TYmin = textureYmin;
        this->TXmax = textureXmax;
        this->TYmax = textureYmax;

        setUpCubeVertexDataAndConfigureVertexAttribute();
    }

    // destructor
    ~Cube()
    {
        glDeleteVertexArrays(1, &cubeVAO);
        glDeleteVertexArrays(1, &lightCubeVAO);
        glDeleteVertexArrays(1, &lightTexCubeVAO);
        glDeleteBuffers(1, &cubeVBO);
        glDeleteBuffers(1, &cubeEBO);
    }

    void drawCubeWithTexture(Shader& lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShaderWithTexture.use();

        lightingShaderWithTexture.setInt("material.diffuse", 0);
        lightingShaderWithTexture.setInt("material.specular", 1);
        lightingShaderWithTexture.setFloat("material.shininess", this->shininess);


        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specularMap);

        lightingShaderWithTexture.setMat4("model", model);

        glBindVertexArray(lightTexCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawCubeWithMaterialisticProperty(Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f))
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void drawCube(Shader& shader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
    {
        shader.use();

        shader.setVec3("color", glm::vec3(r, g, b));
        shader.setMat4("model", model);

        glBindVertexArray(cubeVAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    }

    void setMaterialisticProperty(glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    void setTextureProperty(unsigned int dMap, unsigned int sMap, float shiny)
    {
        this->diffuseMap = dMap;
        this->specularMap = sMap;
        this->shininess = shiny;
    }

private:
    unsigned int cubeVAO;
    unsigned int lightCubeVAO;
    unsigned int lightTexCubeVAO;
    unsigned int cubeVBO;
    unsigned int cubeEBO;

    void setUpCubeVertexDataAndConfigureVertexAttribute()
    {
        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------

        float cube_vertices[] = {
            // positions      // normals         // texture
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmin,
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmin,
            1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmax,
            0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmax,

            1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, TXmax, TYmin,
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, TXmax, TYmax,
            1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, TXmin, TYmin,
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, TXmin, TYmax,

            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,
            1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmax, TYmin,
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmax, TYmax,
            0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmin, TYmax,

            0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, TXmax, TYmin,
            0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
            0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmax,
            0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmin,

            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, TXmax, TYmin,
            1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, TXmax, TYmax,
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, TXmin, TYmax,
            0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, TXmin, TYmin,

            0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmin,
            1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmin,
            1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmax,
            0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmax
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

        glGenVertexArrays(1, &cubeVAO);
        glGenVertexArrays(1, &lightCubeVAO);
        glGenVertexArrays(1, &lightTexCubeVAO);
        glGenBuffers(1, &cubeVBO);
        glGenBuffers(1, &cubeEBO);


        glBindVertexArray(lightTexCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // vertex normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);

        // texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
        glEnableVertexAttribArray(2);


        glBindVertexArray(lightCubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
        glEnableVertexAttribArray(1);


        glBindVertexArray(cubeVAO);

        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

};


class DiscTex
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    // ctor/dtor
    DiscTex(int flag = 1, float radius = 6.0f, int sectorCount = 50, int stackCount = 25, glm::vec3 amb = glm::vec3(0.922, 0.91, 0.902), glm::vec3 diff = glm::vec3(0.922, 0.91, 0.902), glm::vec3 spec = glm::vec3(0.5f, 0.5f, 0.5f), float shiny = 32.0f) : verticesStride(32)
    {
        set(radius, sectorCount, stackCount, amb, diff, spec, shiny);
        buildCoordinatesAndIndices(flag);
        buildVertices();

        glGenVertexArrays(1, &sphereVAO);
        glBindVertexArray(sphereVAO);

        // create VBO to copy vertex data to VBO
        unsigned int sphereVBO;
        glGenBuffers(1, &sphereVBO);
        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);           // for vertex data
        glBufferData(GL_ARRAY_BUFFER,                   // target
            this->getVertexSize(), // data size, # of bytes
            this->getVertices(),   // ptr to vertex data
            GL_STATIC_DRAW);                   // usage

        // create EBO to copy index data
        unsigned int sphereEBO;
        glGenBuffers(1, &sphereEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);   // for index data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
            this->getIndexSize(),             // data size, # of bytes
            this->getIndices(),               // ptr to index data
            GL_STATIC_DRAW);                   // usage

        // activate attrib arrays
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // set attrib arrays with stride and offset
        int stride = this->getVerticesStride();     // should be 24 bytes
        glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6));      // Texture Coordinates

        // unbind VAO and VBOs
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    ~DiscTex() {}

    // getters/setters

    void set(float radius, int sectors, int stacks, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        if (radius > 0)
            this->radius = radius;
        this->sectorCount = sectors;
        if (sectors < MIN_SECTOR_COUNT)
            this->sectorCount = MIN_SECTOR_COUNT;
        this->stackCount = stacks;
        if (stacks < MIN_STACK_COUNT)
            this->stackCount = MIN_STACK_COUNT;
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    // for interleaved vertices
    unsigned int getVertexCount() const
    {
        return (unsigned int)coordinates.size() / 3;     // # of vertices
    }

    unsigned int getVertexSize() const
    {
        return (unsigned int)vertices.size() * sizeof(float);  // # of bytes
    }

    int getVerticesStride() const
    {
        return verticesStride;   // should be 32 bytes
    }
    const float* getVertices() const
    {
        return vertices.data();
    }

    unsigned int getIndexSize() const
    {
        return (unsigned int)indices.size() * sizeof(unsigned int);
    }

    const unsigned int* getIndices() const
    {
        return indices.data();
    }

    unsigned int getIndexCount() const
    {
        return (unsigned int)indices.size();
    }

    // draw in VertexArray mode
    void drawCylinder(Shader& lightingShader, unsigned int texture, glm::mat4 model) const      // draw surface
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // draw a sphere with VAO
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            this->getIndexCount(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                       // offset to indices

        // unbind VAO
        glBindVertexArray(0);
    }

private:
    // member functions
    void buildCoordinatesAndIndices(int flag)
    {
        float x, z;                              // vertex position
        float nx = 0.0f, ny = 1.0f, nz = 0.0f;    // vertex normal (all pointing up for the disc)
        float s, t;                              // texture coordinates

        float sectorStep = 2 * PI / sectorCount; // angular step for sectors
        float sectorAngle = 0.0f;

        // Center vertex for the disc
        coordinates.push_back(0.0f);
        coordinates.push_back(0.0f);
        coordinates.push_back(0.0f);

        normals.push_back(nx);
        normals.push_back(ny);
        normals.push_back(nz);

        texCoords.push_back(0.5f); // Texture coordinate for center
        texCoords.push_back(0.5f);

        // Generate vertices around the edge of the disc
        for (int i = 0; i <= sectorCount / flag; ++i)
        {
            sectorAngle = i * sectorStep;
            x = radius * cosf(sectorAngle);
            z = radius * sinf(sectorAngle);

            // Vertex position (x, y, z)
            coordinates.push_back(x);
            coordinates.push_back(0.0f); // Y-coordinate is 0 for the disc
            coordinates.push_back(z);

            // Normal vector (nx, ny, nz)
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            // Texture coordinates (s, t)
            s = (x / radius + 1.0f) * 0.5f; // Map to [0, 1]
            t = (z / radius + 1.0f) * 0.5f; // Map to [0, 1]
            texCoords.push_back(s);
            texCoords.push_back(t);
        }

        // Generate indices for the disc
        for (int i = 1; i <= sectorCount / flag; ++i)
        {
            indices.push_back(0);        // Center vertex index
            indices.push_back(i);        // Current edge vertex
            indices.push_back(i + 1);    // Next edge vertex
        }
    }

    void buildVertices()
    {
        size_t i, j;
        size_t count = coordinates.size();
        for (i = 0, j = 0; i < count; i += 3, j += 2)
        {
            vertices.push_back(coordinates[i]);
            vertices.push_back(coordinates[i + 1]);
            vertices.push_back(coordinates[i + 2]);

            vertices.push_back(normals[i]);
            vertices.push_back(normals[i + 1]);
            vertices.push_back(normals[i + 2]);

            size_t texIndex = i / 3 * 2;
            vertices.push_back(texCoords[texIndex]);
            vertices.push_back(texCoords[texIndex + 1]);
        }
    }

    vector<float> computeFaceNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
    {
        const float EPSILON = 0.000001f;

        vector<float> normal(3, 0.0f);     // default return value (0,0,0)
        float nx, ny, nz;

        // find 2 edge vectors: v1-v2, v1-v3
        float ex1 = x2 - x1;
        float ey1 = y2 - y1;
        float ez1 = z2 - z1;
        float ex2 = x3 - x1;
        float ey2 = y3 - y1;
        float ez2 = z3 - z1;

        // cross product: e1 x e2
        nx = ey1 * ez2 - ez1 * ey2;
        ny = ez1 * ex2 - ex1 * ez2;
        nz = ex1 * ey2 - ey1 * ex2;

        // normalize only if the length is > 0
        float length = sqrtf(nx * nx + ny * ny + nz * nz);
        if (length > EPSILON)
        {
            // normalize
            float lengthInv = 1.0f / length;
            normal[0] = nx * lengthInv;
            normal[1] = ny * lengthInv;
            normal[2] = nz * lengthInv;
        }

        return normal;
    }

    // memeber vars
    unsigned int sphereVAO;
    float radius;
    int sectorCount;                        // longitude, # of slices
    int stackCount;                         // latitude, # of stacks
    vector<float> vertices;
    vector<float> normals;
    vector<float> texCoords;
    vector<unsigned int> indices;
    vector<float> coordinates;
    int verticesStride;                 // # of bytes to hop to the next vertex (should be 24 bytes)

};


class HalfTorus
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    // Constructor
    HalfTorus(int flag = 1, float radius = 1.0f, float tubeRadius = 0.4f, int sectorCount = 36, int stackCount = 18, glm::vec3 amb = glm::vec3(0.922, 0.91, 0.902), glm::vec3 diff = glm::vec3(0.922, 0.91, 0.902), glm::vec3 spec = glm::vec3(0.5f, 0.5f, 0.5f), float shiny = 32.0f) : verticesStride(32)
    {
        set(radius, tubeRadius, sectorCount, stackCount, amb, diff, spec, shiny);
        buildCoordinatesAndIndices(flag);
        buildVertices();

        glGenVertexArrays(1, &torusVAO);
        glBindVertexArray(torusVAO);

        // create VBO to copy vertex data to VBO
        unsigned int torusVBO;
        glGenBuffers(1, &torusVBO);
        glBindBuffer(GL_ARRAY_BUFFER, torusVBO);           // for vertex data
        glBufferData(GL_ARRAY_BUFFER,                   // target
            this->getVertexSize(), // data size, # of bytes
            this->getVertices(),   // ptr to vertex data
            GL_STATIC_DRAW);                   // usage

        // create EBO to copy index data
        unsigned int torusEBO;
        glGenBuffers(1, &torusEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, torusEBO);   // for index data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
            this->getIndexSize(),             // data size, # of bytes
            this->getIndices(),               // ptr to index data
            GL_STATIC_DRAW);                   // usage

        // activate attrib arrays
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        // set attrib arrays with stride and offset
        int stride = this->getVerticesStride();     // should be 32 bytes
        glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 6));      // Texture Coordinates

        // unbind VAO and VBOs
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    ~HalfTorus() {}

    // getters/setters
    void set(float radius, float tubeRadius, int sectors, int stacks, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        if (radius > 0)
            this->radius = radius;
        if (tubeRadius > 0)
            this->tubeRadius = tubeRadius;
        this->sectorCount = sectors;
        if (sectors < MIN_SECTOR_COUNT)
            this->sectorCount = MIN_SECTOR_COUNT;
        this->stackCount = stacks;
        if (stacks < MIN_STACK_COUNT)
            this->stackCount = MIN_STACK_COUNT;
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    // for interleaved vertices
    unsigned int getVertexCount() const
    {
        return (unsigned int)coordinates.size() / 3;     // # of vertices
    }

    unsigned int getVertexSize() const
    {
        return (unsigned int)vertices.size() * sizeof(float);  // # of bytes
    }

    int getVerticesStride() const
    {
        return verticesStride;   // should be 32 bytes
    }
    const float* getVertices() const
    {
        return vertices.data();
    }

    unsigned int getIndexSize() const
    {
        return (unsigned int)indices.size() * sizeof(unsigned int);
    }

    const unsigned int* getIndices() const
    {
        return indices.data();
    }

    unsigned int getIndexCount() const
    {
        return (unsigned int)indices.size();
    }

    // draw in VertexArray mode
    void drawHalfTorus(Shader& lightingShader, unsigned int texture, glm::mat4 model) const      // draw surface
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        // draw a half torus with VAO
        glBindVertexArray(torusVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            this->getIndexCount(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                       // offset to indices

        // unbind VAO
        glBindVertexArray(0);
    }

private:
    // member functions
    void buildCoordinatesAndIndices(int flag)
    {
        float x, y, z;                          // vertex position
        float s, t;                             // texture coordinates

        float sectorStep = PI / sectorCount;   // 180 degrees
        float stackStep = 2 * PI / stackCount; // Full circle
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = i * stackStep;
            for (int j = 0; j <= sectorCount / flag; ++j)
            {
                sectorAngle = j * sectorStep;

                x = (radius + tubeRadius * cosf(stackAngle)) * cosf(sectorAngle);
                y = tubeRadius * sinf(stackAngle);
                z = (radius + tubeRadius * cosf(stackAngle)) * sinf(sectorAngle);

                coordinates.push_back(x);
                coordinates.push_back(y);
                coordinates.push_back(z);

                // Normal vector (nx, ny, nz)
                glm::vec3 normal = glm::normalize(glm::vec3(x - radius * cosf(sectorAngle), y, z - radius * sinf(sectorAngle)));
                normals.push_back(normal.x);
                normals.push_back(normal.y);
                normals.push_back(normal.z);

                // Texture coordinates (s, t)
                s = (float)j / sectorCount;
                t = (float)i / stackCount;
                texCoords.push_back(s);
                texCoords.push_back(t);
            }
        }

        // Generate indices for the half torus
        for (int i = 0; i < stackCount; ++i)
        {
            int k1 = i * (sectorCount / flag + 1);
            int k2 = k1 + sectorCount / flag + 1;

            for (int j = 0; j < sectorCount / flag; ++j, ++k1, ++k2)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);

                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    void buildVertices()
    {
        size_t i, j;
        size_t count = coordinates.size();
        for (i = 0, j = 0; i < count; i += 3, j += 2)
        {
            vertices.push_back(coordinates[i]);
            vertices.push_back(coordinates[i + 1]);
            vertices.push_back(coordinates[i + 2]);

            vertices.push_back(normals[i]);
            vertices.push_back(normals[i + 1]);
            vertices.push_back(normals[i + 2]);

            size_t texIndex = i / 3 * 2;
            vertices.push_back(texCoords[texIndex]);
            vertices.push_back(texCoords[texIndex + 1]);
        }
    }

    // member variables
    unsigned int torusVAO;
    float radius;
    float tubeRadius;
    int sectorCount;
    int stackCount;
    vector<float> vertices;
    vector<float> normals;
    vector<float> texCoords;
    vector<unsigned int> indices;
    vector<float> coordinates;
    int verticesStride; // # of bytes to hop to the next vertex (should be 32 bytes)

};

class Torus
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    // ctor/dtor
    Torus(int part = 1, float Dis = 1.0f, float rA = 0.3f, float radius = 1.0f, int sectorCount = 36, int stackCount = 18, glm::vec3 amb = glm::vec3(0.412, 0.118, 0.031), glm::vec3 diff = glm::vec3(0.412, 0.118, 0.031), glm::vec3 spec = glm::vec3(0.5f, 0.5f, 0.5f), float shiny = 32.0f) : verticesStride(24)
    {
        set(radius, sectorCount, stackCount, amb, diff, spec, shiny);
        buildCoordinatesAndIndices(part, Dis, rA);
        buildVertices();

        glGenVertexArrays(1, &sphereVAO);
        glBindVertexArray(sphereVAO);

        // create VBO to copy vertex data to VBO
        unsigned int sphereVBO;
        glGenBuffers(1, &sphereVBO);
        glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);           // for vertex data
        glBufferData(GL_ARRAY_BUFFER,                   // target
            this->getVertexSize(), // data size, # of bytes
            this->getVertices(),   // ptr to vertex data
            GL_STATIC_DRAW);                   // usage

        // create EBO to copy index data
        unsigned int sphereEBO;
        glGenBuffers(1, &sphereEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);   // for index data
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
            this->getIndexSize(),             // data size, # of bytes
            this->getIndices(),               // ptr to index data
            GL_STATIC_DRAW);                   // usage

        // activate attrib arrays
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        // set attrib arrays with stride and offset
        int stride = this->getVerticesStride();     // should be 24 bytes
        glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

        // unbind VAO and VBOs
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    ~Torus() {}

    // getters/setters

    void set(float radius, int sectors, int stacks, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        if (radius > 0)
            this->radius = radius;
        this->sectorCount = sectors;
        if (sectors < MIN_SECTOR_COUNT)
            this->sectorCount = MIN_SECTOR_COUNT;
        this->stackCount = stacks;
        if (stacks < MIN_STACK_COUNT)
            this->stackCount = MIN_STACK_COUNT;
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    void setRadius(float radius)
    {
        if (radius != this->radius)
            set(radius, sectorCount, stackCount, ambient, diffuse, specular, shininess);
    }

    void setSectorCount(int sectors)
    {
        if (sectors != this->sectorCount)
            set(radius, sectors, stackCount, ambient, diffuse, specular, shininess);
    }

    void setStackCount(int stacks)
    {
        if (stacks != this->stackCount)
            set(radius, sectorCount, stacks, ambient, diffuse, specular, shininess);
    }

    // for interleaved vertices
    unsigned int getVertexCount() const
    {
        return (unsigned int)coordinates.size() / 3;     // # of vertices
    }

    unsigned int getVertexSize() const
    {
        return (unsigned int)vertices.size() * sizeof(float);  // # of bytes
    }

    int getVerticesStride() const
    {
        return verticesStride;   // should be 24 bytes
    }
    const float* getVertices() const
    {
        return vertices.data();
    }

    unsigned int getIndexSize() const
    {
        return (unsigned int)indices.size() * sizeof(unsigned int);
    }

    const unsigned int* getIndices() const
    {
        return indices.data();
    }

    unsigned int getIndexCount() const
    {
        return (unsigned int)indices.size();
    }

    // draw in VertexArray mode
    void drawTorus(Shader& lightingShader, glm::mat4 model) const      // draw surface
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        lightingShader.setMat4("model", model);

        // draw a sphere with VAO
        glBindVertexArray(sphereVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            this->getIndexCount(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                       // offset to indices

        // unbind VAO
        glBindVertexArray(0);
    }

private:
    // member functions
    void buildCoordinatesAndIndices(int part, float Dis, float rA)
    {
        float x, y, z, xz;                                // vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius;      // vertex normal

        // Adjust sectorCount and stackCount for one-fourth of the torus
        float sectorStep = 2 * PI / sectorCount;          // angular step for each slice (u)
        float stackStep = 2 * PI / stackCount;           // angular step for each profile (v)
        float sectorAngle = 0.0f;                         // starting angle for sector
        float stackAngle = 0.0f;                          // starting angle for stack

        float D = Dis;                                   // Displacement (distance from center of torus to tube center)
        float A = rA;                                   // Radius of the tube

        // Iterate through the stack (v) and sector (u) ranges for one-fourth of the torus
        for (int i = 0; i <= stackCount; ++i)
        {
            stackAngle = i * stackStep;                   // Calculate the angle for each profile (v)

            for (int j = 0; j <= sectorCount / part; ++j)    // Iterate only through one-fourth of the sectors
            {
                sectorAngle = j * sectorStep;             // Calculate the angle for each slice (u)

                // Calculate the position of the vertex (x, y, z) on the torus surface
                xz = D + A * cosf(stackAngle);            // Radius at this height (stack angle)
                x = xz * cosf(sectorAngle);               // x = (D + A * cos(v)) * cos(u)
                y = xz * sinf(sectorAngle);               // y = (D + A * cos(v)) * sin(u)
                z = A * sinf(stackAngle);                 // z = A * sin(v)

                coordinates.push_back(x);
                coordinates.push_back(y);
                coordinates.push_back(z);

                // Calculate the normal vector for the torus surface
                nx = cosf(stackAngle) * cosf(sectorAngle); // Normal for torus
                ny = cosf(stackAngle) * sinf(sectorAngle);
                nz = sinf(stackAngle);

                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);
            }
        }

        // Generate index list of sphere triangles
        int k1, k2;
        for (int i = 0; i < stackCount; ++i)
        {
            k1 = i * (sectorCount / part + 1);     // beginning of current stack
            k2 = k1 + sectorCount / part + 1;      // beginning of next stack

            for (int j = 0; j < sectorCount / part; ++j, ++k1, ++k2)    // Only generate triangles for one-fourth of the sector range
            {
                // 2 triangles per sector excluding first and last stacks
                // k1 => k2 => k1+1
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);

                // k1+1 => k2 => k2+1
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }

    }

    void buildVertices()
    {
        size_t i, j;
        size_t count = coordinates.size();
        for (i = 0, j = 0; i < count; i += 3, j += 2)
        {
            vertices.push_back(coordinates[i]);
            vertices.push_back(coordinates[i + 1]);
            vertices.push_back(coordinates[i + 2]);

            vertices.push_back(normals[i]);
            vertices.push_back(normals[i + 1]);
            vertices.push_back(normals[i + 2]);
        }
    }

    vector<float> computeFaceNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
    {
        const float EPSILON = 0.000001f;

        vector<float> normal(3, 0.0f);     // default return value (0,0,0)
        float nx, ny, nz;

        // find 2 edge vectors: v1-v2, v1-v3
        float ex1 = x2 - x1;
        float ey1 = y2 - y1;
        float ez1 = z2 - z1;
        float ex2 = x3 - x1;
        float ey2 = y3 - y1;
        float ez2 = z3 - z1;

        // cross product: e1 x e2
        nx = ey1 * ez2 - ez1 * ey2;
        ny = ez1 * ex2 - ex1 * ez2;
        nz = ex1 * ey2 - ey1 * ex2;

        // normalize only if the length is > 0
        float length = sqrtf(nx * nx + ny * ny + nz * nz);
        if (length > EPSILON)
        {
            // normalize
            float lengthInv = 1.0f / length;
            normal[0] = nx * lengthInv;
            normal[1] = ny * lengthInv;
            normal[2] = nz * lengthInv;
        }

        return normal;
    }

    // memeber vars
    unsigned int sphereVAO;
    float radius;
    int sectorCount;                        // longitude, # of slices
    int stackCount;                         // latitude, # of stacks
    vector<float> vertices;
    vector<float> normals;
    vector<unsigned int> indices;
    vector<float> coordinates;
    int verticesStride;                 // # of bytes to hop to the next vertex (should be 24 bytes)

};


#endif /* cube_h */
