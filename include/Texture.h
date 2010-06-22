/*
 * Texture.h
 *
 *  Created on: Nov 27, 2009
 *      Author: giorgos
 */

#ifndef TEXTURE_H_
#define TEXTURE_H_

/** @brief Specifies the available inputs for texture coordinates */
enum TexMapInput {
	/** tex coords come from uv mapping */
	TexMapInput_UV,
	/** the normal vector is used for tex coords */
	TexMapInput_Normal,
	/** the reflection vector is used for tex coords */
	TexMapInput_Refl,
	/** spherical mapping */
	TexMapInput_Spherical,
	/** cylindrical mapping */
	TexMapInput_Cylindrical,
	/** object space mapping */
	TexMapInput_ObjectSpace,
	/** view space mapping */
	TexMapInput_EyeSpace,
	/** cube mapping */
	TexMapInput_Cube,
	/** flat mapping */
	TexMapInput_Flat
};
typedef enum TexMapInput TexMapInput;

/** @brief Defines which properties of the material are affected by a texture */
enum TexMapTo {
	/** texture affects the diffuse colour */
	TexMapTo_Diffuse,
	/** texture affects the normal vector */
	TexMapTo_Normal,
	/** texture affects the specular colour */
	TexMapTo_CSpecular,
	/** texture affects the amount of shininess */
	TexMapTo_Shininess,
	/** texture affects the amount of ambient colour */
	TexMapTo_Ambient,
	/** texture affects the amount of emission */
	TexMapTo_Emission,
	/** texture affects the alpha value of the colour */
	TexMapTo_Alpha,
	/** texture affects the position of the corresponding fragment */
	TexMapTo_Displacement
};
typedef enum TexMapTo TexMapTo;

/** @brief Defines how the texture coordinates are wrapped at the borders of a texture */
enum TexWrapMode {
	/** texture coords repeat between 0 and 1 */
	TexWrapMode_Repeat,
	/** texture coords are clamped in the range [0, 1] */
	TexWrapMode_Clamp,
	/** texture coords are clamped in the range [1/(2*N), 1 - 1/(2*N)], where N is the dimension of the texture */
	TexWrapMode_ClampEdge
};
typedef enum TexWrapMode TexWrapMode;

/** @brief Defines how textures, that affect the same material properties, combine their results */
enum TexBlendOp {
	/** textures are mixed according to their blend factors */
	TexBlendOp_Mix,
	/** textures are multiplied */
	TexBlendOp_Multiply,
	/** textures are added */
	TexBlendOp_Add,
	/** textures are subtracted */
	TexBlendOp_Sub,
	TexBlendOp_Div,
	TexBlendOp_Screen,
	TexBlendOp_Decal,
	TexBlendOp_Difference,
	TexBlendOp_Darken,
	TexBlendOp_Lighten
};
typedef enum TexBlendOp TexBlendOp;

enum TexFilter {
	TexFilter_Linear,
	TexFilter_Linear_Mipmap_Linear,
	TexFilter_Linear_Mipmap_Bilinear,
	TexFilter_Bilinear,
	TexFilter_Bilinear_Mipmap_Linear,
	TexFilter_Bilinear_Mipmap_Bilinear,
	TexFilter_Anisotropic
};

#include "Colour.h"
#include "Image.h"

class Texture {

public:
	String m_filename;
	bool m_allocated;
	uint16_t m_width;
	uint16_t m_height;
	uint16_t m_sourceWidth, m_sourceHeight;
	GLuint m_texID;
	GLenum m_textureTarget;

	// a bit messy...for normal textures the internal format specifies the number of color components and is
	// of type GLuint. For compressed textures the internal format is of type GLenum and only indicates the
	// compression type.
	GLuint m_internalFormat;
	bool m_useMipmaps;
	bool m_hasAlpha;
	TexFilter m_minFilter;
	TexFilter m_magFilter;
	TexWrapMode m_wrapping;
	float m_anisotropy;

	Colour m_envColour;

public:
	Texture();
	~Texture();

	void fromImage(const Image& img);

	/** Allocates graphics resources */
	bool allocate();

	/** Allocates resources for the specified internal format and data */
	void allocate(GLenum internalFormat, void* data, uint32_t compressedSize = 0);

	/** Binds and configures OGL texture and material state variables according to this texture's settings */
	void configureGLState();

	/** Binds the texture if it has been allocated, otherwise it has no effect */
	void bind();

	void setData(GLenum sourceFormat, GLenum dataType, void* data);
	void setMipmapData(uint8_t level, GLenum sourceFormat, GLenum dataType, void* data);

	void setCompressedData(uint32_t size, void* data);
	void setCompressedMipmapData(uint8_t level, uint32_t size, void* data);

	void updateMipmaps();

	bool isCompressed() const;

	Colour& getEnvColour() {
		return m_envColour;
	}

	void setEnvColour(const Colour& envColour) {
		this->m_envColour = envColour;
	}

	uint32_t getHeight() const {
		return m_height;
	}

	void setHeight(uint32_t m_height) {
		this->m_height = m_height;
	}

	bool getAllocated() const {
		return m_allocated;
	}

	float getAnisotropy() const {
		return m_anisotropy;
	}

	String getFilename() const {
		return m_filename;
	}

	bool getHasAlpha() const {
		return m_hasAlpha;
	}

	TexFilter getMagFilter() const {
		return m_magFilter;
	}

	TexFilter getMinFilter() const {
		return m_minFilter;
	}

	uint32_t getSourceWidth() const {
		return m_sourceWidth;
	}

	GLuint getTexID() const {
		return m_texID;
	}

	GLenum getTextureTarget() const {
		return m_textureTarget;
	}

	bool getUseMipmaps() const {
		return m_useMipmaps;
	}

	uint32_t getWidth() const {
		return m_width;
	}

	TexWrapMode getWrapping() const {
		return m_wrapping;
	}

	void setAllocated(bool allocated) {
		this->m_allocated = allocated;
	}

	void setAnisotropy(float anisotropy) {
		this->m_anisotropy = anisotropy;
	}

	void setFilename(String filename) {
		this->m_filename = filename;
	}

	void setHasAlpha(bool hasAlpha) {
		this->m_hasAlpha = hasAlpha;
	}

	void setInternalFormat(GLuint internalFormat) {
		this->m_internalFormat = internalFormat;
	}

	void setMagFilter(TexFilter magFilter) {
		this->m_magFilter = magFilter;
	}

	void setMinFilter(TexFilter minFilter) {
		this->m_minFilter = minFilter;
	}

	void setSourceWidth(uint32_t sourceWidth) {
		this->m_sourceWidth = sourceWidth;
	}

	void setTexID(GLuint texID) {
		this->m_texID = texID;
	}

	void setTextureTarget(GLenum textureTarget) {
		this->m_textureTarget = textureTarget;
	}

	void setUseMipmaps(bool useMipmaps) {
		this->m_useMipmaps = useMipmaps;
	}

	void setWidth(uint32_t m_width) {
		this->m_width = m_width;
	}

	void setWrapping(TexWrapMode m_wrapping) {
		this->m_wrapping = m_wrapping;
	}

private:
	void _allocateMipmaps();
	GLint _getInternalFormat(GLenum format, GLenum type);
};
typedef struct Texture Texture;
typedef shared_ptr<Texture> TexturePtr;

#endif /* TEXTURE_H_ */
