#include "Skybox.h"
#include <stb_image.h>

using namespace std;
using namespace glm;

Skybox::Skybox(const SkyboxParameters &skyboxParameters)
{
    vector<const GLchar*> faces;
    faces.push_back(skyboxParameters.right.c_str());
    faces.push_back(skyboxParameters.left.c_str());
    faces.push_back(skyboxParameters.top.c_str());
    faces.push_back(skyboxParameters.bottom.c_str());
    faces.push_back(skyboxParameters.back.c_str());
    faces.push_back(skyboxParameters.front.c_str());

    skyboxProgram_.create_program();
    skyboxProgram_.add_shader_from_file("Shaders/skyVert.glsl", GL_VERTEX_SHADER);
    skyboxProgram_.add_shader_from_file("Shaders/skyFrag.glsl", GL_FRAGMENT_SHADER);
    skyboxProgram_.link_program();

    cubemap_ = make_unique<Texture>(aiTextureType_DIFFUSE, GL_TEXTURE_CUBE_MAP);

    for (GLuint i = 0; i < faces.size(); i++)
    {
        cubemap_->load(faces[i], GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
    }

    cubemap_->set_wrap();

    GLfloat skyboxVertices[] =
    {
        // Positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
    };

    skyboxVAO_.create();
    skyboxVBO_.create();

    skyboxVAO_.bind();

    skyboxVBO_.bind();
    skyboxVBO_.add_data(&skyboxVertices, sizeof(skyboxVertices));
    skyboxVBO_.upload_data();

    skyboxVAO_.enable_attribute(0);
    skyboxVAO_.configure_attribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    skyboxVAO_.unbind();
}

Skybox::~Skybox()
{
}

void Skybox::draw(const Camera* camera) const
{
    glDepthFunc(GL_LEQUAL);

    skyboxProgram_.use();

    glm::mat4 view = glm::mat4(glm::mat3(camera->view_matrix()));
    skyboxProgram_.set_uniform("projection", camera->projection_matrix());
    skyboxProgram_.set_uniform("view", view);

    skyboxVAO_.bind();

    cubemap_->bind();
    skyboxProgram_.set_uniform("skybox", 0);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    skyboxVAO_.unbind();

    glDepthFunc(GL_LESS);
}

