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

ILTextureManager::~ILTextureManager() {
}

GLuint ILTextureManager::loadTexture(String filename, Texture* tex) {
	Image* img = Image::load(filename);
	if (!img) {
		std::cerr << "Error loading texture " << filename << std::endl;
		return 0;
	}

	GLenum target = img->m_height > 1 ? GL_TEXTURE_2D : GL_TEXTURE_1D;

	//	ILint origin = ilGetInteger(IL_IMAGE_ORIGIN);
	//	if (origin != IL_ORIGIN_LOWER_LEFT) {
	//		iluFlipImage();
	//	}

	if (tex) {
		tex->m_filename = filename;
		tex->fromImage(*img);

		if (tex->m_useMipmaps) {
			gluBuild2DMipmaps(target, img->m_format, img->m_width, img->m_height, img->m_format, img->m_type,
					img->m_data);
		}
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
			glTexImage1D(GL_TEXTURE_1D, 0, img->m_format, img->m_width, 0, img->m_format, img->m_type, img->m_data);
		} else if (target == GL_TEXTURE_2D) {
			glTexImage2D(GL_TEXTURE_2D, 0, img->m_format, img->m_width, img->m_height, 0, img->m_format, img->m_type,
					img->m_data);
		} else {
			//TODO: support 3D textures
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

void ILTextureManager::loadCubeMapTextures(String mapFilename, GLuint* texNames) {
	size_t dotPos = mapFilename.rfind('.');
	if (dotPos >= 0) {
		String basename = mapFilename.substr(0, dotPos);
		String ext = mapFilename.substr(dotPos, mapFilename.length() - dotPos);
		texNames[CubeMap_Face_Front] = loadTexture(basename + "_FR" + ext);
		glBindTexture(GL_TEXTURE_2D, texNames[CubeMap_Face_Front]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		texNames[CubeMap_Face_Back] = loadTexture(basename + "_BK" + ext);
		//		glBindTexture(GL_TEXTURE_2D, texNames[CubeMap_Face_Back]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		texNames[CubeMap_Face_Right] = loadTexture(basename + "_RT" + ext);
		glBindTexture(GL_TEXTURE_2D, texNames[CubeMap_Face_Right]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		texNames[CubeMap_Face_Left] = loadTexture(basename + "_LF" + ext);
		glBindTexture(GL_TEXTURE_2D, texNames[CubeMap_Face_Left]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		texNames[CubeMap_Face_Up] = loadTexture(basename + "_UP" + ext);
		glBindTexture(GL_TEXTURE_2D, texNames[CubeMap_Face_Up]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		texNames[CubeMap_Face_Down] = loadTexture(basename + "_DN" + ext);
		glBindTexture(GL_TEXTURE_2D, texNames[CubeMap_Face_Down]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
}

void ILTextureManager::disposeTexture(GLuint texName) {
	if (texName) {
		glDeleteTextures(1, &texName);
	}
}
