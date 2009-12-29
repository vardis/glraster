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
