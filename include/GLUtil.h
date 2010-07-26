/*
 * GLUtil.h
 *
 *  Created on: Jan 21, 2010
 *      Author: giorgos
 */

#ifndef GLUTIL_H_
#define GLUTIL_H_

class GLUtil {
public:
	/**
	 * Returns true if the given format is compressed.
	 */
	static bool isCompressedFormat(GLenum format) {
		return format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || format
				== GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
	}

	/**
	 * Returns true if the given format uses the BGR channel ordering.
	 */
	static bool isBGR(GLenum format) {
		return format == GL_BGRA || format == GL_BGR;
	}

	/**
	 * Indicates if anisotropic filtering is supported by this GL implementation.
	 * @return
	 */
	static bool isAnisotropicFilterSupported() {
		return (GLEW_EXT_texture_filter_anisotropic);
	}

	/**
	 * Returns the maximum anisotropy supported by this GL implementation.
	 * @return
	 */
	static float getMaxAnisotropy() {
		if (GLEW_EXT_texture_filter_anisotropic) {
			float a;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &a);
			return a;
		} else {
			return 1.0f;
		}
	}

	/**
	 * Calculates the internal texture format given the texture format and the data type of each color channel.
	 * @param format the texture format
	 * @param type the data type for representing colors
	 * @return the internal texture format
	 */
	static GLint getInternalFormat(GLenum format, GLenum type) {

		// pass through compressed format
		if (GLUtil::isCompressedFormat(format)) {
			return format;
		}

		//TODO: Add more cases
		switch (format) {
		case GL_DEPTH_COMPONENT:
			if (type == GL_FLOAT) {
				return GL_DEPTH_COMPONENT32F;
			} else {
				return GL_DEPTH_COMPONENT;
			}

		case GL_RGB:
		case GL_BGR:
			if (type == GL_FLOAT) {
				return GL_RGB32F;
			} else if (type == GL_UNSIGNED_INT) {
				return GL_RGB32UI;
			} else if (type == GL_UNSIGNED_INT) {
				return GL_RGB32I;
			} else {
				return GL_RGB;
			}

		case GL_RGBA:
		case GL_BGRA:
			if (type == GL_FLOAT) {
				return GL_RGBA32F;
			} else if (type == GL_UNSIGNED_INT) {
				return GL_RGBA32UI;
			} else if (type == GL_UNSIGNED_INT) {
				return GL_RGBA32I;
			} else {
				return GL_RGBA;
			}

		case GL_ALPHA:
			return GL_ALPHA;

		case GL_LUMINANCE:
			return GL_LUMINANCE;

		case GL_LUMINANCE_ALPHA:
			return GL_LUMINANCE_ALPHA;

		default:
			std::cerr << "mismatch in texture internal format\n";
			return GL_RGBA;
		}
	}

	/**
	 * Sets the default GL state according to the spec.
	 */
	static void setDefaultGLState() {

		glColorMask(true, true, true, true);
		glDepthMask(true);

		glEnable( GL_DEPTH_TEST);
		glDepthFunc( GL_LEQUAL);

		glEnable( GL_CULL_FACE);
		glCullFace( GL_BACK);
		glFrontFace( GL_CCW);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		glClearDepth(1.0f);

		glClearStencil(0);

		glDisable( GL_SCISSOR_TEST);

		// TODO: Set proper values for window_width, window_height at runtime.
		// An idea is to use the observer pattern and have the future window manage notify GLUtil
		// when the dimensions of the window change
		//glScissor(0, 0, window_width, window_height);

		glDisable( GL_STENCIL_TEST);

		// TODO: Set proper value for the stencil mask, which should be:
		// 2^s - 1, where s is the number of bits allocated for the stencil buffer
		// Again check the observer pattern idea above as the window manager can notify GLUtil
		// when the window is created and then GLUtil can cache the number of bits just once
//		glStencilFunc(GL_ALWAYS, 0, rs.getStencilMask());
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

		glDisable( GL_BLEND);
		glBlendFunc(GL_ONE, GL_ZERO);
		glBlendColor(0.0f, 0.0f, 0.0f, 0.0f);
	}
};

#endif /* GLUTIL_H_ */
