/*
 * Texture.cpp
 *
 *  Created on: Dec 12, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "Texture.h"

Texture::Texture() :
	m_filename(""), m_allocated(false), m_width(0), m_height(0), m_sourceWidth(0), m_sourceHeight(0), m_texID(0),
			m_textureTarget(GL_TEXTURE_2D), m_internalFormat(GL_RGBA8), m_useMipmaps(false), m_hasAlpha(false),
			m_minFilter(TexFilter_Linear), m_magFilter(TexFilter_Linear), m_wrapping(TexWrapMode_Repeat), m_anisotropy(
					1.0f) {
}

Texture::~Texture() {
	if (m_texID) {
		glDeleteTextures(1, &m_texID);
	}
}

/** Allocates graphics resources */
bool Texture::allocate() {
	if (!m_texID) {
		glGenTextures(1, &m_texID);
	}
	glBindTexture(m_textureTarget, m_texID);

	// allocate memory storage for the texture
	if (m_textureTarget == GL_TEXTURE_1D) {
		glTexImage1D(GL_TEXTURE_1D, 0, GL_RGBA8, m_width, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	} else if (m_textureTarget == GL_TEXTURE_2D) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	} else {
		// TODO: support more texture types
	}

	if (m_useMipmaps) {
		//TODO: allocate space for mipmap levels
		glGenerateMipmap(m_textureTarget);
	}

	m_allocated = true;
	return true;
}

void Texture::configureGLState() {

	if (m_texID && glIsTexture(m_texID)) {
		glEnable(m_textureTarget);
		glBindTexture(m_textureTarget, m_texID);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, /*texStack->texOutputs[i].blendOp*/GL_MODULATE);
		if (m_hasAlpha) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		} else {
			glDisable(GL_BLEND);
		}

		// Minification filter for mipmaps or without mipmaps
		if (m_useMipmaps) {
			switch (m_minFilter) {
			case TexFilter_Linear:
			case TexFilter_Linear_Mipmap_Linear:
				glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
				break;
			case TexFilter_Linear_Mipmap_Bilinear:
				glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
				break;
			case TexFilter_Bilinear:
			case TexFilter_Bilinear_Mipmap_Bilinear:
				glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				break;
			case TexFilter_Bilinear_Mipmap_Linear:
			default:
				glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				break;
			}

		} else {
			// no mipmaps
			glTexParameteri(m_textureTarget, GL_TEXTURE_MAX_LEVEL, 0);
			switch (m_minFilter) {
			case TexFilter_Linear:
				glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				break;
			case TexFilter_Bilinear:
			default:
				glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				break;
			}
		}

		// Magnification filter
		switch (m_magFilter) {
		case TexFilter_Linear:
			glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case TexFilter_Bilinear:
		default:
			glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		}

		// wrapping mode
		switch (m_wrapping) {
		case TexWrapMode_Clamp:
			glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP);
			break;
		case TexWrapMode_ClampEdge:
			glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			break;
		case TexWrapMode_Repeat:
		default:
			glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_R, GL_REPEAT);
			break;
		}
	}
}

