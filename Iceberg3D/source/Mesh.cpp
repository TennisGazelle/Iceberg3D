#include "Mesh.h"

using namespace iceberg;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, const std::vector<Texture*>& textures)
{
    vertices_ = vertices;
    indices_ = indices;
    textures_ = textures;

    meshVAO_ = std::make_shared<VAO>();
    meshVBO_ = std::make_shared<Buffer>();
    meshIBO_ = std::make_shared<Buffer>();

    setup_mesh();
}

Mesh::~Mesh()
{
}

void Mesh::draw(const ShaderProgram* shaderProgram) const
{
    int diffuseCount = 1;
    int specularCount = 1;
    int textureCount = 0;
    std::string name;
    for (auto const& texture : textures_)
    {
        if (texture->type() == aiTextureType_DIFFUSE)
        {
            name = "texture_diffuse" + std::to_string(diffuseCount++);
        }
        else if (texture->type() == aiTextureType_SPECULAR)
        {
            name = "texture_specular" + std::to_string(specularCount++);
        }
        else
        {
            // Other kinds of texture maps are not handled yet
            continue;
        }
        shaderProgram->set_uniform(name, textureCount);
        texture->bind(textureCount++);
    }

    shaderProgram->use();
    meshVAO_->bind();
    glDrawElements(GL_TRIANGLES, indices_.size(), GL_UNSIGNED_INT, 0);
    meshVAO_->unbind();

    for(auto const& texture : textures_)
    {
        texture->unbind();
    }
}

void Mesh::setup_mesh()
{
    meshVAO_->bind();

    // Add vertices
    meshVBO_->bind();
    meshVBO_->add_data(&vertices_.front(), sizeof(Vertex) * vertices_.size());
    meshVBO_->upload_data();

    // Add indices 
    meshIBO_->bind(GL_ELEMENT_ARRAY_BUFFER);
    meshIBO_->add_data(&indices_.front(), sizeof(GLuint) * indices_.size());
    meshIBO_->upload_data();

    // Vertex Positions
    meshVAO_->enable_attribute(0);
    meshVAO_->configure_attribute(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

    // Vertex Normal Coordinates
    meshVAO_->enable_attribute(1);
    meshVAO_->configure_attribute(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

    // Vertex Texture Coordinates
    meshVAO_->enable_attribute(2);
    meshVAO_->configure_attribute(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex_coords));

    meshVAO_->unbind();
}
