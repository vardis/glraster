#ifndef _MATERIAL_H
#define	_MATERIAL_H

#include "Colour.h"
#include "Vector.h"
#include "Matrix4.h"
#include "Texture.h"
#include "GLSLProgram.h"

struct TextureMapInput {
	TextureMapInput() :
		mapping(TexMapInput_UV), uvSet(0), texMatrix() {
		texMatrix.identity();
	}
	TexMapInput mapping;
	ushort uvSet;
	Matrix4f texMatrix;
};

struct TextureMapOutput {
	TextureMapOutput() :
		mapTo(TexMapTo_Diffuse), blendOp(TexBlendOp_Multiply), blendFactor(0.5f) {
	}
	TexMapTo mapTo;
	TexBlendOp blendOp;
	float blendFactor;
};
typedef struct TextureMapOutput TextureMapOutput;

#define MAX_TEXTURES_STACK 8

struct TextureStack {
	TextureStack() :
		textures() {

	}
	TexturePtr textures[MAX_TEXTURES_STACK];
	TextureMapInput texInputs[MAX_TEXTURES_STACK];
	TextureMapOutput texOutputs[MAX_TEXTURES_STACK];
};
typedef struct TextureStack TextureStack;
typedef shared_ptr<TextureStack> TextureStackPtr;

class Material {

public:
	uint16_t m_id;
	String m_name;
	float m_shininess;
	Colour m_diffuse;
	Colour m_specular;
	Colour m_emissive;
	bool m_twoSided;
	bool m_transparent;
	float m_opacity;

	TextureStackPtr m_textures;

	// filenames of shaders
	String m_vertexShader;
	String m_fragmentShader;

	GLSLProgram m_gpuProgram;

public:
	Material();
	~Material();

	void applyGLState();

private:
	void _applyTextureStack();
};
typedef shared_ptr<Material> MaterialPtr;

#endif	/* _MATERIAL_H */

