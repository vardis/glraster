/*
 * TTFont.h
 *
 *  Created on: Dec 27, 2009
 *      Author: giorgos
 */

#ifndef TTFONT_H_
#define TTFONT_H_

#include "Colour.h"
#include "Material.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

typedef std::pair<uint32_t, uint32_t> CodepointRange;
typedef struct Codepoint {
	uint32_t m_codepoint;
	float m_u0, m_u1;
	float m_v0, m_v1;
	float m_width;
	float m_height;
	int8_t m_bearingX;

	bool operator<(const Codepoint& other) const {
		return m_codepoint < other.m_codepoint;
	}

}Codepoint;

bool operator==(const Codepoint& lhs, const Codepoint& rhs);

/**
 * A TrueType font
 */
class TTFont {
private:
	String      m_filename;
	MaterialPtr m_material;
	TexturePtr  m_tex;
	uint m_pointSize;
	uint m_pixelSize;
	uint m_hres;
	uint m_vres;
	uint m_maxWidth;
	uint m_maxHeight;
	std::list<CodepointRange> m_ranges;
	std::vector<Codepoint> m_codepoints;

	FT_Library m_freetype;
	FT_Face m_ftFace;

public:

	TTFont();
	TTFont(const String& filename, uint pointSize, uint pixelSize, uint hres, uint vres);
	virtual ~TTFont();

	/**
	 * Reports if this font contains a glyph for the given codepoint. Information about the glyph
	 * is filled-out  in the given Codepoint instance.
	 * @param codepoint the unicode code
	 * @param cp a Codepoint instance which receives information about the glyph
	 * @return true if the font contains a glyph for the codepoint or false if otherwise
	 */
	bool getCodepoint(uint32_t codepoint, Codepoint& cp);

	/**
	 * Returns the kerning between the glyphs that correspond to the given codepoints when nextCp follows
	 * prevCp in a text.
	 */
	int getKerning(uint32_t prevCp, uint32_t nextCp);

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

	MaterialPtr getMaterial() {
		return m_material;
	}

	uint getMaxHeight() const {
		return m_maxHeight;
	}

	uint getMaxWidth() const {
			return m_maxWidth;
		}

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
typedef shared_ptr<TTFont> TTFontPtr;

#endif /* TTFONT_H_ */
