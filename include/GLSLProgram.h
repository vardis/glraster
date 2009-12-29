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
	bool m_linked;
	bool m_hasErrors;

public:
	GLSLProgram();
	GLSLProgram(GLSLShaderPtr m_vertexShader, GLSLShaderPtr m_fragmentShader);
	virtual ~GLSLProgram();

	void attachShader(GLSLShaderPtr shader);
	void attachShader(const String& source, GLuint shaderType);
	void attachShaderFromFile(const String& filename, GLuint shaderType);

	GLSLShaderPtr getVertexsShader() {
		return m_vertexShader;
	}
	GLSLShaderPtr getFragmentShader() {
		return m_fragmentShader;
	}

	bool compile();
	bool isCompiled() const {
		return m_compiled;
	}

	bool link();
	bool isLinked() const {
		return m_linked;
	}

	bool hasErrors() const {
		return m_hasErrors;
	}

	void bind();
	void unbind();

	void reset();
};
typedef shared_ptr<GLSLProgram> GLSLProgramPtr;

#endif /* GLSLPROGRAM_H_ */
