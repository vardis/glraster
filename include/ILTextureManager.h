/*
 * ILTextureManager.h
 *
 *  Created on: Nov 23, 2009
 *      Author: giorgos
 */

#ifndef ILTEXTUREMANAGER_H_
#define ILTEXTUREMANAGER_H_

#include "ITextureManager.h"

class ILTextureManager: public ITextureManager {
public:
	ILTextureManager();
	virtual ~ILTextureManager();

	virtual GLuint loadTexture(String filename, Texture* tex = 0);
	virtual void loadCubeMapTextures(String mapFilename, GLuint* texNames);
	virtual void disposeTexture(GLuint texName);

private:
	bool m_anisotropicFilterSupported;
	float m_maxAnisotropy;
};

#endif /* ILTEXTUREMANAGER_H_ */
