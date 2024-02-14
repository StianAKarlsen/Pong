#include "defines.hpp"

#include "ShaderProgramManager.hpp"
#include "text.hpp"

Text::Text()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));

    LoadCharacterTextures();

    //TODO: useless....
    auto &shaderManager = ShaderManager::getInstance();
    textShaderProgram = shaderManager.getShaderProgram("sazxd");
};

void Text::LoadCharacterTextures()
{
    if (FT_Init_FreeType(&ft))
    {
        std::cerr << "Could not init FreeType Library" << std::endl;
    }
    if (FT_New_Face(ft, "C:\\Windows\\fonts\\Arial.ttf", 0, &face))
    // if (FT_New_Face(ft, "OpenDyslexic3-Regular.ttf", 0, &face))
    {
        std::cerr << "Failed to load font" << std::endl;
    }
    FT_Set_Pixel_Sizes(face, 0, 48 * 2);
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cerr << "Failed to load Glyph" << std::endl;
            continue;
        }

        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);

        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Store character for later use
        Character character = {
            texture,
            {(int)face->glyph->bitmap.width, (int)face->glyph->bitmap.rows},
            {face->glyph->bitmap_left, face->glyph->bitmap_top},
            static_cast<GLuint>(face->glyph->advance.x)};
        characters.insert(std::pair<GLchar, Character>(c, character));
    }
}

void Text::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale)
{
    GLfloat textWidth = 0.0f;
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = characters[*c];
        textWidth += (ch.Advance >> 6) * scale;
    }
    x -= textWidth / 2.0f;

    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;

        // Update VBO for each character
        GLfloat vertices[6][4] = {
            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos, ypos, 0.0f, 1.0f},
            {xpos + w, ypos, 1.0f, 1.0f},

            {xpos, ypos + h, 0.0f, 0.0f},
            {xpos + w, ypos, 1.0f, 1.0f},
            {xpos + w, ypos + h, 1.0f, 0.0f}};

        // Render glyph texture over quad
        static const GLfloat pos[] = {0, 0};

        // TODO: textShaderProgram burde ikke trenge å søke hver gang.
        auto &shaderManager = ShaderManager::getInstance();
        textShaderProgram = shaderManager.getShaderProgram("TextShaderProgram");
        textShaderProgram->use();
        textShaderProgram->setTexture("textureSampler", ch.TextureID, 0);
        textShaderProgram->setUniform("modelPos", pos);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Now advance cursors for next glyph
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    // glBindVertexArray(0);
    // glBindTexture(GL_TEXTURE_2D, 0);
    // glActiveTexture(0);
}

void Text::CleanUp()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    for (auto c : characters)
        glDeleteTextures(1, &c.second.TextureID);
}
