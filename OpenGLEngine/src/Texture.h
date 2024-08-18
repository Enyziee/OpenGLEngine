#pragma once

#include "stb_image/stb_image.h"
#include <glad/glad.h>

class Texture {
	unsigned int m_RendererID;
	int m_Height;
	int m_Width;
	int m_Channels;

public:
	Texture(const char* path);
	~Texture();

	void bind(unsigned int slot = 0) const;
};