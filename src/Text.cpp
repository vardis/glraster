/*
 * Text.cpp
 *
 *  Created on: Jan 12, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "Text.h"

Text::Text(TTFontPtr font) :
	Renderable(), m_font(font), m_pos(0, 0), m_colour(), m_text(), m_geom(), m_updateGeom(false) {
	m_material = font->getMaterial();
	m_geom.specifyVertexFormat(VertexFormatPtr(VertexFormat::create(VF_V3_N3_T2)));
	m_vf = m_geom.getVertexFormat();

	m_state.setDepthTest(false);
	m_state.setShadeless(true);
	m_state.setBlend(true);
	m_state.setBlenSrcFunc(GL_SRC_ALPHA);
	m_state.setBlenDstFunc(GL_ONE_MINUS_SRC_ALPHA);
}

Text::~Text() {

}

void Text::renderGeometry() {
	m_geom.renderGeometry();
}

void Text::preRender() {

	if (!m_updateGeom && m_geom.isSpecified())
		return;

	float cx = m_pos.x, cy = m_pos.y;
	uint32_t codepoint = 0, previousCodepoint = 0;
	uint8_t pending = 0;
	uint8_t b = 0;

	// 2 triangles per glyph
	m_geom.beginGeometry(2 * m_text.length());
	m_geom.setMaterial(m_material);

	const uint8_t* p = reinterpret_cast<const uint8_t*> (m_text.c_str());
	while (*p) {
		do {
			uint8_t c = *p++;

			// c & 11110000
			switch (c & 0xF0) {
			case 0x80:
				// continuation byte of a sequence
				b = c & 0x7F;
				codepoint = (codepoint << 6) | b;
				--pending;
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
			case 0:
			default:
				codepoint = c;
				pending = 0;
				break;
			}
		} while (pending > 0 && *p);

		float ll_pos[3] = { 0, 0, 0 };
		float lr_pos[3] = { 0, 0, 0 };
		float hr_pos[3] = { 0, 0, 0 };
		float hl_pos[3] = { 0, 0, 0 };

		float ll_uv[2];
		float lr_uv[2];
		float hr_uv[2];
		float hl_uv[2];
		float normal[] = { Vec3f::Front.x, Vec3f::Front.y, Vec3f::Front.z };
		Codepoint cp;
		if (m_font->getCodepoint(codepoint, cp)) {

			//			std::cout << "printing char " << (char) cp.m_codepoint << std::endl;

			cx += cp.m_bearingX;

			// low left point
			ll_pos[0] = cx;
			ll_pos[1] = cy;
			ll_uv[0] = cp.m_u0;
			ll_uv[1] = cp.m_v0;

			// low right point
			lr_pos[0] = cx + cp.m_width;
			lr_pos[1] = cy;
			lr_uv[0] = cp.m_u1;
			lr_uv[1] = cp.m_v0;

			// up right point
			hr_pos[0] = cx + cp.m_width;
			hr_pos[1] = cy + cp.m_height;
			hr_uv[0] = cp.m_u1;
			hr_uv[1] = cp.m_v1;

			// up left point
			hl_pos[0] = cx;
			hl_pos[1] = cy + cp.m_height;
			hl_uv[0] = cp.m_u0;
			hl_uv[1] = cp.m_v1;

			// upper triangle
			m_geom.vertexAttrib(Vertex_Pos, ll_pos);
			m_geom.vertexAttrib(Vertex_TexCoord0, ll_uv);
			m_geom.vertexAttrib(Vertex_Normal, normal);
			m_geom.vertexAttrib(Vertex_Pos, hr_pos);
			m_geom.vertexAttrib(Vertex_TexCoord0, hr_uv);
			m_geom.vertexAttrib(Vertex_Normal, normal);
			m_geom.vertexAttrib(Vertex_Pos, hl_pos);
			m_geom.vertexAttrib(Vertex_TexCoord0, hl_uv);
			m_geom.vertexAttrib(Vertex_Normal, normal);

			// lower triangle
			m_geom.vertexAttrib(Vertex_Pos, ll_pos);
			m_geom.vertexAttrib(Vertex_TexCoord0, ll_uv);
			m_geom.vertexAttrib(Vertex_Normal, normal);
			m_geom.vertexAttrib(Vertex_Pos, lr_pos);
			m_geom.vertexAttrib(Vertex_TexCoord0, lr_uv);
			m_geom.vertexAttrib(Vertex_Normal, normal);
			m_geom.vertexAttrib(Vertex_Pos, hr_pos);
			m_geom.vertexAttrib(Vertex_TexCoord0, hr_uv);
			m_geom.vertexAttrib(Vertex_Normal, normal);

			cx += cp.m_width;
			//			std::cout << "char " << (char) codepoint << "\n";
			//			std::cout << "width = " << (int) cp.m_width << "\n";
			//			std::cout << "bearing x = " << (int) cp.m_bearingX << "\n";

			cx += m_font->getKerning(previousCodepoint, cp.m_codepoint);
			previousCodepoint = cp.m_codepoint;

		} else {
			// handle control characters
			if ((char) codepoint == '\n') {
				cy -= m_font->getMaxHeight();
				cx = m_pos.x;
				continue;
			}
		}
	}
	m_geom.endGeometry();
	m_updateGeom = false;
}

