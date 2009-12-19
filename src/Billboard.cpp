/*
 * Billboard.cpp
 *
 *  Created on: Dec 3, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "Billboard.h"

Billboard::Billboard(BillboardType type, float width, float height) :
	Renderable(), m_type(type), m_upAxis(0.0f, 1.0f, 0.0f) {
	m_bounds.addPoint(-width, -height, 0.0f);
	m_bounds.addPoint(width, height, 0.0f);
}

void Billboard::renderGeometry() {
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(m_bounds.min.x, m_bounds.min.y);

	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(m_bounds.max.x, m_bounds.min.y);

	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(m_bounds.max.x, m_bounds.max.y);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(m_bounds.min.x, m_bounds.max.y);
	glEnd();
}

void Billboard::postViewTransform(PinholeCameraPtr cam) {
	GLfloat m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);

	if (m_type == Billboard_Cylindrical) {
		// reset the X and Z axes rotations by aligning them to the camera's local X and Z axes respectively
		m[0] = 1.0f;
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[8] = 0.0f;
		m[9] = 0.0f;
		m[10] = 1.0f;
	} else if (m_type == Billboard_Spherical) {
		m[0] = 1.0f;
		m[1] = 0.0f;
		m[2] = 0.0f;
		m[4] = 0.0f;
		m[5] = 1.0f;
		m[6] = 0.0f;
		m[8] = 0.0f;
		m[9] = 0.0f;
		m[10] = 1.0f;
	}
	glLoadMatrixf(m);
}
