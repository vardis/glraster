/*
 * TextureCubeMap.cpp
 *
 *  Created on: Jun 28, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "GLUtil.h"
#include "TextureCubeMap.h"

TextureCubeMap::TextureCubeMap() :
	Texture() {
	m_textureTarget = GL_TEXTURE_CUBE_MAP;

	// ensure that the filtering is linear in order to take advantage of the seamless cube map filtering functionality
	m_minFilter = TexFilter_Bilinear;
	m_magFilter = TexFilter_Bilinear;

	// by default don't use mipmaps
	m_useMipmaps = false;
}

TextureCubeMap::~TextureCubeMap() {
	// TODO Auto-generated destructor stub
}

void TextureCubeMap::setData(GLenum sourceFormat, GLenum dataType, void* data) {
	SAFE_THROW(GLException(E_BADOP, "For TextureCubeMap textures, you should use setCubeMapData instead"));
}

void TextureCubeMap::setMipmapData(uint level, GLenum sourceFormat, GLenum dataType, void* data) {
	SAFE_THROW(GLException(E_BADOP, "For TextureCubeMap textures, you should use setCubeMapMipmapData instead"));
}

void TextureCubeMap::setCubeMapData(GLenum face, GLenum sourceFormat, GLenum dataType, void* data) {
	this->setCubeMapMipmapData(face, 0, sourceFormat, dataType, data);
}

void TextureCubeMap::setCubeMapMipmapData(GLenum face, uint level, GLenum sourceFormat, GLenum dataType, void* data) {
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

	glTexSubImage2D(face, level, 0, 0, levelWidth, levelHeight, sourceFormat, dataType, data);

	if (GLUtil::isBGR(sourceFormat)) {
		glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	}
}

void TextureCubeMap::configureGLState() {
	Texture::configureGLState();
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void TextureCubeMap::fromImages(const Image& front, const Image& back, const Image& left, const Image& right,
		const Image& top, const Image& bottom) {

	m_width = front.getWidth();
	m_sourceWidth = front.getWidth();
	m_height = front.getHeight();
	m_sourceHeight = front.getHeight();
	m_internalFormat = GLUtil::getInternalFormat(front.getFormat(), front.getDataType());
	m_hasAlpha = (m_internalFormat == GL_RGBA || m_internalFormat == GL_BGRA);

	if (front.hasMipmaps() || isCompressed()) {
		m_useMipmaps = false;
	} else {
		m_useMipmaps = front.hasMipmaps();
	}

	if (allocate()) {
		glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
		front.setPixelStoreAttributes();
		this->setCubeMapData(GL_TEXTURE_CUBE_MAP_POSITIVE_X, right.getFormat(), right.getDataType(), right.getData());
		this->setCubeMapData(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, left.getFormat(), left.getDataType(), left.getData());
		this->setCubeMapData(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, top.getFormat(), top.getDataType(), top.getData());
		this->setCubeMapData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, bottom.getFormat(), bottom.getDataType(), bottom.getData());
		this->setCubeMapData(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, front.getFormat(), front.getDataType(), front.getData());
		this->setCubeMapData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, back.getFormat(), back.getDataType(), back.getData());
		glPopClientAttrib();

		uint mipMapCount = (front.hasMipmaps() ? front.getNumMipmaps() + 1 : 0);
		for (uint level = 1; level < mipMapCount; ++level) {
			//			if (this->isCompressed()) {
			//				this->setCompressedMipmapData(level, img.getMipmap(level).m_compressedSize, img.getData(level));
			//			} else {

			this->setCubeMapMipmapData(GL_TEXTURE_CUBE_MAP_POSITIVE_X, level, right.getFormat(), right.getDataType(),
					right.getData(level));
			this->setCubeMapMipmapData(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, level, left.getFormat(), left.getDataType(),
					left.getData(level));
			this->setCubeMapMipmapData(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, level, top.getFormat(), top.getDataType(),
					top.getData(level));
			this->setCubeMapMipmapData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, level, bottom.getFormat(), bottom.getDataType(),
					bottom.getData(level));
			this->setCubeMapMipmapData(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, level, front.getFormat(), front.getDataType(),
					front.getData(level));
			this->setCubeMapMipmapData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, level, back.getFormat(), back.getDataType(),
					back.getData(level));
			//			}
		}

		// if the image didn't contain mipmap data, ask GL to create them for us
		if (!front.hasMipmaps() && m_useMipmaps && !isCompressed()) {
			glGenerateMipmap(m_textureTarget);
		}

		this->configureGLState();
	}
}

void TextureCubeMap::fromImage(const Image& img) {
	SAFE_THROW(GLException(E_NOTIMPL));
}
