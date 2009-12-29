/*
 * GLSLShader.cpp
 *
 *  Created on: Dec 16, 2009
 *      Author: giorgos
 */

#include <fstream>
#include <sstream>

#include "GLTutor.h"
#include "GLSLShader.h"

GLSLShader::GLSLShader(const String& shaderSource, GLuint shaderType) :
	m_source(shaderSource), m_shaderType(shaderType), m_shaderID(0), m_isCompiled(false), m_hasErrors(false) {
}

GLSLShader::~GLSLShader() {
	if (m_shaderID && glIsShader(m_shaderID)) {
		glDeleteShader(m_shaderID);
	}
}

GLuint GLSLShader::compile() {
	m_shaderID = glCreateShader(m_shaderType);
	if (m_shaderID) {
		const GLchar* sources[1];
		sources[0] = m_source.c_str();
		glShaderSource(m_shaderID, 1, sources, NULL);

		GLint status;
		glCompileShader(m_shaderID);
		glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &status);
		if (!status) {
			glGetShaderInfoLog(m_shaderID, MAX_LOG_SIZE, NULL, m_compileLog);
			glDeleteShader(m_shaderID);
			m_shaderID = 0;
			m_hasErrors = true;
			return 0;
		}
		m_isCompiled = true;
		m_hasErrors = false;
	}
	return m_shaderID;
}

GLSLShader* GLSLShader::createFromFile(const String& filename, GLuint shaderType) {
	std::ifstream fs(filename.c_str());
	std::stringstream ss;
	while (fs >> ss.rdbuf())
		;
	String source = ss.str();
	std::cout << source;
	if (source.length() > 0) {
		return new GLSLShader(source, shaderType);
	} else {
		return 0;
	}
}
