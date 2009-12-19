/*
 * ILTextureManager.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: giorgos
 */
#include "GLTutor.h"
#include "ILTextureManager.h"

#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>

ILTextureManager::ILTextureManager() :
	m_anisotropicFilterSupported(false), m_maxAnisotropy(1.0f) {
	if (GLEW_EXT_texture_filter_anisotropic) {
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &m_maxAnisotropy);
	}
}

ILTextureManager::~ILTextureManager() {
}

GLuint ILTextureManager::loadTexture(String filename, Texture* tex) {
	ILuint imgId;
	ilGenImages(1, &imgId);
	ilBindImage(imgId);
	if (!ilLoadImage(filename.c_str())) {
		ILenum error = ilGetError();
		if (error != IL_NO_ERROR) {
			std::cerr << "error in ilLoadImage: " << error << std::endl;
		}
		return 0;
	}

	GLsizei width = ilGetInteger(IL_IMAGE_WIDTH);
	GLsizei height = ilGetInteger(IL_IMAGE_HEIGHT);
	GLenum target = height > 1 ? GL_TEXTURE_2D : GL_TEXTURE_1D;

	GLenum internalFormat = ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL);
	ILenum imgFormat = ilGetInteger(IL_IMAGE_FORMAT);
	GLenum format;
	bool hasAlpha = false;
	switch (imgFormat) {
	case IL_RGBA:
		format = GL_RGBA;
		hasAlpha = true;
		std::cout << "enabling alpha for texture " << filename << "\n";
		break;
	case IL_BGR:
		format = GL_BGR;
		break;
	case IL_BGRA:
		format = GL_BGRA;
		hasAlpha = true;
		break;
	case IL_LUMINANCE:
		format = GL_LUMINANCE;
		break;
	case IL_RGB:
	default:
		format = GL_RGB;
		break;
	}

	ILenum imgType = ilGetInteger(IL_IMAGE_TYPE);
	GLenum type;
	switch (imgType) {
	case IL_BYTE:
		type = GL_BYTE;
		break;
	case IL_UNSIGNED_BYTE:
		type = GL_UNSIGNED_BYTE;
		break;
	case IL_INT:
		type = GL_INT;
		break;
	case IL_FLOAT:
		type = GL_FLOAT;
		break;
	case IL_UNSIGNED_INT:
	default:
		type = GL_UNSIGNED_INT;
		break;
	}

	//	ILint origin = ilGetInteger(IL_IMAGE_ORIGIN);
	//	if (origin != IL_ORIGIN_LOWER_LEFT) {
	//		iluFlipImage();
	//	}

	GLuint texID;
	if (tex && tex->m_texID) {
		texID = tex->m_texID;
	} else {
		glGenTextures(1, &texID);
	}
	glBindTexture(target, texID);

	glPushClientAttrib(GL_CLIENT_PIXEL_STORE_BIT);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SWAP_BYTES, IL_FALSE);
	glTexImage2D(target, 0, internalFormat, width, height, 0, format, type, ilGetData());

	if (tex) {
		tex->m_filename = filename;
		tex->m_texID = texID;
		tex->m_sourceWidth = width;
		tex->m_sourceHeight = height;
		tex->m_width = tex->m_sourceWidth;
		tex->m_height = tex->m_sourceHeight;
		tex->m_textureTarget = target;
		tex->m_internalFormat = internalFormat;
		tex->m_hasAlpha = hasAlpha;

		if (tex->m_useMipmaps) {
			gluBuild2DMipmaps(target, internalFormat, width, height, format, type, ilGetData());
		}
		tex->configureGLState();

		// set anisotropy amount
		if (m_anisotropicFilterSupported) {
			glTexParameterf(target, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(tex->m_anisotropy, m_maxAnisotropy));
		}

	} else {
		// set some defaults
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	glPopClientAttrib();

	ilDeleteImages(1, &imgId);
	return texID;
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
