/*
 * GLSLShader.h
 *
 *  Created on: Dec 16, 2009
 *      Author: giorgos
 */

#ifndef GLSLSHADER_H_
#define GLSLSHADER_H_

class GLSLShader {
public:
	static const uint16_t MAX_LOG_SIZE = 1024;

	String m_source;
	GLuint m_shaderType;
	GLuint m_shaderID;
	bool   m_isCompiled;
	bool   m_hasErrors;
	GLchar m_compileLog[MAX_LOG_SIZE];
	//std::map<String, Any> m_inputs;

public:
	GLSLShader(const String& shaderSource, GLuint shaderType);
	virtual ~GLSLShader();

	GLuint compile();
	bool isCompiled() const {
		return m_isCompiled;
	}
	bool hasErrors() const {
		return m_hasErrors;
	}
	const char* getCompilationLog() {
		return &m_compileLog[0];
	}

	static GLSLShader* createFromFile(const String& filename, GLuint shaderType);
};

typedef shared_ptr<GLSLShader> GLSLShaderPtr;

#endif /* GLSLSHADER_H_ */
