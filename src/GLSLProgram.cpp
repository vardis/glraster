/*
 * GLSLProgram.cpp
 *
 *  Created on: Dec 16, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "GLSLProgram.h"

GLSLProgram::GLSLProgram() :
	m_progID(0), m_vertexShader(), m_fragmentShader(), m_compiled(false), m_linked(false), m_hasErrors(false) {

}

GLSLProgram::GLSLProgram(GLSLShaderPtr vertexShader, GLSLShaderPtr fragmentShader) :
	m_progID(0), m_vertexShader(vertexShader), m_fragmentShader(fragmentShader), m_compiled(false), m_linked(false),
			m_hasErrors(false) {

}

GLSLProgram::~GLSLProgram() {
	if (m_progID && glIsProgram(m_progID)) {
		glDeleteProgram(m_progID);
	}
}

void GLSLProgram::attachShader(GLSLShaderPtr shader) {
	reset();
	if (shader->m_shaderType == GL_VERTEX_SHADER) {
		m_vertexShader = shader;
	} else {
		m_fragmentShader = shader;
	}
}

void GLSLProgram::attachShader(const String& source, GLuint shaderType) {
	GLSLShaderPtr shader(new GLSLShader(source, shaderType));
	attachShader(shader);
}

void GLSLProgram::attachShaderFromFile(const String& filename, GLuint shaderType) {
	GLSLShaderPtr shader(GLSLShader::createFromFile(filename, shaderType));
	if (shader) {
		return attachShader(shader);
	}
}

bool GLSLProgram::compile() {
	if (!m_compiled) {
		m_hasErrors = true;
		if (m_vertexShader && !m_vertexShader->isCompiled()) {
			if (!m_vertexShader->compile()) {
				return false;
			}
		}

		if (m_fragmentShader && !m_fragmentShader->isCompiled()) {
			if (!m_fragmentShader->compile()) {
				return false;
			}
		}
	}
	m_compiled = true;
	m_hasErrors = false;
	return true;
}

bool GLSLProgram::link() {
	if (!m_progID || !glIsProgram(m_progID)) {
		m_progID = glCreateProgram();
	}

	if (m_vertexShader) {
		glAttachShader(m_progID, m_vertexShader->m_shaderID);
	}
	if (m_fragmentShader) {
		glAttachShader(m_progID, m_fragmentShader->m_shaderID);
	}

	GLint status;
	glLinkProgram(m_progID);
	glGetProgramiv(m_progID, GL_LINK_STATUS, &status);
	if (!status) {
		glGetProgramInfoLog(m_progID, MAX_LOG_SIZE, NULL, m_compileLog);
		glDeleteProgram(m_progID);
		return false;
	}

	glValidateProgram(m_progID);
	glGetProgramiv(m_progID, GL_VALIDATE_STATUS, &status);
	if (!status) {
		glGetProgramInfoLog(m_progID, MAX_LOG_SIZE, NULL, m_compileLog);
		glDeleteProgram(m_progID);
		return false;
	}

	// flag shaders for removal, they will get removed when the program is deleted
	//			glDeleteShader(vertexShader);
	//			glDeleteShader(fragmentShader);
	m_linked = true;
	return true;
}

void GLSLProgram::bind() {
	if (m_compiled && m_linked && !m_hasErrors && (m_vertexShader || m_fragmentShader)) {
		glUseProgram(m_progID);
	}
}

void GLSLProgram::unbind() {
	glUseProgram(0);
}

void GLSLProgram::reset() {
	if (m_vertexShader) {
		glDetachShader(m_progID, m_vertexShader->m_shaderID);
	}
	if (m_fragmentShader) {
		glDetachShader(m_progID, m_fragmentShader->m_shaderID);
	}
	m_compiled = false;
	m_hasErrors = false;
}

GLint GLSLProgram::getAttributeIndex(const char* attributeName) {
	GLint loc = glGetAttribLocation(m_progID, attributeName);
	if (loc == 1 && strncmp("gl_", attributeName, 3)) {
		SAFE_THROW(GLException(E_BADARG, "The specified vertex attribute hasn't been activated"));
	}
	return loc;
}

GLint GLSLProgram::getAttributeIndex(const String& attributeName) {
	return this->getAttributeIndex(attributeName.c_str());
}

template<typename T>
void GLSLProgram::setUniform(const char* uniformName, T value) {
	if (!m_linked) {
		SAFE_THROW(GLException(E_BADSTATE, "GLSL program must be linked before binding uniforms"));
	}
	int loc = glGetUniformLocation(m_progID, uniformName);
	this->setUniform(loc, value);
}

template<typename T>
void GLSLProgram::setUniform(const String& uniformName, T value) {
	if (!m_linked) {
		SAFE_THROW(GLException(E_BADSTATE, "GLSL program must be linked before binding uniforms"));
	}
	this->setUniform(uniformName.c_str(), value);
}

void GLSLProgram::setUniform(int uniformLoc, int value) {
	assert(m_linked);
	glUniform1iv(uniformLoc, 1, &value);
}

void GLSLProgram::setUniform(int uniformLoc, unsigned int value) {
	assert(m_linked);
	glUniform1uiv(uniformLoc, 1, &value);
}

void GLSLProgram::setUniform(int uniformLoc, float value) {
	assert(m_linked);
	glUniform1fv(uniformLoc, 1, &value);
}

void GLSLProgram::setUniform(int uniformLoc, Colour& value) {
	assert(m_linked);
	glUniform4fv(uniformLoc, 1, value.rgba);
}

// Function templates instantiations
template void GLSLProgram::setUniform(const char* uniformName, Colour value);
template void GLSLProgram::setUniform(const char* uniformName, float value);
template void GLSLProgram::setUniform(const char* uniformName, unsigned int value);

