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
		glTexImage1D(GL_TEXTURE_1D, 0, m_internalFormat, m_width, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	} else if (m_textureTarget == GL_TEXTURE_2D) {
		glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	} else {
		// TODO: support more texture types
	}

	if (m_useMipmaps) {
		_allocateMipmaps();
		glGenerateMipmap(m_textureTarget);
	}

	m_allocated = true;
	return true;
}

void Texture::_allocateMipmaps() {
	uint32_t w = m_width;
	uint32_t h = m_height;
	uint16_t level = 1;
	while (w > 1 || h > 1) {
		if (w > 1)
			w >>= 1;
		if (h > 1)
			h >>= 1;
		glTexImage2D(GL_TEXTURE_2D, level, m_internalFormat, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		++level;
	}
}

void Texture::fromImage(const Image& img) {
	m_width = img.m_width;
	m_sourceWidth = img.m_width;
	m_height = img.m_height;
	m_sourceHeight = img.m_height;
	m_textureTarget = (m_height > 1) ? GL_TEXTURE_2D : GL_TEXTURE_1D;
	m_internalFormat = _getInternalFormat(img.m_format, img.m_type);
	m_hasAlpha = (m_internalFormat == GL_RGBA || m_internalFormat == GL_BGRA);

	if (allocate()) {
		glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
		//TODO: Get pixel store parameters from image
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);

		if (m_textureTarget == GL_TEXTURE_1D) {
			glTexSubImage1D(GL_TEXTURE_1D, 0, 0, m_width, img.m_format, img.m_type, img.m_data);
		} else if (m_textureTarget == GL_TEXTURE_2D) {
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, img.m_format, img.m_type, img.m_data);
		} else {
			// TODO: support more texture types
		}

		if (m_useMipmaps) {
			glGenerateMipmap(m_textureTarget);
		}

		glPopClientAttrib();
	}
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

GLint Texture::_getInternalFormat(GLenum format, GLenum type) {
	//TODO: Add more cases
	switch (format) {
	case GL_DEPTH_COMPONENT:
		if (type == GL_FLOAT) {
			return GL_DEPTH_COMPONENT32F;
		} else {
			return GL_DEPTH_COMPONENT;
		}

	case GL_RGB:
	case GL_BGR:
		if (type == GL_FLOAT) {
			return GL_RGB32F;
		} else if (type == GL_UNSIGNED_INT) {
			return GL_RGB32UI;
		} else if (type == GL_UNSIGNED_INT) {
			return GL_RGB32I;
		} else {
			return GL_RGB;
		}

	case GL_RGBA:
	case GL_BGRA:
		if (type == GL_FLOAT) {
			return GL_RGBA32F;
		} else if (type == GL_UNSIGNED_INT) {
			return GL_RGBA32UI;
		} else if (type == GL_UNSIGNED_INT) {
			return GL_RGBA32I;
		} else {
			return GL_RGBA;
		}

	case GL_ALPHA:
		return GL_ALPHA;

	case GL_LUMINANCE:
		return GL_LUMINANCE;

	case GL_LUMINANCE_ALPHA:
		return GL_LUMINANCE_ALPHA;

	default:
		std::cerr << "mismatch in texture internal format\n";
		return GL_RGBA;
	}
}
