/*
 * Image.h
 *
 *  Created on: Dec 27, 2009
 *      Author: giorgos
 */

#ifndef IMAGE_H_
#define IMAGE_H_


class Image {
public:
	uint32_t m_width;
	uint32_t m_height;
	GLuint   m_format; // e.g. GL_RGBA, GL_LUMINANCE, GL_ALPHA, etc
	GLenum   m_type; // specifies the type of the image's components, e.g. GL_UNSIGNED_BYTE, GL_FLOAT, GL_SHORT, etc
	uint32_t m_pitch;
	void* m_data;

public:
	static Image* load(const String& filename);

	Image();
	Image(uint32_t width, uint32_t height, GLuint channels, void* data, GLenum type, uint32_t pitch);
	virtual ~Image();

	// frees previously allocate resources and allocates new resources based on the new image settings
	void allocate();

};

#endif /* IMAGE_H_ */
