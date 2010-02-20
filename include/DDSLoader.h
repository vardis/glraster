/*
 * DDSLoader.h
 *
 *  Created on: Jan 16, 2010
 *      Author: giorgos
 */

#ifndef DDSLOADER_H_
#define DDSLOADER_H_

#include "Texture.h"

/*
 Members
 dwSize     Structure size; set to 32 (bytes).
 dwFlags    Values which indicate what type of data is in the surface.
 Flag Description Value
 DDPF_ALPHAPIXELS Texture contains alpha data; dwRGBAlphaBitMask contains valid data. 0x1
 DDPF_ALPHA       Used in some older DDS files for alpha channel only uncompressed data
 (dwRGBBitCount contains the alpha channel bitcount; dwABitMask contains valid data) 0x2
 DDPF_FOURCC      Texture contains compressed RGB data; dwFourCC contains valid data. 0x4
 DDPF_RGB         Texture contains uncompressed RGB data; dwRGBBitCount and the RGB masks (dwRBitMask, dwRBitMask, dwRBitMask) contain valid data. 0x40
 DDPF_YUV         Used in some older DDS files for YUV uncompressed data (dwRGBBitCount contains the YUV bit count;
 dwRBitMask contains the Y mask, dwGBitMask contains the U mask, dwBBitMask contains the V mask) 0x200
 DDPF_LUMINANCE   Used in some older DDS files for single channel color uncompressed data (dwRGBBitCount contains
 the luminance channel bit count; dwRBitMask contains the channel mask). Can be combined with DDPF_ALPHAPIXELS
 for a two channel DDS file. 0x20000

 dwFourCC     Four-character codes for specifying compressed or custom formats. Possible values include:
 DXT1, DXT2, DXT3, DXT4, or DXT5. A FourCC of DX10 indicates the prescense of the DDS_HEADER_DXT10
 extended header, and the dxgiFormat member of that structure indicates the true format.
 When using a four-character code, dwFlags must include DDPF_FOURCC.
 dwRGBBitCount  Number of bits in an RGB (possibly including alpha) format. Valid when dwFlags includes DDPF_RGB, DDPF_LUMINANCE, or DDPF_YUV.
 dwRBitMask        Red (or lumiannce or Y) mask for reading color data. For instance, given the A8R8G8B8 format, the red mask would be 0x00ff0000.
 dwGBitMask        Green (or U) mask for reading color data. For instance, given the A8R8G8B8 format, the green mask would be 0x0000ff00.
 dwBBitMask        Blue (or V) mask for reading color data. For instance, given the A8R8G8B8 format, the blue mask would be 0x000000ff.
 dwABitMask        Alpha mask for reading alpha data. dwFlags must include DDPF_ALPHAPIXELS or DDPF_ALPHA.
 For instance, given the A8R8G8B8 format, the alpha mask would be 0xff000000.

 */
typedef struct {
	uint32_t dwSize;
	uint32_t dwFlags;
	uint32_t dwFourCC;
	uint32_t dwRGBBitCount;
	uint32_t dwRBitMask;
	uint32_t dwGBitMask;
	uint32_t dwBBitMask;
	uint32_t dwABitMask;
} DDS_PIXELFORMAT;

/*
 Members
 dwSize              Size of structure. This member must be set to 124.
 dwFlags             Flags to indicate which members contain valid data.
 Flag Description Value
 DDSD_CAPS Required in every .dds file. 0x1
 DDSD_HEIGHT Required in every .dds file. 0x2
 DDSD_WIDTH Required in every .dds file. 0x4
 DDSD_PITCH Required when pitch is provided for an uncompressed texture. 0x8
 DDSD_PIXELFORMAT Required in every .dds file. 0x1000
 DDSD_MIPMAPCOUNT Required in a mipmapped texture. 0x20000
 DDSD_LINEARSIZE Required when pitch is provided for a compressed texture. 0x80000
 DDSD_DEPTH Required in a depth texture. 0x800000

 dwHeight            Surface height (in pixels).
 dwWidth             Surface width (in pixels).
 dwLinearSize        The number of bytes per scan line in an uncompressed texture; the total number of bytes
 in the top level texture for a compressed texture. The pitch must be DWORD aligned.
 dwDepth             Depth of a volume texture (in pixels), otherwise unused.
 dwMipMapCount        Number of mipmap levels, otherwise unused.
 dwReserved1[11]     Unused.
 ddpf                 The pixel format (see DDS_PIXELFORMAT).
 dwCaps                 Specifies the complexity of the surfaces stored.
 Flag Description Value
 DDSCAPS_COMPLEX Optional; must be used on any file that contains more than one surface (a mipmap, a cubic environment map, or volume texture). 0x8
 DDSCAPS_MIPMAP Optional; should be used for a mipmap. 0x400000
 DDSCAPS_TEXTURE Required 0x1000
 dwCaps2             Additional detail about the surfaces stored.
 Flag Description Value
 DDSCAPS2_CUBEMAP Required for a cube map. 0x200
 DDSCAPS2_CUBEMAP_POSITIVEX Required when these surfaces are stored in a cube map. 0x400
 DDSCAPS2_CUBEMAP_NEGATIVEX Required when these surfaces are stored in a cube map. 0x800
 DDSCAPS2_CUBEMAP_POSITIVEY Required when these surfaces are stored in a cube map. 0x1000
 DDSCAPS2_CUBEMAP_NEGATIVEY Required when these surfaces are stored in a cube map. 0x2000
 DDSCAPS2_CUBEMAP_POSITIVEZ Required when these surfaces are stored in a cube map. 0x4000
 DDSCAPS2_CUBEMAP_NEGATIVEZ Required when these surfaces are stored in a cube map. 0x8000
 DDSCAPS2_VOLUME Required for a volume texture. 0x200000

 dwCaps3
 dwCaps4
 dwReserved2         Unused.
 */
typedef struct {
	uint32_t dwMagic;
	uint32_t dwSize;
	uint32_t dwFlags;
	uint32_t dwHeight;
	uint32_t dwWidth;
	uint32_t dwLinearSize;
	uint32_t dwDepth;
	uint32_t dwMipMapCount;
	uint32_t dwReserved1[11];
	DDS_PIXELFORMAT ddpf;
	uint32_t dwCaps;
	uint32_t dwCaps2;
	uint32_t dwCaps3;
	uint32_t dwCaps4;
	uint32_t dwReserved2;
} DDS_HEADER;



/**
 * A loader for DDS images. Most of this code is based on the implementation provided
 * by Jon Watte to the public domain (http://www.mindcontrol.org/~hplus/graphics/dds-info/).
 *
 *  DDS file structure:
 *      uint32_t   magic  , 0x20534444 ("DDS ")
 *      DDS_HEADER header , size 124 bytes
 *      byte[]     data
 *      byte[]     mipmap, cubemap faces, tex3D layers data
 *
 * A valid DDS file is at least 128 bytes to accomodate for the magic and a basic header.
 * For cubemap textures the faces are written in the order: positive x, negative x, positive y,
 * negative y, positive z, negative z, with any missing faces omitted. Each face is written with
 * its main image, followed by any mipmap levels.
 */
class DDSLoader {

public:
	TexturePtr loadTexture(const String& filename);
	ImagePtr loadImage(const String& filename);
};

#endif /* DDSLOADER_H_ */
