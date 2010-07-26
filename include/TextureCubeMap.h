/*
 * TextureCubeMap.h
 *
 *  Created on: Jun 28, 2010
 *      Author: giorgos
 */

#ifndef TEXTURECUBEMAP_H_
#define TEXTURECUBEMAP_H_

#include "Texture.h"

class TextureCubeMap: public Texture {
public:
	TextureCubeMap();
	virtual ~TextureCubeMap();

	void fromImages(const Image& front, const Image& back, const Image& left, const Image& right, const Image& top, const Image& bottom);

	/**
	 * Overridden from Texture::fromImage to be able to handle cubemap images.
	 * If this function is called with an image that doesn't represent a cubemap then all the texture
	 * faces of the cubemap will use the same image.
	 * @param img a cubemap or simple 2D image
	 */
	virtual void fromImage(const Image& img);

	// override the functions below in order to de-activate them
	virtual void setData(GLenum sourceFormat, GLenum dataType, void* data);
	virtual void setMipmapData(uint level, GLenum sourceFormat, GLenum dataType, void* data);

	// overridden to set seam cube map filtering
	virtual void configureGLState();

	/**
	 * Sets the texel data for a specific face of the cubemap at the finest level of detail, i.e. level 0
	 * @param face specifies the cubemap face, e.g. GL_TEXTURE_CUBE_MAP_POSITIVE_X
	 * @param sourceFormat the format of the color data (e.g. GL_RGB)
	 * @param dataType the data type of each color element in the source buffer
	 * @param data a pointer to the actual data
	 */
	void setCubeMapData(GLenum face, GLenum sourceFormat, GLenum dataType, void* data);

	/**
	 * Sets the texel data for a specific face of the cubemap and for a specific mimap level
	 * @param face specifies the cubemap face, e.g. GL_TEXTURE_CUBE_MAP_POSITIVE_X
	 * @param level the mipmap level, starting from 0
	 * @param sourceFormat the format of the color data (e.g. GL_RGB)
	 * @param dataType the data type of each color element in the source buffer
	 * @param data a pointer to the actual data
	 */
	void setCubeMapMipmapData(GLenum face, uint level, GLenum sourceFormat, GLenum dataType, void* data);

};

typedef shared_ptr<TextureCubeMap> TextureCubeMapPtr;

#endif /* TEXTURECUBEMAP_H_ */
