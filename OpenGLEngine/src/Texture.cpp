#include "Texture.h"
#include <cassert>

Texture::Texture(const char* path) {
	stbi_set_flip_vertically_on_load(true);

    stbi_uc* data = stbi_load(path, &m_Width, &m_Height, &m_Channels, 0);
    assert(data);

    unsigned int internalFormat = 0, dataFormat = 0;
    if (m_Channels == 4) {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    }
    else if (m_Channels == 3) {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
    glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

    glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_RendererID);
}

void Texture::bind(unsigned int slot) const {
    glBindTextureUnit(slot, m_RendererID);
}
