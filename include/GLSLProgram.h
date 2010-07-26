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

	bool compile();

	// Linking is applied as a separate operation from compilation in order to allow for transform feedback
	// which is specified after compilation but before linking
	bool link();

	/** Sets the program as the active one for future rendering operations */
	void bind();
	void unbind();

	//-------------------------------------------------------------------------------------

	GLint getAttributeIndex(const char* attributeName);
	GLint getAttributeIndex(const String& attributeName);

	// The family of functions below provide a uniform interface for setting values for a program's uniform
	// variables
	template<typename T>
	void setUniform(const char* uniformName, T& value);

	template<typename T>
	void setUniform(const String& uniformName, T& value);

	/**
	 * Generic uniform setters, every T instance must have a method 'void* ptr()' that returns
	 * a pointer to the actual data that need to be bound at that uniform location.
	 * @param uniformLoc the uniform location index
	 * @param value any instance of a class that provides the above mentioned ptr() function
	 */
	template<typename T>
	void setUniform(int uniformLoc, T& value);

	/** Specialization of setUniform<T> for integer uniforms */
	void setUniform(int uniformLoc, int value);

	/** Specialization of setUniform<T> for unsigned integer uniforms */
	void setUniform(int uniformLoc, unsigned int value);

	/** Specialization of setUniform<T> for matrices */
	void setUniform(int uniformLoc, Matrix4f& mat);
	void setUniform(int uniformLoc, Matrix3f& mat);

	/** Specialization of setUniform<T> for 4 component vectors */
	void setUniform(int uniformLoc, Vec4f& vec);

	/** Specialization of setUniform<T> for 3 component vectors */
	void setUniform(int uniformLoc, Vec3f& vec);

	/** Specialization of setUniform<T> for 2 component vectors */
	void setUniform(int uniformLoc, Vec2f& vec);

	/** Specialization of setUniform<T> for floating point uniforms */
	void setUniform(int uniformLoc, float value);
	//-------------------------------------------------------------------------------------

	/**
	 * Resets the state of this program and detaches any previously attached shaders.
	 */
	void reset();

	bool isCompiled() const {
		return m_compiled;
	}

	bool isLinked() const {
		return m_linked;
	}

	bool hasErrors() const {
		return m_hasErrors;
	}

	GLSLShaderPtr getVertexShader() {
		return m_vertexShader;
	}
	GLSLShaderPtr getFragmentShader() {
		return m_fragmentShader;
	}
};
typedef shared_ptr<GLSLProgram> GLSLProgramPtr;

#endif /* GLSLPROGRAM_H_ */
