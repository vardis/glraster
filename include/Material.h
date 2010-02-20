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

struct TextureStack {
	TextureStack() : textures() {
//		for (size_t i = 0; i < MAX_TEXTURES_STACK; i++) {
//			textures[0] = TexturePtr();
//		}
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
	Colour m_ambient;
	Colour m_diffuse;
	Colour m_specular;
	Colour m_emissive;
	bool m_twoSided;
	bool m_transparent;
	float m_opacity;

	TextureStackPtr m_texStack;

private:
	// filenames of shaders
	String m_vertexShader;
	String m_fragmentShader;

	GLSLProgramPtr m_gpuProgram;

public:
	Material();
	~Material();

	/** For compatibility profile */
	void applyGLState();

	/**
	 * Sets up the GLSL shader program for this material using either the provided program
	 * or creating a new program based on the m_vertexShader and m_fragmentShader fields.
	 *
	 * @param prog an existing program for using with this material
	 */
	void setupShaderProgram(GLSLProgramPtr prog);

	/**
	 * Binds the material's attributes to uniform variables of the glsl program.
	 * Note that the material must be linked before calling this.
	 */
	void bindShaderData();

	TextureStackPtr getTexStack() const {
		return m_texStack;
	}

	void setTexStack(TextureStackPtr texStack) {
		this->m_texStack = texStack;
	}

	bool hasCustomShaders() const {
		return m_vertexShader.length() || m_fragmentShader.length();
	}

	String getFragmentShader() const {
		return m_fragmentShader;
	}

	GLSLProgramPtr getGpuProgram() const {
		return m_gpuProgram;
	}

	String getVertexShader() const {
		return m_vertexShader;
	}

	void setFragmentShader(String fragmentShader) {
		this->m_fragmentShader = fragmentShader;
	}

	void setGpuProgram(GLSLProgramPtr gpuProgram) {
		this->m_gpuProgram = gpuProgram;
	}

	void setVertexShader(String vertexShader) {
		this->m_vertexShader = vertexShader;
	}

private:
	void _applyTextureStack();
};
typedef shared_ptr<Material> MaterialPtr;

#endif	/* _MATERIAL_H */

