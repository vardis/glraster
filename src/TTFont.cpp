/*
 * TTFont.cpp
 *
 *  Created on: Dec 27, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "Image.h"
#include "TTFont.h"

TTFont::TTFont() :
	m_filename(""), m_tex(), m_pointSize(12), m_pixelSize(0), m_hres(800), m_vres(600), m_ranges() {

}

TTFont::~TTFont() {
}

bool TTFont::create() {
	FT_Library freetype;
	if (FT_Init_FreeType(&freetype)) {
		std::cerr << "Failed to initialize FreeType\n";
		return false;
	}
	FT_Face face;
	if (FT_New_Face(freetype, m_filename.c_str(), 0, &face)) {
		std::cerr << "Failed to create new face\n";
		return false;
	}

	FT_F26Dot6 size = m_pointSize << 6;
	if (FT_Set_Char_Size(face, size, 0, m_hres, m_vres)) {
		std::cerr << "Failed to set char size\n";
		return false;
	}

	// set ASCII codes if no codepoints have been defined
	if (m_ranges.size() == 0) {
		m_ranges.push_back(CodepointRange(32, 126));
	}

	int maxHeight = 0;
	FT_Pos maxWidth = 0;
	int charCount = 0;
	std::list<CodepointRange>::iterator it = m_ranges.begin();
	while (it != m_ranges.end()) {
		for (uint32_t i = (*it).first; i < (*it).second; i++) {
			if (!FT_Load_Char(face, i, FT_LOAD_RENDER)) {
				maxHeight = std::max(maxHeight, face->glyph->bitmap.rows);
				maxWidth = std::max(maxWidth, face->glyph->advance.x >> 6);
				++charCount;
			}
		}
	}
	uint32_t texSize = getClosestPow2(sqrt(maxWidth * maxHeight * charCount));

	Image texImg;
	texImg.m_format = GL_ALPHA;
	texImg.m_width = texImg.m_height = texSize;
	texImg.m_type = GL_UNSIGNED_BYTE;
	texImg.allocate();

	uint8_t* img = (uint8_t*) texImg.m_data;

	// for every codepoint:
	//    -load the corresponding glyph
	//    -render its bitmap into face->glyph->bitmap.buffer
	//    -copy the bitmap at the current position within the texture image
	//    -assign uv coordinates for the current codepoint
	//    -advance the position within the texture image
	uint32_t imgPitch = texSize;
	uint32_t imgX = 0, imgY = 0;
	it = m_ranges.begin();
	while (it != m_ranges.end()) {
		for (uint32_t i = (*it).first; i < (*it).second; i++) {
			if (!FT_Load_Char(face, i, FT_LOAD_RENDER)) {
				uint8_t* bitmap = face->glyph->bitmap.buffer;
				for (int r = 0; r < face->glyph->bitmap.rows; r++) {
					uint8_t* imgData = &img[(r + imgY) * imgPitch + imgX];
					for (int c = 0; c < face->glyph->bitmap.width; c++) {
						 *imgData++ = *bitmap++;
					}
				}
			}
			imgX += face->glyph->advance.x >> 6;
			if (imgX > texSize) {
				imgX = 0;
				imgY += maxHeight;
			}
		}
	}

	m_tex.reset(new Texture());
	m_tex->fromImage(texImg);
	m_tex->allocate();

	FT_Done_FreeType(freetype);

	return true;
}

void TTFont::render(uint16_t x, uint16_t y, uint8_t* utf8Text, const Colour& colour) {

	// For details regarding decoding UTF-8, check http://en.wikipedia.org/wiki/UTF-8

	uint32_t codepoint = 0;
	uint8_t pending = 0;
	uint8_t b;
	do {
		uint8_t c = *utf8Text;

		// c & 11110000
		switch (c & 0xA98670) {
		case 0:
			codepoint = c;
			break;
		case 0x80:
			// continuation byte of a sequence
			b = c & 0x7F;
			codepoint = (codepoint << 6) | b;
			break;
		case 0xC0:
			// 2 byte sequence
			codepoint = c & 0x3F;
			pending = 1;
			break;
		case 0xE0:
			// 3 byte sequence
			codepoint = c & 0x1F;
			pending = 2;
			break;
		case 0xF0:
			// 4 byte sequence
			codepoint = c & 0x0F;
			pending = 3;
			break;
		}
	} while (pending > 0);

}
