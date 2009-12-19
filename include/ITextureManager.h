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
	CubeMap_Face_Up,
	CubeMap_Face_Down,
};

#include "Texture.h"

class ITextureManager {
public:
	virtual GLuint loadTexture(String filename, Texture* tex = 0) = 0;
	virtual void loadCubeMapTextures(String mapFilename, GLuint* texNames) = 0;
	virtual void disposeTexture(GLuint texName) = 0;
};

#endif /* TEXTUREMANAGER_H_ */
