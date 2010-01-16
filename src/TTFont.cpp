/*
 * TTFont.cpp
 *
 *  Created on: Dec 27, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"

#include "Image.h"
#include "TTFont.h"

bool operator==(const Codepoint& lhs, const Codepoint& rhs) {
	return lhs.m_codepoint == rhs.m_codepoint;
}

TTFont::TTFont() :
	m_filename(""), m_tex(), m_pointSize(12), m_pixelSize(0), m_hres(800), m_vres(600), m_maxWidth(0), m_maxHeight(0),
			m_ranges(), m_codepoints(), m_freetype(), m_ftFace() {

}

TTFont::TTFont(const String& filename, uint pointSize, uint pixelSize, uint hres, uint vres) :
	m_filename(filename), m_pointSize(pointSize), m_pixelSize(pixelSize), m_hres(hres), m_vres(vres), m_maxWidth(0),
			m_maxHeight(0), m_ranges(), m_codepoints() {

}

TTFont::~TTFont() {
	FT_Done_FreeType(m_freetype);
}

bool TTFont::create() {
	if (FT_Init_FreeType(&m_freetype)) {
		std::cerr << "Failed to initialize FreeType\n";
		return false;
	}

	if (FT_New_Face(m_freetype, m_filename.c_str(), 0, &m_ftFace)) {
		std::cerr << "Failed to create new face\n";
		return false;
	}

	FT_F26Dot6 size = m_pointSize << 6;
	if (FT_Set_Char_Size(m_ftFace, size, 0, m_hres, m_vres)) {
		std::cerr << "Failed to set char size\n";
		return false;
	}

	// set ASCII codes if no codepoints have been defined
	if (m_ranges.size() == 0) {
		m_ranges.push_back(CodepointRange(32, 126));
	}

	FT_Pos maxHeight = 0;
	FT_Pos maxWidth = 0;
	FT_Pos maxYBearing = -100;
	int charCount = 0;
	std::list<CodepointRange>::iterator it = m_ranges.begin();
	while (it != m_ranges.end()) {
		for (uint32_t i = (*it).first; i < (*it).second; i++) {
			if (!FT_Load_Char(m_ftFace, i, FT_LOAD_RENDER)) {
				//				maxHeight = std::max(maxHeight, face->glyph->bitmap.rows);
				maxHeight = std::max(maxHeight, (2 * m_ftFace->glyph->bitmap.rows
						- (m_ftFace->glyph->metrics.horiBearingY >> 6)));
				//				maxWidth = std::max(maxWidth, face->glyph->advance.x >> 6);
				maxWidth = std::max(maxWidth, (m_ftFace->glyph->advance.x >> 6)
						+ (m_ftFace->glyph->metrics.horiBearingX >> 6));
				maxYBearing = std::max(maxYBearing, m_ftFace->glyph->metrics.horiBearingY);
				++charCount;
			}
		}
		++it;
	}

	GLuint spacing = 5;
	maxWidth += spacing;
	maxHeight += spacing;

	m_maxWidth = maxWidth;
	m_maxHeight = maxHeight;

	std::cout << "maxWidth: " << maxWidth << " , maxHeight: " << maxHeight << std::endl;
	uint glyphsPerDim = (uint) sqrt(charCount) + 1;

	Image texImg;
	texImg.m_format = GL_RGBA;
	texImg.m_width = getClosestPow2(glyphsPerDim * maxWidth);
	texImg.m_height = getClosestPow2(glyphsPerDim * maxHeight);
	texImg.m_type = GL_UNSIGNED_BYTE;
	texImg.allocate();

	uint glypsX = round(texImg.m_width / maxWidth);
	uint glypsY = round(texImg.m_height / maxHeight);

	// for every codepoint:
	//    -load the corresponding glyph
	//    -render its bitmap into face->glyph->bitmap.buffer
	//    -copy the bitmap at the current position within the texture image
	//    -assign uv coordinates for the current codepoint
	//    -advance the position within the texture image
	int32_t imgPitch = texImg.m_width * 4; // texSize;
	int32_t imgX = 0, imgY = 0;
	uint8_t* img = (uint8_t*) texImg.m_data;
	uint16_t c1 = 0;
	it = m_ranges.begin();
	while (it != m_ranges.end()) {
		for (uint32_t i = (*it).first; i < (*it).second; i++) {
			if (!FT_Load_Char(m_ftFace, i, FT_LOAD_RENDER)) {
				uint8_t* bitmap = m_ftFace->glyph->bitmap.buffer;
				int offset = (maxYBearing >> 6) - (m_ftFace->glyph->metrics.horiBearingY >> 6);
				std::cout << "offsetY = " << offset << std::endl;
				std::cout << "bitmap rows = " << m_ftFace->glyph->bitmap.rows << "\n";
				for (int r = 0; r < m_ftFace->glyph->bitmap.rows; r++) {
					std::cout << "pointer = " << imgY + m_ftFace->glyph->bitmap.rows - r - offset << "\n";
					uint8_t* imgData = &img[(imgY + m_maxHeight - r - offset) * imgPitch + imgX * 4];
					for (int c = 0; c < m_ftFace->glyph->bitmap.width; c++) {
						*imgData++ = *bitmap;
						*imgData++ = *bitmap;
						*imgData++ = *bitmap;
						*imgData++ = *bitmap++;
					}
				}

				Codepoint cp;
				cp.m_codepoint = i;
				cp.m_width = m_ftFace->glyph->advance.x >> 6;
				cp.m_height = m_maxHeight - offset;
				cp.m_u0 = imgX / (float) texImg.m_width;
				cp.m_u1 = cp.m_u0 + (cp.m_width / (float) texImg.m_width);
				cp.m_v0 = imgY / (float) texImg.m_height;
				cp.m_v1 = cp.m_v0 + (cp.m_height / (float) texImg.m_height);
				cp.m_bearingX = m_ftFace->glyph->metrics.horiBearingX >> 6;
				m_codepoints.push_back(cp);
			}

			imgX += spacing + (m_ftFace->glyph->advance.x >> 6) + (m_ftFace->glyph->metrics.horiBearingX >> 6);
			++c1;
			if (c1 % glypsX == 0) {
				imgX = 0;
				imgY += m_maxHeight;
			}
		}
		++it;
	}
	std::cout << c1 << " characters\n";

	std::sort(m_codepoints.begin(), m_codepoints.end());

	texImg.saveToFile("texImg.png");

	m_tex.reset(new Texture());
	m_tex->fromImage(texImg);
	m_tex->m_minFilter = TexFilter_Bilinear;
	m_tex->m_magFilter = TexFilter_Bilinear;
	m_tex->m_useMipmaps = false;
	m_tex->m_wrapping = TexWrapMode_Clamp;

	Material* mat = new Material();
	mat->m_diffuse.set(1.0f);
	mat->m_specular.set(0.0f);
	mat->m_emissive.set(0.0f);
	mat->m_name = m_filename;
	mat->m_opacity = 1.0f;
	mat->m_shininess = 0.0f;
	mat->m_transparent = true;
	mat->m_twoSided = false;
	mat->m_textures.reset(new TextureStack());
	mat->m_textures->textures[0] = m_tex;
	mat->m_textures->texInputs[0].mapping = TexMapInput_UV;
	mat->m_textures->texInputs[0].uvSet = 0;
	mat->m_textures->texInputs[0].texMatrix.identity();
	mat->m_textures->texOutputs[0].mapTo = TexMapTo_Diffuse;
	mat->m_textures->texOutputs[0].blendOp = TexBlendOp_Multiply;
	mat->m_textures->texOutputs[0].blendFactor = 0.5f;
	m_material.reset(mat);

	return true;
}

bool TTFont::getCodepoint(uint32_t codepoint, Codepoint& cp) {
	Codepoint key;
	key.m_codepoint = codepoint;
	std::vector<Codepoint>::iterator it = std::find(m_codepoints.begin(), m_codepoints.end(), key);
	if (it != m_codepoints.end()) {
		cp = *it;
		return true;
	} else {
		return false;
	}
}


int TTFont::getKerning(uint32_t prevCp, uint32_t nextCp) {
	if (FT_HAS_KERNING(m_ftFace)) {
		FT_UInt prevGlyph = FT_Get_Char_Index(m_ftFace, prevCp);
		FT_UInt nextGlyph = FT_Get_Char_Index(m_ftFace, nextCp);
		if (nextGlyph > 0 && prevGlyph > 0) {
			FT_Vector delta;
			if (FT_Get_Kerning(m_ftFace, nextGlyph, prevGlyph, FT_KERNING_DEFAULT, &delta)) {
				std::cout << "ERROR\n";
			}
//			std::cout << "kerning x delta = " << (delta.x >> 6) << "\n";
			return (delta.x >> 6);
		}
	}
	return 0;
}
