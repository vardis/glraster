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
	static bool isCompressedFormat(GLenum format) {
		return format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT || format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT || format
				== GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
	}

	static bool isBGR(GLenum format) {
		return format == GL_BGRA || format == GL_BGR;
	}

	static bool isAnisotropicFilterSupported() {
		return (GLEW_EXT_texture_filter_anisotropic);
	}

	static float getMaxAnisotropy() {
		if (GLEW_EXT_texture_filter_anisotropic) {
			float a;
			glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &a);
			return a;
		} else {
			return 1.0f;
		}
	}
};

#endif /* GLUTIL_H_ */
