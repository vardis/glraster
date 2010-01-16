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
}

Text::Text(TTFont* font) :
	Renderable(), m_font(TTFontPtr(font)), m_pos(0, 0), m_colour(), m_text(), m_geom() {
	m_material = font->getMaterial();
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

	VertexFormat vf;
	vf.addElement(Vertex_Pos, VertexFormat_FLOAT3);
	vf.addElement(Vertex_Normal, VertexFormat_FLOAT3);
	vf.addElement(Vertex_TexCoord0, VertexFormat_FLOAT2);
	m_geom.specifyVertexFormat(vf);

	m_geom.beginGeometry(m_text.length());
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

		float pos[3] = { 0, 0, 0 };
		float uv[2];
		float normal[] = { Vec3f::Front.x, Vec3f::Front.y, Vec3f::Front.z };
		Codepoint cp;
		if (m_font->getCodepoint(codepoint, cp)) {

//			std::cout << "printing char " << (char) cp.m_codepoint << std::endl;

			cx += cp.m_bearingX;

			pos[0] = cx;
			pos[1] = cy;
			uv[0] = cp.m_u0;
			uv[1] = cp.m_v0;
//			std::cout << "adding pos " << pos[0] << " , " << pos[1] << "\n";
//			std::cout << "adding uv " << uv[0] << " , " << uv[1] << "\n";
			m_geom.vertexAttrib(Vertex_Pos, pos);
			m_geom.vertexAttrib(Vertex_TexCoord0, uv);
			m_geom.vertexAttrib(Vertex_Normal, normal);

			pos[0] = cx + cp.m_width;
			pos[1] = cy;
			uv[0] = cp.m_u1;
			uv[1] = cp.m_v0;
//			std::cout << "adding pos " << pos[0] << " , " << pos[1] << "\n";
//			std::cout << "adding uv " << uv[0] << " , " << uv[1] << "\n";
			m_geom.vertexAttrib(Vertex_Pos, pos);
			m_geom.vertexAttrib(Vertex_TexCoord0, uv);
			m_geom.vertexAttrib(Vertex_Normal, normal);

			pos[0] = cx + cp.m_width;
			pos[1] = cy + cp.m_height;
			uv[0] = cp.m_u1;
			uv[1] = cp.m_v1;
//			std::cout << "adding pos " << pos[0] << " , " << pos[1] << "\n";
//			std::cout << "adding uv " << uv[0] << " , " << uv[1] << "\n";
			m_geom.vertexAttrib(Vertex_Pos, pos);
			m_geom.vertexAttrib(Vertex_TexCoord0, uv);
			m_geom.vertexAttrib(Vertex_Normal, normal);

			pos[0] = cx;
			pos[1] = cy + cp.m_height;
			uv[0] = cp.m_u0;
			uv[1] = cp.m_v1;
//			std::cout << "adding pos " << pos[0] << " , " << pos[1] << "\n";
//			std::cout << "adding uv " << uv[0] << " , " << uv[1] << "\n";
			m_geom.vertexAttrib(Vertex_Pos, pos);
			m_geom.vertexAttrib(Vertex_TexCoord0, uv);
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

