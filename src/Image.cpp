/*
 * Image.cpp
 *
 *  Created on: Dec 27, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"

#define ILUT_USE_OPENGL
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <IL/devil_cpp_wrapper.hpp>

#include "GLUtil.h"
#include "Image.h"
#include "DDSLoader.h"

static ILenum _getILTypeFromGLType(GLenum type) {
	ILenum ilType;
	switch (type) {
	case GL_BYTE:
		ilType = IL_BYTE;
		break;
	case GL_UNSIGNED_BYTE:
		ilType = IL_UNSIGNED_BYTE;
		break;
	case GL_INT:
		ilType = IL_INT;
		break;
	case GL_UNSIGNED_INT:
		ilType = IL_UNSIGNED_INT;
		break;
	case GL_SHORT:
		ilType = IL_SHORT;
		break;
	case GL_UNSIGNED_SHORT:
		ilType = IL_UNSIGNED_SHORT;
		break;
	case GL_FLOAT:
		ilType = IL_FLOAT;
		break;
	case GL_DOUBLE:
		ilType = IL_DOUBLE;
		break;
	case GL_HALF_FLOAT:
		ilType = IL_HALF;
		break;
	default:
		ilType = IL_UNSIGNED_BYTE;
	}
	return ilType;
}

static ILenum _getILFormatFromGLFormat(GLenum format) {
	ILenum ilFmt;
	switch (format) {
	case GL_RGBA:
		ilFmt = IL_RGBA;
		break;
	case GL_BGRA:
		ilFmt = IL_BGRA;
		break;
	case GL_LUMINANCE:
		ilFmt = IL_LUMINANCE;
		break;
	case GL_LUMINANCE_ALPHA:
		ilFmt = IL_LUMINANCE_ALPHA;
		break;
	case GL_ALPHA:
		ilFmt = IL_ALPHA;
		break;
	case GL_BGR:
		ilFmt = IL_BGR;
		break;
	case GL_RGB:
	default:
		ilFmt = IL_RGB;
		break;
	}
	return ilFmt;
}

Image::Image() :
	m_width(0), m_height(0), m_format(GL_RGBA), m_dataType(GL_UNSIGNED_BYTE), m_pitch(0), m_mipmaps() {
	m_mipmaps.push_back(ImageMipmapLevel());
}

Image::Image(uint32_t width, uint32_t height, GLenum format, void* data, GLenum type, uint32_t pitch) :
	m_width(width), m_height(height), m_format(format), m_dataType(type), m_pitch(pitch), m_mipmaps() {

	allocate();
	m_mipmaps.push_back(ImageMipmapLevel(data));
}

Image::~Image() {
//	if (m_data) {
//		delete[] m_data;
//	}
}

ImagePtr Image::load(const String& filename) {
	if (filename.find(".dds") != String::npos) {
		std::cout << "DDS loader in action";
		DDSLoader dds;
		return dds.loadImage(filename);
	}
	ilImage il(filename.c_str());
	if (il.Width() <= 0) {
		ILenum error = ilGetError();
		if (error != IL_NO_ERROR) {
			std::cerr << "error in ilLoadImage: " << error << std::endl;
		}
		return ImagePtr();
	}

	ImagePtr img(new Image());
	img->m_width = il.Width();
	img->m_height = il.Height();

	ILenum imgFormat = il.Format();
	size_t numComponents = 0;
	switch (imgFormat) {
	case IL_RGBA:
		img->m_format = GL_RGBA;
		numComponents = 4;
		break;
	case IL_BGRA:
		img->m_format = GL_BGRA;
		numComponents = 4;
		break;
	case IL_LUMINANCE:
		img->m_format = GL_LUMINANCE;
		numComponents = 1;
		break;
	case IL_BGR:
		img->m_format = GL_BGR;
		numComponents = 3;
		break;
	case IL_RGB:
	default:
		img->m_format = GL_RGB;
		numComponents = 3;
		break;
	}

	ILenum imgType = il.Type();
	switch (imgType) {
	case IL_BYTE:
		img->m_dataType = GL_BYTE;
		break;
	case IL_UNSIGNED_BYTE:
		img->m_dataType = GL_UNSIGNED_BYTE;
		break;
	case IL_INT:
		img->m_dataType = GL_INT;
		break;
	case IL_UNSIGNED_INT:
		img->m_dataType = GL_UNSIGNED_INT;
		break;
	case IL_SHORT:
		img->m_dataType = GL_SHORT;
		break;
	case IL_UNSIGNED_SHORT:
		img->m_dataType = GL_UNSIGNED_SHORT;
		break;
	case IL_FLOAT:
		img->m_dataType = GL_FLOAT;
		break;
	case IL_DOUBLE:
		img->m_dataType = GL_DOUBLE;
		break;
	case IL_HALF:
		img->m_dataType = GL_HALF_FLOAT;
		break;
	default:
		return ImagePtr();
	}

	img->allocate();
	memcpy(img->m_mipmaps[0].m_data.get(), il.GetData(), img->m_width * img->m_height * il.Bpp());

	il.Close();

	return img;
}

void Image::allocate() {
	if (!m_mipmaps[0].m_data) {
		size_t numComponents = 0;
		switch (m_format) {
		case GL_RGB:
		case GL_BGR:
			numComponents = 3;
			break;
		case GL_DEPTH_STENCIL:
		case GL_LUMINANCE_ALPHA:
			numComponents = 2;
			break;
		case GL_DEPTH:
		case GL_ALPHA:
		case GL_LUMINANCE:
		case GL_RED:
		case GL_GREEN:
		case GL_BLUE:
			numComponents = 1;
			break;
		case GL_RGBA:
		case GL_BGRA:
		default:
			numComponents = 4;
		}

		switch (m_dataType) {
		case GL_BYTE:
			m_pitch = numComponents * m_width;
			m_mipmaps[0].m_data.reset(new int8_t[m_pitch * m_height]);
			break;
		case GL_UNSIGNED_BYTE:
			m_pitch = numComponents * m_width;
			m_mipmaps[0].m_data.reset(new uint8_t[m_pitch * m_height]);
			break;
		case GL_INT:
			m_pitch = numComponents * m_width * 4;
			m_mipmaps[0].m_data.reset(new int32_t[m_pitch * m_height]);
			break;
		case GL_UNSIGNED_INT:
			m_pitch = numComponents * m_width * 4;
			m_mipmaps[0].m_data.reset(new uint32_t[m_pitch * m_height]);
			break;
		case GL_SHORT:
			m_pitch = numComponents * m_width * 2;
			m_mipmaps[0].m_data.reset(new int16_t[m_pitch * m_height]);
			break;
		case GL_UNSIGNED_SHORT:
			m_pitch = numComponents * m_width * 2;
			m_mipmaps[0].m_data.reset(new uint16_t[m_pitch * m_height]);
			break;
		case GL_FLOAT:
			m_pitch = numComponents * m_width * sizeof(float);
			m_mipmaps[0].m_data.reset(new uint32_t[m_pitch * m_height]);
			break;
		case GL_DOUBLE:
			m_pitch = numComponents * m_width * sizeof(double);
			m_mipmaps[0].m_data.reset(new uint32_t[m_pitch * m_height]);
			break;
		case GL_HALF_FLOAT:
			m_pitch = numComponents * m_width * sizeof(GLhalf);
			m_mipmaps[0].m_data.reset(new GLhalf[m_pitch * m_height]);
			break;

		default:
			//TODO: error checking
			assert(0);
		}
		memset(m_mipmaps[0].m_data.get(), 0, m_pitch * m_height);
	}
}

bool Image::saveToFile(const String& filename) {
	ilImage il;
	uint32_t bpp = m_pitch / m_width;
	il.TexImage(m_width, m_height, 1, bpp, _getILFormatFromGLFormat(m_format), _getILTypeFromGLType(m_dataType), m_mipmaps[0].m_data.get());
	return il.Save(filename.data());
}

bool Image::isCompressed() const {
	return GLUtil::isCompressedFormat(m_format);
}

void Image::setPixelStoreAttributes() const {
	//TODO: Get pixel store parameters from image
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
}
