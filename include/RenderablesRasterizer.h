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
#include "Lights.h"
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
	std::vector<LightPtr> m_activeLights;

	FogParameters m_fogParams;

public:
	RenderablesRasterizer(ITextureManager* texMgr);
	virtual ~RenderablesRasterizer();

	void beginFrame(CameraPtr cam);
	void endFrame();
	void render(Renderable* r, CameraPtr cam, uint16_t materialOverride = NULL_MATERIAL_INDEX);

	RenderLayer& getRenderLayer(uint8_t i) {
		return m_layers[i];
	}

    void addLight(LightPtr l);
    void clearLights();

    FogParameters getFogParameters() const {
    	return m_fogParams;
    }

    void setFogParameters(const FogParameters params) {
    	m_fogParams = params;
    }

private:
	void _bindRenderState(RenderState& state);
	void _bindLights(GLSLProgramPtr program, Matrix4f& modelView);
	void _bindShaderViewingData(GLSLProgramPtr program, CameraPtr cam, Matrix4f& modelView, Matrix4f& model);
	void bindVertexFormat(GLSLProgramPtr prog, VertexFormatPtr vf);
	void _bindUVCoords(GLSLProgramPtr prog, TextureStackPtr texStack, VertexFormatPtr vf);
	void _bindFogParameters(GLSLProgramPtr prog, FogParameters& fogParams);
};

#endif /* RENDERABLESRASTERIZER_H_ */
