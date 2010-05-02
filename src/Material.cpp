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
	m_id(0), m_name(""), m_shininess(100.0f), m_ambient(0.0f, 0.0f, 0.0f, 0.0f), m_diffuse(1.0f, 1.0f, 1.0f, 1.0f),
			m_specular(1.0f, 1.0f, 1.0f, 1.0f), m_emissive(0.0f, 0.0f, 0.0f, 0.0f), m_twoSided(false), m_transparent(
					false), m_opacity(1.0f), m_shadeless(false), m_texStack(TextureStackPtr(new TextureStack())),
			m_customShaders(false), m_vertexShader(), m_fragmentShader(), m_gpuProgram(new GLSLProgram()) {
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
		float opacity = 1.0f;
		m_gpuProgram->setUniform(ShaderConstants::UNIFORM_MaterialOpacity, opacity);
	}

	uint numTextures = 0;
	for (uint i = 0; i < MAX_TEXTURES_STACK; i++) {
		TexturePtr tex = m_texStack->textures[i];
		if (tex && tex->m_texID && glIsTexture(tex->m_texID)) {
			++numTextures;
//			std::cout << "binding texture " << tex->m_filename << " to unit " << (int)i << "\n";
			glActiveTexture(GL_TEXTURE0 + i);
			glClientActiveTexture(GL_TEXTURE0 + i);
			tex->bind();

			m_gpuProgram->setUniform(ShaderConstants::UNIFORM_Samplers[i], i);
			m_gpuProgram->setUniform(ShaderConstants::UNIFORM_TexEnvColors[i], tex->getEnvColour());
		}
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


