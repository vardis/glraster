/*
 * Material.cpp
 *
 *  Created on: Dec 15, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "ShaderConstants.h"
#include "Material.h"

Material::Material(const char* name) :
	m_id(0), m_name(name), m_shininess(100.0f), m_ambient(0.0f, 0.0f, 0.0f, 0.0f), m_diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_specular(1.0f, 1.0f, 1.0f, 1.0f), m_emissive(0.0f, 0.0f, 0.0f, 0.0f), m_twoSided(false), m_transparent(
					false), m_opacity(1.0f), m_shadeless(false), m_customShaders(false), m_vertexShader(),
					m_fragmentShader(), m_gpuProgram(new GLSLProgram()), m_parallaxScale(0.04f), m_parallaxBias(0.02f),
					m_texStack(TextureStackPtr(new TextureStack())) {
}

Material::~Material() {
}

#define Colour_to_float4(c, f) f[0] = c.r; f[1] = c.g; f[2] = c.b; f[3] = c.a;

void Material::bindShaderData() {

	m_gpuProgram->bind();

	m_gpuProgram->setUniform(ShaderConstants::UNIFORM_MaterialAmbient, m_ambient);
	m_gpuProgram->setUniform(ShaderConstants::UNIFORM_MaterialDiffuse, m_diffuse);
	m_gpuProgram->setUniform(ShaderConstants::UNIFORM_MaterialSpecular, m_specular);
	m_gpuProgram->setUniform(ShaderConstants::UNIFORM_MaterialShininess, m_shininess);

	// pack the parallax scale and bias in a single 2d vector
	Vec2f scaleBias(m_parallaxScale, m_parallaxBias);
	m_gpuProgram->setUniform(ShaderConstants::UNIFORM_MaterialParallaxScaleBias, scaleBias);

	if (m_transparent) {
		m_gpuProgram->setUniform(ShaderConstants::UNIFORM_MaterialOpacity, m_opacity);
	} else {
		float opacity = 1.0f;
		m_gpuProgram->setUniform(ShaderConstants::UNIFORM_MaterialOpacity, opacity);
	}

	uint numTextures = this->getActiveTextures();
	for (uint i = 0; i < numTextures; i++) {
		TexturePtr tex = m_texStack->textures[i];

//			std::cout << "binding texture " << tex->m_filename << " to unit " << (int)i << "\n";
		glActiveTexture(GL_TEXTURE0 + i);
		glClientActiveTexture(GL_TEXTURE0 + i);
		tex->bind();

		m_gpuProgram->setUniform(ShaderConstants::UNIFORM_Samplers[i], i);
		m_gpuProgram->setUniform(ShaderConstants::UNIFORM_TexEnvColors[i], tex->getEnvColour());
	}
	m_gpuProgram->setUniform(ShaderConstants::UNIFORM_NumTextures, numTextures);
}

void Material::setupShaderProgram(GLSLProgramPtr prog) {
	if (prog) {
		m_gpuProgram = prog;
	} else {
		if (m_vertexShader.length()) {
			m_gpuProgram->attachShaderFromFile(m_vertexShader, GL_VERTEX_SHADER);
		}
		if (m_fragmentShader.length()) {
			m_gpuProgram->attachShaderFromFile(m_fragmentShader, GL_FRAGMENT_SHADER);
		}
	}

	if (m_gpuProgram) {
		if (!m_gpuProgram->isCompiled()) {
			if (!m_gpuProgram->compile()) {
				std::cerr << "Failed to compile program\n";
			} else {
				std::cout << "Program compiled successfully\n";
			}
		}

		if (m_gpuProgram->isCompiled() && !m_gpuProgram->isLinked()) {
			if (!m_gpuProgram->link()) {
				std::cerr << "Failed to link program\n";
				std::cerr << m_gpuProgram->m_compileLog << std::endl;
			} else {
				std::cout << "Program linked successfully\n";
			}
		}
	}
}

uint8_t Material::getActiveTextures() {
	uint8_t numTextures = 0;
	for (uint i = 0; i < MAX_TEXTURES_STACK; i++) {
			TexturePtr tex = m_texStack->textures[i];
			if (tex && tex->m_texID && glIsTexture(tex->m_texID)) {
				++numTextures;
			}
	}
	return numTextures;
}

void Material::addTexture(TexturePtr tex, TexMapTo mapTo, TexBlendOp blendOp, TexMapInput mapInput, float factor) {

	if (!m_texStack) {
		m_texStack = TextureStackPtr(new TextureStack());
	}
	uint8_t texIndex = this->getActiveTextures();
	if (texIndex == MAX_TEXTURES_STACK - 1) {
		std::cerr << "Cannot add a new texture\n";
		return;
	}
	m_texStack->textures[texIndex] = tex;
	m_texStack->texInputs[texIndex].mapping = mapInput;
	m_texStack->texOutputs[texIndex].mapTo = mapTo;
	m_texStack->texOutputs[texIndex].blendOp = blendOp;
	m_texStack->texOutputs[texIndex].blendFactor = factor;
}
