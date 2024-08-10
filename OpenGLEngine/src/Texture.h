#pragma once

#include "stb_image/stb_image.h"

#include "GLLog.h"

class Texture {
	unsigned int m_RendererID;
	unsigned char* m_LocalBuffer;
	int m_Height;
	int m_Width;
	int m_BPP;

public:
	Texture(const char* path);
	~Texture();

	void bind(unsigned int slot) const;
	void unbind();
};