/*
 * Material.cpp
 *
 *  Created on: Dec 15, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "ShaderConstants.h"
#include "Material.h"

Material::Material() :
	m_id(0), m_name(""), m_shininess(1.0f), m_ambient(0.1f, 0.1f, 0.1f, 1.0f), m_diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_specular(1.0f, 1.0f, 1.0f, 1.0f), m_emissive(0.0f, 0.0f, 0.0f, 0.0f), m_twoSided(false), m_transparent(
					false), m_opacity(1.0f), m_texStack(TextureStackPtr(new TextureStack())), m_vertexShader(), m_fragmentShader(), m_gpuProgram(new GLSLProgram()) {
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
	if (m_transparent) {
		m_gpuProgram->setUniform(ShaderConstants::UNIFORM_MaterialOpacity, m_opacity);
	} else {
		m_gpuProgram->setUniform(ShaderConstants::UNIFORM_MaterialOpacity, 1.0f);
	}

	uint numTextures = 0;
	for (uint8_t i = 0; i < MAX_TEXTURES_STACK; i++) {
		TexturePtr tex = m_texStack->textures[i];
		if (tex && tex->m_texID && glIsTexture(tex->m_texID)) {
			++numTextures;
			glActiveTexture(GL_TEXTURE0 + i);
			tex->configureGLState();

			m_gpuProgram->setUniform(ShaderConstants::UNIFORM_Samplers[i], tex->m_texID);
			m_gpuProgram->setUniform(ShaderConstants::UNIFORM_TexEnvColors[i], tex->getEnvColour());
		}
	}
	m_gpuProgram->setUniform(ShaderConstants::UNIFORM_NumTextures, numTextures);
}

void Material::setupShaderProgram(GLSLProgramPtr prog) {
	if (prog) {
		m_gpuProgram = prog;
	} else if (!m_gpuProgram && this->hasCustomShaders()) {
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
			}
		}

		if (m_gpuProgram->isCompiled() && !m_gpuProgram->isLinked()) {
			if (!m_gpuProgram->link()) {
				std::cerr << "Failed to link program\n";
			}
		}
	}
}

void Material::applyGLState() {

	// create the shader program if a shader has been specified
	if (!m_gpuProgram->isCompiled() && (m_vertexShader.length() || m_fragmentShader.length())) {
		if (m_vertexShader.length()) {
			m_gpuProgram->attachShaderFromFile(m_vertexShader, GL_VERTEX_SHADER);
		}
		if (m_fragmentShader.length()) {
			m_gpuProgram->attachShaderFromFile(m_fragmentShader, GL_FRAGMENT_SHADER);
		}

		if (m_gpuProgram->compile()) {
			if (!m_gpuProgram->link()) {
				std::cerr << "Failed to link program\n";
			}
		} else {
			std::cerr << "Failed to compiler program\n";
		}
	}

	if (m_gpuProgram->isCompiled() && m_gpuProgram->isLinked() && !m_gpuProgram->hasErrors()) {
		m_gpuProgram->bind();
	} else {
		glUseProgram(0);
	}

	float c[4];
	Colour_to_float4(m_diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	Colour_to_float4(m_specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	Colour_to_float4(m_emissive, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);

	//	if (m_twoSided) {
	//		glDisable(GL_CULL_FACE);
	//	} else {
	//		glEnable(GL_CULL_FACE);
	//	}
	_applyTextureStack();
}

void Material::_applyTextureStack() {
	for (uint8_t i = 0; i < MAX_TEXTURES_STACK; i++) {
		TexturePtr tex = m_texStack->textures[i];
		if (tex && tex->m_texID && glIsTexture(tex->m_texID)) {
			glActiveTexture(GL_TEXTURE0 + i);
			tex->configureGLState();
		}
	}

}
