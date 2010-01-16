/*
 * RenderablesRasterizer.h
 *
 *  Created on: Nov 25, 2009
 *      Author: giorgos
 */

#ifndef RENDERABLESRASTERIZER_H_
#define RENDERABLESRASTERIZER_H_

#include "PinholeCamera.h"
#include "Renderable.h"
#include "RenderLayer.h"
#include "ITextureManager.h"
#include "ShaderManager.h"
#include "MaterialDB.h"

#define MAX_RENDER_LAYERS 20
#define DEFAULT_LAYER 10

class RenderablesRasterizer {

private:
	RenderLayer      m_layers[MAX_RENDER_LAYERS];
	ITextureManager* m_texMgr;
	MaterialDB*      m_matDB;
	ShaderManager    m_shaders;

public:
	RenderablesRasterizer(ITextureManager* texMgr, MaterialDB* matDB);
	virtual ~RenderablesRasterizer();

	void beginFrame(PinholeCameraPtr cam);
	void endFrame();
	void render(Renderable* r, PinholeCameraPtr cam, uint16_t materialOverride = NULL_MATERIAL_INDEX);
	RenderLayer& getRenderLayer(uint8_t i) {
		return m_layers[i];
	}

//	void setRenderTarget(RenderTarget rt);

	void setRender2D(uint width, uint height);
};

#endif /* RENDERABLESRASTERIZER_H_ */
