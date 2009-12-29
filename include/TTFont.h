/*
 * TTFont.h
 *
 *  Created on: Dec 27, 2009
 *      Author: giorgos
 */

#ifndef TTFONT_H_
#define TTFONT_H_

#include "Colour.h"
#include "Texture.h"

typedef std::pair<uint32_t, uint32_t> CodepointRange;

/**
 * A TrueType font
 */
class TTFont {
private:
	String     m_filename;
	TexturePtr m_tex;
	uint       m_pointSize;
	uint       m_pixelSize;
	uint       m_hres;
	uint       m_vres;
	std::list<CodepointRange> m_ranges;

public:

	TTFont();
	virtual ~TTFont();

	void addCodepointRange(const CodepointRange& range) {
		m_ranges.push_back(range);
	}
	const std::list<CodepointRange>& getCodepointRanges() const {
		return m_ranges;
	}
	void clearCodepointRanges() {
		m_ranges.clear();
	}

	bool create();

	void render(uint16_t x, uint16_t y, uint8_t* utf8Text, const Colour& colour = Colour::WHITE);

	String getFilename() const {
		return m_filename;
	}

	void setFilename(String m_filename) {
		this->m_filename = m_filename;
	}

	uint getHres() const {
		return m_hres;
	}

	uint getPixelSize() const {
		return m_pixelSize;
	}

	uint getPointSize() const {
		return m_pointSize;
	}

	uint getVres() const {
		return m_vres;
	}


	void setHres(uint m_hres) {
		this->m_hres = m_hres;
	}

	void setPixelSize(uint m_pixelSize) {
		this->m_pixelSize = m_pixelSize;
	}

	void setPointSize(uint m_pointSize) {
		this->m_pointSize = m_pointSize;
	}

	void setVres(uint m_vres) {
		this->m_vres = m_vres;
	}

};

#endif /* TTFONT_H_ */
