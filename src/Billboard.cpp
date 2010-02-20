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

#ifdef EXPERIMENTAL_CODE
	m_vf.reset(VertexFormat::create(VF_V3_N3_T2));
	m_render = new RenderPrimitive<TrianglesPrimitiveType> ();
	m_render->setMaterial(m_material);
	m_render->specifyVertexFormat(m_vf);
	m_render->beginGeometry(2);

		float quads_pos[] = {
		                     // first tri
		                      m_bounds.max.x, m_bounds.max.y, 0.0f,
		                      m_bounds.min.x, m_bounds.max.y, 0.0f,
		                      m_bounds.min.x, m_bounds.min.y, 0.0f,

		                      // second tri
		                      m_bounds.min.x, m_bounds.min.y, 0.0f,
		                      m_bounds.max.x, m_bounds.min.y, 0.0f,
		                      m_bounds.max.x, m_bounds.max.y, 0.0f,
		};
		m_render->vertexAttribArray(Vertex_Pos, quads_pos);

		float normal[] = { Vec3f::Z_Axis.x, Vec3f::Z_Axis.y, Vec3f::Z_Axis.z };
		for (uint v = 0; v < tris->getNumVertices(); v++)
			m_render->vertexAttrib(Vertex_Normal, normal);

		float uvs[] = { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f };

		m_render->vertexAttribArray(Vertex_TexCoord0, uvs);

		if (!m_render->endGeometry()) {
			std::cerr << "error while specifying triangles\n";
			exit(0);
		}
#endif
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

#ifdef EXPERIMENTAL_CODE

void Billboard::renderGeometry() {
	m_render->renderGeometry();
}

void Billboard::postViewTransform(Matrix4f& xform {
	if (m_type == Billboard_Cylindrical) {
		// reset the X and Z axes rotations by aligning them to the camera's local X and Z axes respectively
		xform.setX(Vec3f::X_Axis);
		xform.setZ(Vec3f::Z_Axis);

	} else if (m_type == Billboard_Spherical) {
		xform.setX(Vec3f::X_Axis);
		xform.setY(Vec3f::Y_Axis);
		xform.setZ(Vec3f::Z_Axis);
	}
}
#endif

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
