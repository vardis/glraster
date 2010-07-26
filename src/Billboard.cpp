/*
 * Billboard.cpp
 *
 *  Created on: Dec 3, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "Billboard.h"

BillboardPtr Billboard::create(BillboardType type, float width, float height) {

	BillboardPtr bb = BillboardPtr(new Billboard());

	bb->m_type = type;

	// call meshFactory::createQuad instead of all this circus here...
	bb->m_render = new RenderPrimitive<TrianglesPrimitiveType> ();
	bb->m_render->specifyVertexFormat(VertexFormatPtr(VertexFormat::create(VF_V3_N3_T2)));

	bb->m_vf = bb->m_render->getVertexFormat();

	bb->m_render->setMaterial(bb->m_material);
	bb->m_render->beginGeometry(2);

	bb->m_bounds.addPoint(-width, -height, 0.0f);
	bb->m_bounds.addPoint(width, height, 0.0f);

	float quads_pos[] = {
							// first tri
							bb->m_bounds.max.x, bb->m_bounds.max.y, 0.0f, bb->m_bounds.min.x, bb->m_bounds.max.y, 0.0f, bb->m_bounds.min.x,
							bb->m_bounds.min.y, 0.0f,

							// second tri
							bb->m_bounds.min.x, bb->m_bounds.min.y, 0.0f, bb->m_bounds.max.x, bb->m_bounds.min.y, 0.0f, bb->m_bounds.max.x,
							bb->m_bounds.max.y, 0.0f, };
	bb->m_render->vertexAttribArray(Vertex_Pos, quads_pos);

	float normal[] = { Vec3f::Z_Axis.x, Vec3f::Z_Axis.y, Vec3f::Z_Axis.z };
	for (uint v = 0; v < bb->m_render->getNumVertices(); v++)
		bb->m_render->vertexAttrib(Vertex_Normal, normal);

	float uvs[] = { 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f };

	bb->m_render->vertexAttribArray(Vertex_TexCoord0, uvs);

	if (!bb->m_render->endGeometry()) {
		std::cerr << "error while specifying triangles\n";
		return BillboardPtr();
	}
	return bb;
}

Billboard::Billboard() :	Renderable(), m_upAxis(0.0f, 1.0f, 0.0f) {
	registerListener(this);
}

void Billboard::renderGeometry() {
	m_render->renderGeometry();
}

void Billboard::onPostViewTransform(Renderable* r, Matrix4f& xform) {

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

