/*
 * TextureManager.h
 *
 *  Created on: Nov 23, 2009
 *      Author: giorgos
 */

#ifndef TEXTUREMANAGER_H_
#define TEXTUREMANAGER_H_

enum CubeMap_Faces {
	CubeMap_Face_Front = 0,
	CubeMap_Face_Back,
	CubeMap_Face_Left,
	CubeMap_Face_Right,
	CubeMap_Face_Top,
	CubeMap_Face_Bottom,
};

#include "Texture.h"

class ITextureManager {

protected:
	bool   m_compress;
	GLenum m_internalFormat;
	bool   m_generateMipmaps;

public:

	void setCompression(bool flag) {
		m_compress = flag;
	}
	bool getCompression() const {
		return m_compress;
	}

	void setInternalFormat(GLenum internalFormat) {
		m_internalFormat = internalFormat;
	}
	GLenum getInternalFormat() const {
		return m_internalFormat;
	}

	void setMipmapGeneration(bool flag) {
		m_generateMipmaps = flag;
	}
	bool getMipmapGeneration() const {
		return m_generateMipmaps;
	}

	virtual GLuint loadTexture(String filename, Texture* tex = 0) = 0;
	virtual void loadCubeMapTextures(String mapFilename, TexturePtr cubeTextures[6]) = 0;
};

#endif /* TEXTUREMANAGER_H_ */
