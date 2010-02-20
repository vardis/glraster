/*
 * Image.h
 *
 *  Created on: Dec 27, 2009
 *      Author: giorgos
 */

#ifndef IMAGE_H_
#define IMAGE_H_

typedef struct ImageMipmapLevel {
	shared_ptr<void> m_data;
	uint32_t m_compressedSize;

	ImageMipmapLevel() :
		m_data() {
		m_compressedSize = 0;
	}
	ImageMipmapLevel(void* data, uint32_t compressedSize = 0) :
		m_data(data) {
		m_compressedSize = compressedSize;
	}
} ImageMipmapLevel;

class Image;
typedef shared_ptr<Image> ImagePtr;

//TODO !!IMPORTANT!!: add getPitch and getBpp methods
class Image {

protected:
	uint32_t m_width;
	uint32_t m_height;
	GLenum   m_format; // e.g. GL_RGBA, GL_LUMINANCE, GL_ALPHA, etc
	GLenum   m_dataType; // specifies the type of the image's components, e.g. GL_UNSIGNED_BYTE, GL_FLOAT, GL_SHORT, etc
	uint32_t m_pitch;
	std::vector<ImageMipmapLevel> m_mipmaps;

public:
	static ImagePtr load(const String& filename);

	Image();
	Image(uint32_t width, uint32_t height, GLenum channels, void* data, GLenum type, uint32_t pitch);
	virtual ~Image();

	// frees previously allocate resources and allocates new resources based on the new image settings
	void allocate();

	bool saveToFile(const String& filename);

	void setMipmap(uint level, ImageMipmapLevel mipmap) {
		if (m_mipmaps.size() <= level) {
			m_mipmaps.resize(level, ImageMipmapLevel());
		}
		m_mipmaps[level] = mipmap;
	}

	const ImageMipmapLevel& getMipmap(uint level) const {
		if (level > m_mipmaps.size()) {
			SAFE_THROW(GLException(E_BADINDEX));
		}
		return m_mipmaps[level];
	}

	size_t getNumMipmaps() const {
		return m_mipmaps.size();
	}

	bool isCompressed() const;

	bool hasMipmaps() const {
		return m_mipmaps.size() > 1;
	}

	void *getData(uint level = 0) const {
		assert(level < m_mipmaps.size());
		if (level >= m_mipmaps.size()) {
			SAFE_THROW(GLException(E_BADINDEX, "Image mipmap index out of range"));
		}
		return m_mipmaps[level].m_data.get();
	}

	GLenum getDataType() const {
		return m_dataType;
	}

	GLenum getFormat() const {
		return m_format;
	}

	uint32_t getHeight() const {
		return m_height;
	}

	uint32_t getPitch() const {
		return m_pitch;
	}

	uint32_t getWidth() const {
		return m_width;
	}

	void setData(void* data) {
		m_mipmaps[0].m_data.reset(data);
	}

	void setDataType(GLenum dataType) {
		this->m_dataType = dataType;
	}

	void setFormat(GLenum format) {
		this->m_format = format;
	}

	void setHeight(uint32_t height) {
		this->m_height = height;
	}

	void setPitch(uint32_t pitch) {
		this->m_pitch = pitch;
	}

	void setWidth(uint32_t width) {
		this->m_width = width;
	}
};

#endif /* IMAGE_H_ */
