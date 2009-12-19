/*
 * RenderablesRasterizer.cpp
 *
 *  Created on: Nov 25, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "Material.h"
#include "MeshModel.h"
#include "RenderablesRasterizer.h"

#define Colour_to_float4(c, f) f[0] = c.r; f[1] = c.g; f[2] = c.b; f[3] = c.a;

RenderablesRasterizer::RenderablesRasterizer(ITextureManager* texMgr, MaterialDB* matDB) :
	m_texMgr(texMgr), m_matDB(matDB), m_shaders() {

}

RenderablesRasterizer::~RenderablesRasterizer() {
}


void RenderablesRasterizer::beginFrame(PinholeCameraPtr cam) {
	for (uint8_t i = 0; i < MAX_RENDER_LAYERS; i++) {
		RenderablesList sorted = m_layers[i].depthSort();
		RenderablesList::iterator it = sorted.begin();
		while (it != sorted.end()) {
			Renderable* r = *it;
			this->render(r, cam);
			++it;
		}
	}
}

void RenderablesRasterizer::endFrame() {
	for (uint8_t i = 0; i < MAX_RENDER_LAYERS; i++) {
		m_layers[i].clear();
	}
}

void RenderablesRasterizer::render(Renderable* r, PinholeCameraPtr cam, uint16_t materialOverride) {

	// setup viewing state
	const Matrix4f& tr = r->getTransform();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultTransposeMatrixf(&tr.m[0][0]);
	r->postViewTransform(cam);

	// setup material state
	glEnable(GL_COLOR_MATERIAL);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

//	if (materialOverride == NULL_MATERIAL_INDEX) {
//		_applyMaterial(r->getMaterialIndex());
//	} else {
	MaterialPtr mat = r->getMaterial();
	if (mat) {
		mat->applyGLState();
	}
//	}
	r->renderGeometry();

	// restore material state
	glDisable(GL_COLOR_MATERIAL);

	// restore viewing state
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glUseProgram(0);
}
