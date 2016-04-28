#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

#include "GlobalIncludes.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "VBO.h"
#include "VAO.h"
#include "Game.h"

namespace iceberg
{
    struct GlyphInfo
    {
        Texture texture;
        glm::ivec2 size;
        glm::ivec2 bearing;
        int advance;
        int scale;
    };

    typedef std::map<GLchar, GlyphInfo*> Font;
    typedef std::map<std::string, Font> FontCache;

    class ICEBERGAPI TextManager
    {
    public:
        TextManager(Game* game);
        ~TextManager();

        void load_font(const std::string& path, int scale);

        // Uses the default orthographic, 2D text shader
        void RenderText(const std::string& text, int xPos, int yPos, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f));

    private:

        FontCache fontCache_;
        std::unique_ptr<ShaderProgram> shaderProgram_;
        std::string currentFont_;
        bool fontLoaded_;

        std::unique_ptr<iceberg::VAO> textVAO_;
        std::unique_ptr<iceberg::VBO> textVBO_;
        std::unique_ptr<iceberg::VBO> textIBO_;

    };
}
#endif