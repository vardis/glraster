/*
 * Texture.cpp
 *
 *  Created on: Dec 12, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "GLUtil.h"
#include "Texture.h"

Texture::Texture() :
	m_filename(""), m_allocated(false), m_width(0), m_height(0), m_sourceWidth(0), m_sourceHeight(0), m_texID(0),
			m_textureTarget(GL_TEXTURE_2D), m_internalFormat(GL_RGBA8), m_useMipmaps(false), m_hasAlpha(false),
			m_minFilter(TexFilter_Linear), m_magFilter(TexFilter_Linear), m_wrapping(TexWrapMode_Repeat), m_anisotropy(
					1.0f), m_envColour(Colour::WHITE) {
}

Texture::~Texture() {
	this->_deallocate();
}

void Texture::_deallocate() {
	if (m_texID) {
		glDeleteTextures(1, &m_texID);
		m_texID = 0;
	}
	m_allocated = false;
}

void Texture::setData(GLenum sourceFormat, GLenum dataType, void* data) {
	this->setMipmapData(0, sourceFormat, dataType, data);
}

void Texture::setMipmapData(uint level, GLenum sourceFormat, GLenum dataType, void* data) {
	if (this->isCompressed()) {
		SAFE_THROW(GLException(E_BADOP, "Texture has compressed format, use setCompressedData instead"));
	}

	if (!m_allocated) {
		SAFE_THROW(GLException(E_BADOP, "Texture hasn't been allocated yet"));
	}

	uint levelWidth = m_width >> level;
	uint levelHeight = m_height >> level;

	this->bind();

	if (GLUtil::isBGR(sourceFormat)) {
		glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_TRUE);
	}

	if (m_textureTarget == GL_TEXTURE_1D) {
		glTexSubImage1D(GL_TEXTURE_1D, level, 0, levelWidth, sourceFormat, dataType, data);
	} else if (m_textureTarget == GL_TEXTURE_2D) {
		glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, levelWidth, levelHeight, sourceFormat, dataType, data);
	} else {
		SAFE_THROW(GLException(E_NOTIMPL));
	}

	if (GLUtil::isBGR(sourceFormat)) {
		glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	}
}

void Texture::setCompressedData(uint32_t size, void* data) {
	this->setCompressedMipmapData(0, size, data);
}

void Texture::setCompressedMipmapData(uint level, uint32_t size, void* data) {
	if (!this->isCompressed()) {
		SAFE_THROW(GLException(E_BADOP, "Texture is not compressed, use setData instead"));
	}

	uint levelWidth = m_width >> level;
	uint levelHeight = m_height >> level;

	this->bind();
	if (m_textureTarget == GL_TEXTURE_1D) {
		glCompressedTexImage1D(GL_TEXTURE_1D, level, m_internalFormat, levelWidth, 0, size, data);
	} else if (m_textureTarget == GL_TEXTURE_2D) {
		glCompressedTexImage2D(GL_TEXTURE_2D, level, m_internalFormat, levelWidth, levelHeight, 0, size, data);
	} else {
		SAFE_THROW(GLException(E_NOTIMPL));
	}
}

void Texture::allocate(GLenum internalFormat, void* data, uint32_t compressedSize) {

	if (m_allocated)
		return;

	glGenTextures(1, &m_texID);
	glBindTexture(m_textureTarget, m_texID);

	m_internalFormat = internalFormat;

	// allocate memory storage for the texture
	if (!this->isCompressed()) {
		switch (m_textureTarget) {
		case GL_TEXTURE_1D:
			glTexImage1D(GL_TEXTURE_1D, 0, m_internalFormat, m_width, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			break;
		case GL_TEXTURE_2D:
			glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			break;
		case GL_TEXTURE_CUBE_MAP:
			for (int i = GL_TEXTURE_CUBE_MAP_POSITIVE_X; i <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; i++) {
				glTexImage2D(i, 0, m_internalFormat, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			}
			break;
		default:
			// TODO: support more texture types
			SAFE_THROW(GLException(E_NOTIMPL))
			;
		}

		if (m_useMipmaps && !this->isCompressed()) {
			_allocateMipmaps();
		}
		m_allocated = true;
	}
}

bool Texture::allocate() {
	this->allocate(m_internalFormat, 0);
	return true;
}

void Texture::_allocateMipmaps() {
	uint32_t w = m_width;
	uint32_t h = m_height;
	uint level = 1;
	while (w > 1 || h > 1) {
		if (w > 1)
			w >>= 1;
		if (h > 1)
			h >>= 1;
		switch (m_textureTarget) {
		case GL_TEXTURE_1D:
			glTexImage1D(GL_TEXTURE_1D, level, m_internalFormat, w, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			break;
		case GL_TEXTURE_2D:
			glTexImage2D(GL_TEXTURE_2D, level, m_internalFormat, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			break;
		case GL_TEXTURE_CUBE_MAP:
			for (int i = GL_TEXTURE_CUBE_MAP_POSITIVE_X; i <= GL_TEXTURE_CUBE_MAP_NEGATIVE_Z; i++) {
				glTexImage2D(i, level, m_internalFormat, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			}
			break;
		default:
			// TODO: support more texture types
			SAFE_THROW(GLException(E_NOTIMPL))
			;
		}
		++level;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, level - 1);
}

void Texture::fromImage(const Image& img) {
	m_width = img.getWidth();
	m_sourceWidth = img.getWidth();
	m_height = img.getHeight();
	m_sourceHeight = img.getHeight();
	m_textureTarget = (m_height > 1) ? GL_TEXTURE_2D : GL_TEXTURE_1D;
	m_internalFormat = GLUtil::getInternalFormat(img.getFormat(), img.getDataType());
	m_hasAlpha = (m_internalFormat == GL_RGBA || m_internalFormat == GL_BGRA);

	if (img.hasMipmaps() || isCompressed()) {
		m_useMipmaps = false;
	} else {
		m_useMipmaps = img.hasMipmaps();
	}

	if (allocate()) {
		glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
		img.setPixelStoreAttributes();
		this->setData(img.getFormat(), img.getDataType(), img.getData());
		glPopClientAttrib();

		uint mipMapCount = (img.hasMipmaps() ? img.getNumMipmaps() + 1 : 0);
		for (uint level = 1; level < mipMapCount; ++level) {
			if (this->isCompressed()) {
				this->setCompressedMipmapData(level, img.getMipmap(level).m_compressedSize, img.getData(level));
			} else {
				this->setMipmapData(level, img.getFormat(), img.getDataType(), img.getData(level));
			}
		}

		// if the image didn't contain mipmap data, ask GL to create them for us
		if (!img.hasMipmaps() && m_useMipmaps && !isCompressed()) {
			glGenerateMipmap(m_textureTarget);
		}

		this->configureGLState();
	}
}

void Texture::updateMipmaps() {
	if (m_useMipmaps && !isCompressed()) {
		this->bind();
		glGenerateMipmap(m_textureTarget);
	}
}

void Texture::bind() {
	if (m_texID && glIsTexture(m_texID)) {
		glEnable(m_textureTarget);
		glBindTexture(m_textureTarget, m_texID);
	} else {
		SAFE_THROW(GLException(E_BADOP, "Binding non texture"));
	}
}

void Texture::configureGLState() {

	this->bind();

	// TODO: deprecated
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

	// set anisotropy amount
	if (m_anisotropy > 1.0f && GLUtil::isAnisotropicFilterSupported()) {
		glTexParameterf(m_textureTarget, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(m_anisotropy,
				GLUtil::getMaxAnisotropy()));
	}

}



bool Texture::isCompressed() const {
	return GLUtil::isCompressedFormat(m_internalFormat);
}
