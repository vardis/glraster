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
	/** texture affects the amount of specularity */
	TexMapTo_Specularity,
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
	TexBlendOp_Overlay,
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

class Texture {

public:
	String m_filename;
	bool   m_allocated;
	uint32_t m_width, m_height;
	uint32_t m_sourceWidth, m_sourceHeight;
	GLuint m_texID;
	GLenum m_textureTarget;
	GLuint m_internalFormat;
	bool m_useMipmaps;
	bool m_hasAlpha;
	TexFilter m_minFilter;
	TexFilter m_magFilter;
	TexWrapMode m_wrapping;
	float m_anisotropy;

public:
	Texture();
	~Texture();

	/** Allocates graphics resources */
	bool allocate();

	void configureGLState();

};
typedef struct Texture Texture;
typedef shared_ptr<Texture> TexturePtr;

#endif /* TEXTURE_H_ */
