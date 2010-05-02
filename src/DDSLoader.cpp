/*
 * DDSLoader.cpp
 *
 *  Created on: Jan 16, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include <fstream>
#include "DDSLoader.h"

// All constants are defined for little-endian format

const uint DDS_MAGIC = *((uint*)"DDS ");

//  DDS_header.dwFlags
#define DDSD_CAPS                   0x00000001
#define DDSD_HEIGHT                 0x00000002
#define DDSD_WIDTH                  0x00000004
#define DDSD_PITCH                  0x00000008
#define DDSD_PIXELFORMAT            0x00001000
#define DDSD_MIPMAPCOUNT            0x00020000
#define DDSD_LINEARSIZE             0x00080000
#define DDSD_DEPTH                  0x00800000

//  DDS_header.sPixelFormat.dwFlags
#define DDPF_ALPHAPIXELS            0x00000001
#define DDPF_FOURCC                 0x00000004
#define DDPF_INDEXED                0x00000020
#define DDPF_RGB                    0x00000040

//  DDS_header.sCaps.dwCaps1
#define DDSCAPS_COMPLEX             0x00000008
#define DDSCAPS_TEXTURE             0x00001000
#define DDSCAPS_MIPMAP              0x00400000

//  DDS_header.sCaps.dwCaps2
#define DDSCAPS2_CUBEMAP            0x00000200
#define DDSCAPS2_CUBEMAP_POSITIVEX  0x00000400
#define DDSCAPS2_CUBEMAP_NEGATIVEX  0x00000800
#define DDSCAPS2_CUBEMAP_POSITIVEY  0x00001000
#define DDSCAPS2_CUBEMAP_NEGATIVEY  0x00002000
#define DDSCAPS2_CUBEMAP_POSITIVEZ  0x00004000
#define DDSCAPS2_CUBEMAP_NEGATIVEZ  0x00008000
#define DDSCAPS2_VOLUME             0x00200000

const uint D3DFMT_DXT1 = *((uint*) "1TXD"); //  DXT1 compression texture format
const uint D3DFMT_DXT2 = *((uint*) "2TXD"); //  DXT2 compression texture format
const uint D3DFMT_DXT3 = *((uint*) "3TXD"); //  DXT3 compression texture format
const uint D3DFMT_DXT4 = *((uint*) "4TXD"); //  DXT4 compression texture format
const uint D3DFMT_DXT5 = *((uint*) "5TXD"); //  DXT5 compression texture format

#define PF_IS_DXT1(pf) \
	((pf.dwFlags & DDPF_FOURCC) && \
	(pf.dwFourCC == D3DFMT_DXT1))

#define PF_IS_DXT3(pf) \
 ((pf.dwFlags & DDPF_FOURCC) && \
 (pf.dwFourCC == D3DFMT_DXT3))

#define PF_IS_DXT5(pf) \
 ((pf.dwFlags & DDPF_FOURCC) && \
 (pf.dwFourCC == D3DFMT_DXT5))

#define PF_IS_BGRA8(pf) \
 ((pf.dwFlags & DDPF_RGB) && \
 (pf.dwFlags & DDPF_ALPHAPIXELS) && \
 (pf.dwRGBBitCount == 32) && \
 (pf.dwRBitMask == 0xff0000) && \
 (pf.dwGBitMask == 0xff00) && \
 (pf.dwBBitMask == 0xff) && \
 (pf.dwABitMask == 0xff000000U))

#define PF_IS_BGR8(pf) \
 ((pf.dwFlags & DDPF_ALPHAPIXELS) && \
 !(pf.dwFlags & DDPF_ALPHAPIXELS) && \
 (pf.dwRGBBitCount == 24) && \
 (pf.dwRBitMask == 0xff0000) && \
 (pf.dwGBitMask == 0xff00) && \
 (pf.dwBBitMask == 0xff))

#define PF_IS_BGR5A1(pf) \
 ((pf.dwFlags & DDPF_RGB) && \
 (pf.dwFlags & DDPF_ALPHAPIXELS) && \
 (pf.dwRGBBitCount == 16) && \
 (pf.dwRBitMask == 0x00007c00) && \
 (pf.dwGBitMask == 0x000003e0) && \
 (pf.dwBBitMask == 0x0000001f) && \
 (pf.dwABitMask == 0x00008000))

#define PF_IS_BGR565(pf) \
 ((pf.dwFlags & DDPF_RGB) && \
 !(pf.dwFlags & DDPF_ALPHAPIXELS) && \
 (pf.dwRGBBitCount == 16) && \
 (pf.dwRBitMask == 0x0000f800) && \
 (pf.dwGBitMask == 0x000007e0) && \
 (pf.dwBBitMask == 0x0000001f))

TexturePtr DDSLoader::loadTexture(const String& filename) {
	std::ifstream fs(filename.c_str(), std::ifstream::in);
	if (!fs) {
		return TexturePtr();
	}

	DDS_HEADER header;
	fs.read(reinterpret_cast<char*> (&header), sizeof(DDS_HEADER));
	if (header.dwMagic != DDS_MAGIC || header.dwSize != 124) {
		// invalid file
		return TexturePtr();
	}
	if (!(header.dwFlags & (DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT))) {
		// invalid does not contain required data
		return TexturePtr();
	}
//	if (!(header.dwFlags & DDSD_MIPMAPCOUNT) && header.dwMipMapCount > 0) {
//		// invalid, must specify mipmap count
//		return TexturePtr();
//	}
	// DDSD_PITCH must be set for uncompressed data
	// DDSD_DEPTH must be set for volume textures
	bool doSwap = false;
	size_t blockBytes, divSize;
	GLenum sourceFormat = GL_BGRA, dataType = GL_UNSIGNED_BYTE;
	TexturePtr tex(new Texture());
	if (PF_IS_DXT1(header.ddpf)) {
		tex->m_internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
		tex->setHasAlpha(true);
		blockBytes = 8;
		divSize = 4;
	} else if (PF_IS_DXT3(header.ddpf)) {
		tex->m_internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
		tex->setHasAlpha(true);
		blockBytes = 16;
		divSize = 4;
	} else if (PF_IS_DXT5( header.ddpf )) {
		tex->m_internalFormat = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
		tex->setHasAlpha(true);
		blockBytes = 16;
		divSize = 4;
	} else if (PF_IS_BGRA8( header.ddpf )) {
		tex->m_internalFormat = GL_RGBA8;
		tex->setHasAlpha(true);
		blockBytes = 4;
		divSize = 1;
		sourceFormat = GL_BGRA;
		dataType = GL_UNSIGNED_BYTE;
	} else if (PF_IS_BGR8( header.ddpf )) {
		tex->m_internalFormat = GL_RGB8;
		tex->setHasAlpha(false);
		blockBytes = 3;
		divSize = 1;
		sourceFormat = GL_BGR;
		dataType = GL_UNSIGNED_BYTE;
	} else if (PF_IS_BGR5A1( header.ddpf )) {
		tex->m_internalFormat = GL_RGB5_A1;
		tex->setHasAlpha(true);
		doSwap = true;
		blockBytes = 2;
		divSize = 1;
		sourceFormat = GL_BGRA;
		dataType = GL_UNSIGNED_SHORT_1_5_5_5_REV;
	} else if (PF_IS_BGR565( header.ddpf )) {
		tex->m_internalFormat = GL_RGB5;
		tex->setHasAlpha(false);
		doSwap = true;
		blockBytes = 2;
		divSize = 1;
		sourceFormat = GL_RGB;
		dataType = GL_UNSIGNED_SHORT_5_6_5;
	} else {
		// unsupported format
		return TexturePtr();
	}

	tex->setWidth(header.dwWidth);
	tex->setHeight(header.dwHeight);
	tex->setTextureTarget(GL_TEXTURE_2D);

	// we will create mipmaps manually
	tex->m_useMipmaps = false;

	tex->allocate();

	size_t mipMapCount = (header.dwFlags & DDSD_MIPMAPCOUNT) ? header.dwMipMapCount : 1;
	tex->m_useMipmaps = mipMapCount > 1;

	size_t x = header.dwWidth;
	size_t y = header.dwHeight;
	if (tex->isCompressed()) {
		// DDSD_LINEARSIZE must be set for compressed data
		if (!(header.dwFlags & DDSD_LINEARSIZE)) {
			// invalid file, field must be set
			return TexturePtr();
		}

		size_t size = (std::max(divSize, x) / divSize) * (std::max(divSize, y) / divSize) * blockBytes;
		assert( size == header.dwLinearSize );

		char* data = new char[size];
		for (unsigned int level = 0; level < mipMapCount; ++level) {
			fs.read(data, size);
			tex->setCompressedMipmapData(level, size, data);
			glCompressedTexImage2D(GL_TEXTURE_2D, level, tex->m_internalFormat, x, y, 0, size, data);
			x = (x + 1) >> 1;
			y = (y + 1) >> 1;
			size = (std::max(divSize, x) / divSize) * (std::max(divSize, y) / divSize) * blockBytes;
		}
		delete[] data;
	} else {
		if (doSwap) {
			glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_TRUE);
		}
		size_t size = x * y * blockBytes;
		char* data = new char[size];

		for (unsigned int level = 0; level < mipMapCount; ++level) {
			fs.read(data, size);
			glPixelStorei(GL_UNPACK_ROW_LENGTH, y);
			tex->setMipmapData(level, sourceFormat, dataType, (void*)data);
			x = (x + 1) >> 1;
			y = (y + 1) >> 1;
			size = x * y * blockBytes;
		}
		delete[] data;
		glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	}
	return tex;
}


ImagePtr DDSLoader::loadImage(const String& filename) {
	std::ifstream fs(filename.c_str(), std::ifstream::in);
	if (!fs) {
		return ImagePtr();
	}

	DDS_HEADER header;
	fs.read(reinterpret_cast<char*> (&header), sizeof(DDS_HEADER));
	if (header.dwMagic != DDS_MAGIC || header.dwSize != 124) {
		// invalid file
		return ImagePtr();
	}
	if (!(header.dwFlags & (DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT))) {
		// invalid does not contain required data
		return ImagePtr();
	}
//	if (!(header.dwFlags & DDSD_MIPMAPCOUNT) && header.dwMipMapCount > 0) {
//		// invalid, must specify mipmap count
//		return TexturePtr();
//	}
	// DDSD_PITCH must be set for uncompressed data
	// DDSD_DEPTH must be set for volume textures
	bool doSwap = false;
	size_t blockBytes, divSize;
	ImagePtr img(new Image());
	if (PF_IS_DXT1(header.ddpf)) {
		img->setFormat(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT);
		blockBytes = 8;
		divSize = 4;
	} else if (PF_IS_DXT3(header.ddpf)) {
		img->setFormat(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT);
		blockBytes = 16;
		divSize = 4;
	} else if (PF_IS_DXT5( header.ddpf )) {
		img->setFormat(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT);
		blockBytes = 16;
		divSize = 4;
	} else if (PF_IS_BGRA8( header.ddpf )) {
		img->setFormat(GL_RGBA8);
		blockBytes = 4;
		divSize = 1;
		img->setFormat(GL_BGRA);
		img->setDataType(GL_UNSIGNED_BYTE);
	} else if (PF_IS_BGR8( header.ddpf )) {
		img->setFormat(GL_RGB8);
		blockBytes = 3;
		divSize = 1;
		img->setFormat(GL_BGR);
		img->setDataType(GL_UNSIGNED_BYTE);
	} else if (PF_IS_BGR5A1( header.ddpf )) {
		img->setFormat(GL_RGB5_A1);
		doSwap = true;
		blockBytes = 2;
		divSize = 1;
		img->setFormat(GL_BGRA);
		img->setDataType(GL_UNSIGNED_SHORT_1_5_5_5_REV);
	} else if (PF_IS_BGR565( header.ddpf )) {
		img->setFormat(GL_RGB5);
		doSwap = true;
		blockBytes = 2;
		divSize = 1;
		img->setFormat(GL_RGB);
		img->setDataType(GL_UNSIGNED_SHORT_5_6_5);
	} else {
		// unsupported format
		return ImagePtr();
	}

	img->setWidth(header.dwWidth);
	img->setHeight(header.dwHeight);

	size_t mipMapCount = (header.dwFlags & DDSD_MIPMAPCOUNT) ? header.dwMipMapCount : 1;
	size_t x = header.dwWidth;
	size_t y = header.dwHeight;
	if (img->isCompressed()) {
		// DDSD_LINEARSIZE must be set for compressed data
		if (!(header.dwFlags & DDSD_LINEARSIZE)) {
			// invalid file, field must be set
			return ImagePtr();
		}

		size_t size = (std::max(divSize, x) / divSize) * (std::max(divSize, y) / divSize) * blockBytes;
		assert( size == header.dwLinearSize );

		for (unsigned int level = 0; level < mipMapCount; ++level) {
			char* data = new char[size];
			fs.read(data, size);
			img->setMipmap(level, ImageMipmapLevel(data, size));
			x = (x + 1) >> 1;
			y = (y + 1) >> 1;
			size = (std::max(divSize, x) / divSize) * (std::max(divSize, y) / divSize) * blockBytes;
		}

	} else {
		size_t size = x * y * blockBytes;
		for (unsigned int level = 0; level < mipMapCount; ++level) {
			char* data = new char[size];
			fs.read(data, size);
			img->setMipmap(level, ImageMipmapLevel(data));
			x = (x + 1) >> 1;
			y = (y + 1) >> 1;
			size = x * y * blockBytes;
		}
	}
	return img;
}
