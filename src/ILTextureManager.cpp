/*
 * ILTextureManager.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: giorgos
 */
#include "GLTutor.h"
#include "Image.h"
#include "ILTextureManager.h"

#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

ILTextureManager::ILTextureManager() :
	m_anisotropicFilterSupported(false), m_maxAnisotropy(1.0f) {
	m_compress = false;
	m_internalFormat = GL_RGBA;
	m_generateMipmaps = true;
	if (GLEW_EXT_texture_filter_anisotropic) {
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_maxAnisotropy);
	}
}

GLuint ILTextureManager::loadTexture(String filename, Texture* tex) {
	ImagePtr img(Image::load(filename));
	if (!img) {
		std::cerr << "Error loading texture " << filename << std::endl;
		return 0;
	}

	GLenum target = img->getHeight() > 1 ? GL_TEXTURE_2D : GL_TEXTURE_1D;

	//	ILint origin = ilGetInteger(IL_IMAGE_ORIGIN);
	//	if (origin != IL_ORIGIN_LOWER_LEFT) {
	//		iluFlipImage();
	//	}

	if (tex) {
		tex->m_filename = filename;
		tex->fromImage(*img);
		tex->configureGLState();

		// set anisotropy amount
		if (m_anisotropicFilterSupported) {
			glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(tex->m_anisotropy, m_maxAnisotropy));
		}
		return tex->m_texID;

	} else {
		GLuint texID;
		glGenTextures(1, &texID);
		glBindTexture(target, texID);

		glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);
		//TODO: Get pixel store parameters from image
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
		glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
		glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
		glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);

		if (target == GL_TEXTURE_1D) {
			glTexImage1D(GL_TEXTURE_1D, 0, img->getFormat(), img->getWidth(), 0, img->getFormat(), img->getDataType(),
					img->getData());
		} else if (target == GL_TEXTURE_2D) {
			glTexImage2D(GL_TEXTURE_2D, 0, img->getFormat(), img->getWidth(), img->getHeight(), 0, img->getFormat(),
					img->getDataType(), img->getData());
		} else {
			//TODO: support 3D textures
		}
		if (!img->isCompressed()) {
			glGenerateMipmap(target);
		}
		// set some defaults
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glPopClientAttrib();
		return texID;
	}
}

void ILTextureManager::loadCubeMapTextures(String mapFilename, TexturePtr cubeTextures[6]) {
	size_t dotPos = mapFilename.rfind('.');
	if (dotPos >= 0) {
		String basename = mapFilename.substr(0, dotPos);
		String ext = mapFilename.substr(dotPos, mapFilename.length() - dotPos);

		TexturePtr frontTex = TexturePtr(new Texture());
		frontTex->setWrapping(TexWrapMode_ClampEdge);
		loadTexture(basename + "_FR" + ext, frontTex.get());
		cubeTextures[CubeMap_Face_Front] = frontTex;

		TexturePtr backTex = TexturePtr(new Texture());
		backTex->setWrapping(TexWrapMode_ClampEdge);
		loadTexture(basename + "_BK" + ext, backTex.get());
		cubeTextures[CubeMap_Face_Back] = backTex;

		TexturePtr rightTex = TexturePtr(new Texture());
		rightTex->setWrapping(TexWrapMode_ClampEdge);
		loadTexture(basename + "_RT" + ext, rightTex.get());
		cubeTextures[CubeMap_Face_Right] = rightTex;

		TexturePtr leftTex = TexturePtr(new Texture());
		leftTex->setWrapping(TexWrapMode_ClampEdge);
		loadTexture(basename + "_LF" + ext, leftTex.get());
		cubeTextures[CubeMap_Face_Left] = leftTex;

		TexturePtr upTex = TexturePtr(new Texture());
		upTex->setWrapping(TexWrapMode_ClampEdge);
		loadTexture(basename + "_UP" + ext, upTex.get());
		cubeTextures[CubeMap_Face_Top] = upTex;

		TexturePtr bottomTex = TexturePtr(new Texture());
		bottomTex->setWrapping(TexWrapMode_ClampEdge);
		loadTexture(basename + "_DN" + ext, bottomTex.get());
		cubeTextures[CubeMap_Face_Bottom] = bottomTex;
	}
}

