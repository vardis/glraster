/*
 * ShaderManager.cpp
 *
 *  Created on: Dec 6, 2009
 *      Author: giorgos
 */

#include <fstream>
#include <sstream>

#include "GLTutor.h"
#include "ShaderManager.h"


ShaderManager::ShaderManager() :
	m_glslSupported(false), m_glslVersion(0.0f) {

	m_glslSupported = _checkGLversion();
}

ShaderManager::~ShaderManager() {

}

GLuint ShaderManager::createShaderFromFile(const String& filename, GLenum shaderType) {
	std::ifstream fs(filename.c_str());
	std::stringstream ss;
	while (fs >> ss.rdbuf()) ;
	String source = ss.str();
	if (source.length() > 0) {
		return this->createShader(source, shaderType);
	} else {
		return 0;
	}

}

GLuint ShaderManager::createShader(const String& source, GLenum shaderType) {

	assert(shaderType == GL_VERTEX_SHADER || shaderType == GL_FRAGMENT_SHADER);

	if (!m_glslSupported) {
		return 0;
	}
	GLuint shaderID = glCreateShader(shaderType);
	if (!shaderID)
		return 0;

	const GLchar* sources[1];
	sources[0] = source.c_str();
	glShaderSource(shaderID, 1, sources, NULL);

	GLint status;
	glCompileShader(shaderID);
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
	if (!status) {
		glGetShaderInfoLog(shaderID, MAX_LOG_SIZE, NULL, m_compileLog);
		glDeleteShader(shaderID);
		return 0;
	}
	return shaderID;
}

GLuint ShaderManager::createProgram(GLuint vertexShader, GLuint fragmentShader) {

	assert(vertexShader || fragmentShader);

	if (!m_glslSupported) {
		return 0;
	}

	GLuint progID = glCreateProgram();
	if (!progID) {
		return 0;
	}

	if (vertexShader) {
		glAttachShader(progID, vertexShader);
	}
	if (fragmentShader) {
		glAttachShader(progID, fragmentShader);
	}

	GLint status;
	glLinkProgram(progID);
	glGetProgramiv(progID, GL_LINK_STATUS, &status);
	if (!status) {
		glGetProgramInfoLog(progID, MAX_LOG_SIZE, NULL, m_compileLog);
		glDeleteProgram(progID);
		return 0;
	}

	glValidateProgram(progID);
	glGetProgramiv(progID, GL_VALIDATE_STATUS, &status);
	if (!status) {
		glGetProgramInfoLog(progID, MAX_LOG_SIZE, NULL, m_compileLog);
		glDeleteProgram(progID);
		return 0;
	}

	// flag shaders for removal, they will get removed when the program is deleted
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return progID;
}

bool ShaderManager::_checkGLversion() {
	std::stringstream ss;
	ss << glGetString(GL_VERSION);

	float glver;
	ss >> glver;
	if (glver >= 2.0f) {
		ss.str("");
		ss << glGetString(GL_SHADING_LANGUAGE_VERSION);
		ss >> m_glslVersion;
		return true;
	} else {
		return false;
	}
}
