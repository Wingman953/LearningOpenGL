#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Shader/shader.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

// Stores per vertex data.
struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

// Stores per texture data.
struct Texture
{
    unsigned int id;
    string type;
    string path;
};

///
/// Custom class to store and draw meshes.
///
class Mesh
{
  public:
    // Mesh Data.
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int VAO;

    // Functions.
    Mesh(vector<Vertex> verts, vector<unsigned int> idxs, vector<Texture> txts)
    {
        vertices = verts;
        indices = idxs;
        textures = txts;

        SetupMesh();
    }

    ///
    /// Grab texture data and draw mesh.
    /// \param shader - The shader to send texture data to and draw with.
    ///
    void Draw(Shader shader)
    {
        // Bind appropriate textures.
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr = 1;
        unsigned int heightNr = 1;
        for(unsigned int i = 0; i < textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);

            string number;
            string name = textures[i].type;
            if(name == "texture_diffuse")
            {
                number = std::to_string(diffuseNr++);
            }
            else if(name == "texture_specular")
            {
                number = std::to_string(specularNr++);
            }
            else if(name == "texture_normal")
            {
                number = std::to_string(normalNr++);
            }
            else if(name == "texture_height")
            {
                number = std::to_string(heightNr++);
            }

            glUniform1i(glGetUniformLocation(shader.ProgramID(), (name + number).c_str()), i);
            glBindTexture(GL_TEXTURE_2D, textures[i].id);
        }

        // Draw mesh.
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Good practice to set everything back to defaults once configured.
        glActiveTexture(GL_TEXTURE0);
    }

  private:

    unsigned int VBO, EBO;

    // Functions.

    ///
    /// Creates buffer and stores vertex data in buffer.
    ///
    void SetupMesh()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ( void*) 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ( void*) offsetof(Vertex, Normal));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), ( void*) offsetof(Vertex, TexCoords));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ( void*) offsetof(Vertex, Tangent));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), ( void*) offsetof(Vertex, Bitangent));

        glBindVertexArray(0);
    }
};

#endif