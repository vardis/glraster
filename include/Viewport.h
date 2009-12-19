/*
 * Viewport.h
 *
 *  Created on: Dec 12, 2009
 *      Author: giorgos
 */

#ifndef VIEWPORT_H_
#define VIEWPORT_H_

typedef struct Viewport {
	Viewport() : m_width(1024), m_height(768), m_x(0), m_y(0), m_zorder(0), m_clearColor(0.0f, 0.0f, 0.0f, 1.0f) {

	}
	uint16_t m_width;
	uint16_t m_height;
	uint16_t m_x;
	uint16_t m_y;
	uint16_t m_zorder;
	Colour   m_clearColor;
}Viewport;
typedef shared_ptr<Viewport> ViewportPtr;

#endif /* VIEWPORT_H_ */
