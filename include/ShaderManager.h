/*
 * ShaderManager.h
 *
 *  Created on: Dec 6, 2009
 *      Author: giorgos
 */

#ifndef SHADERMANAGER_H_
#define SHADERMANAGER_H_

class ShaderManager {

private:
	static const uint16_t MAX_LOG_SIZE = 2048;
	bool m_glslSupported;
	float m_glslVersion;
	GLchar m_compileLog[MAX_LOG_SIZE];

public:
	ShaderManager();
	virtual ~ShaderManager();

	/**
	 * Compiles a shader of the specified type, from the provided source code.
	 * If compilation was successful, the handle to the shader object is returned.
	 * If compilation failed, a zero value is returned and the error log can be acquired
	 * using getCompilationLog
	 *
	 * @param source
	 * @param shaderType
	 * @return
	 */
	GLuint createShader(const String& source, GLenum shaderType);

	GLuint createShaderFromFile(const String& filename, GLenum shaderType);

	String getCompilationLog() {
		return String(&m_compileLog[0]);
	}

	/**
	 * Creates a shader program that will use the specified vertex and fragment shaders.
	 * If linking and validation of the program succeeds, then the handle to the program
	 * is returned.
	 * Otherwise a zero value is returned and the error log can be acquired using
	 * getProgramErrorLog.
	 * @param vertexShader
	 * @param fragmentShader
	 * @return
	 */
	GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);

	float getGlslVersion() const {
		return m_glslVersion;
	}

private:
	bool _checkGLversion();
};

#endif /* SHADERMANAGER_H_ */
