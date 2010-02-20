/*
 * Text.h
 *
 *  Created on: Jan 12, 2010
 *      Author: giorgos
 */

#ifndef TEXT_H_
#define TEXT_H_

#include "Vector.h"
#include "Colour.h"
#include "Renderable.h"
#include "RenderPrimitive.h"
#include "TTFont.h"

class Text : public Renderable {

private:
	TTFontPtr  m_font;
	Vec2<uint> m_pos;
	Colour     m_colour;
	String     m_text;
	RenderPrimitive<TrianglesPrimitiveType> m_geom;
	bool       m_updateGeom;

public:
	explicit Text(TTFontPtr font);
	virtual ~Text();

	TTFontPtr getFont() {
		return m_font;
	}

	void setColour(const Colour& c) {
		m_colour = c;
	}
	const Colour& getColour() const {
		return m_colour;
	}

	void setPos(float x, float y) {
		m_pos.x = x;
		m_pos.y = y;
	}
	const Vec2<uint>& getPos() const {
		return m_pos;
	}

	void setText(const String& text) {
		m_text = text;
		m_updateGeom = true;
	}
	const String& getText() const {
		return m_text;
	}


	virtual void preRender();
	virtual void renderGeometry();
	void render(uint16_t x, uint16_t y, uint8_t* utf8Text, const Colour& colour = Colour::WHITE);
};
typedef shared_ptr<Text> TextPtr;

#endif /* TEXT_H_ */
