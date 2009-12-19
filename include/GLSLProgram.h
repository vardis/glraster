/*
 * GLSLProgram.h
 *
 *  Created on: Dec 16, 2009
 *      Author: giorgos
 */

#ifndef GLSLPROGRAM_H_
#define GLSLPROGRAM_H_

#include "GLSLShader.h"

class GLSLProgram {
public:
	static const uint16_t MAX_LOG_SIZE = 2048;

	GLuint m_progID;
	GLSLShaderPtr m_vertexShader;
	GLSLShaderPtr m_fragmentShader;
	GLchar m_compileLog[MAX_LOG_SIZE];
	bool m_compiled;
	bool m_hasErrors;

public:
	GLSLProgram();
	GLSLProgram(GLSLShaderPtr m_vertexShader, GLSLShaderPtr m_fragmentShader);
	virtual ~GLSLProgram();

	GLuint attachShader(GLSLShaderPtr shader);
	GLuint attachShader(const String& source, GLuint shaderType);
	GLuint attachShaderFromFile(const String& filename, GLuint shaderType);

	bool compile();
	bool isCompiled() const {
		return m_compiled;
	}
	bool hasErrors() const {
		return m_hasErrors;
	}

	void bind();
	void unbind();

	void _reset();
};
typedef shared_ptr<GLSLProgram> GLSLProgramPtr;

#endif /* GLSLPROGRAM_H_ */
