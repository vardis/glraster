/*
 * RenderablesRasterizer.cpp
 *
 *  Created on: Nov 25, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "Material.h"
#include "MeshModel.h"
#include "ShaderConstants.h"
#include "RenderablesRasterizer.h"

#define Colour_to_float4(c, f) f[0] = c.r; f[1] = c.g; f[2] = c.b; f[3] = c.a;

RenderablesRasterizer::RenderablesRasterizer(ITextureManager* texMgr, MaterialDB* matDB) :
	m_texMgr(texMgr), m_matDB(matDB), m_shaders(), m_activeLights() {

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
	r->preViewTransform(cam);

	const Matrix4f& tr = r->getTransform();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultTransposeMatrixf(&tr.m[0][0]);
	r->postViewTransform(cam);

	r->preRender();

	// setup material state
	glEnable(GL_COLOR_MATERIAL);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	MaterialPtr mat = r->getMaterial();
	if (mat) {
		mat->applyGLState();
	}

	r->renderGeometry();

	r->postRender();

	// restore material state
	glDisable(GL_COLOR_MATERIAL);

	// restore viewing state
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glUseProgram(0);
}

#ifdef EXPERIMENTAL_CODE
void RenderablesRasterizer::render2(Renderable* r, PinholeCameraPtr cam, uint16_t materialOverride) {

	// get the existing material and if one has not been set, use the default one
	MaterialPtr material = r->getMaterial();
	if (!material) {
		material = m_defaultMaterial;
	}

	// get the material's shader and if no custom shader files have been specified, then generate
	// a new shader and assign it to the material
	GLSLProgramPtr prog;
	if (!material->hasCustomShaders()) {
		prog = shaderGen->generateProgram(material);
		material->setupShaderProgram(prog);
	} else {
		material->setupShaderProgram();
		prog = material->getGpuProgram();
	}
	material->bindShaderData();

	this->_bindLights(prog);

	// pre viewing transformation stage
	Matrix4f& viewXForm = cam->getView();
	r->preViewTransform(viewXForm);

	Matrix4f modelView = viewXForm * r->getTransform();

	// post viewing transformation stage
	r->postViewTransform(modelView);

	this->_bindShaderViewingData(cam, modelView, r->getTransform());

	// pre rendering stage
	r->preRender();

	r->bindVertexFormat(prog, r->getVertexFormat());
	r->renderGeometry();

	// post viewing transformation stage
	r->postRender();
}

void RenderablesRasterizer::_bindLights(GLSLProgramPtr program) {
	for (size_t i = 0; i < m_activeLights.size(); i++) {
		LightPtr l = m_activeLights[i];
		if (!l->m_active) continue;

		program->setUniform(ShaderConstants::UNIFORM_LightsType[i], l->m_type);
		program->setUniform(ShaderConstants::UNIFORM_LightsPos[i], l->m_position);
		program->setUniform(ShaderConstants::UNIFORM_LightsDirection[i], l->m_direction);
		program->setUniform(ShaderConstants::UNIFORM_LightsAmbient[i], l->m_ambient);
		program->setUniform(ShaderConstants::UNIFORM_LightsDiffuse[i], l->m_diffuse);
		program->setUniform(ShaderConstants::UNIFORM_LightsSpecular[i], l->m_specular);
		program->setUniform(ShaderConstants::UNIFORM_LightsCosSpotCutOff[i], l->m_cosSpotCutOff);
		program->setUniform(ShaderConstants::UNIFORM_LightsAttenuation[i], l->m_attenuation);
		program->setUniform(ShaderConstants::UNIFORM_LightsLinearAttenuation[i], l->m_linearAttenuation);
		program->setUniform(ShaderConstants::UNIFORM_LightsQuadraticAttenuation[i], l->m_quadraticAttenuation);
	}
}

void RenderablesRasterizer::_bindShaderViewingData(GLSLProgramPtr program, PinholeCameraPtr cam, Matrix4f& modelView, Matrix4f& model) {
	program->setUniform(ShaderConstants::UNIFORM_Model, model);
	program->setUniform(ShaderConstants::UNIFORM_ModelView, modelView);
	program->setUniform(ShaderConstants::UNIFORM_View, cam->getView());
	program->setUniform(ShaderConstants::UNIFORM_Proj, cam->getProj());
	program->setUniform(ShaderConstants::UNIFORM_ModelViewProj, cam->getProj() * modelView);

	//TODO: check if I need to use the inverse transpose here or i can trust this matrix is orthonormal
	program->setUniform(ShaderConstants::UNIFORM_NormalMatrix, modelView);
}

// This should go to the Renderable interface
void RenderablesRasterizer::bindVertexFormat(GLSLProgramPtr prog, VertexFormatPtr vf) {

	VertexAttribute* posAttrib = vf->getAttributeBySemantic(Vertex_Pos);
	if (posAttrib) {
		GLuint i = prog->getAttributeIndex(ShaderConstants::VATTR_Pos);
		posAttrib->bindAttributeData(i);
	}

	VertexAttribute* normalAttrib = vf->getAttributeBySemantic(Vertex_Normal);
	if (normalAttrib) {
		GLuint i = prog->getAttributeIndex(ShaderConstants::VATTR_Normal);
		normalAttrib->bindAttributeData(i);
	}

	VertexAttribute* colorAttrib = vf->getAttributeBySemantic(Vertex_Color);
	if (colorAttrib) {
		GLuint i = prog->getAttributeIndex(ShaderConstants::VATTR_Color);
		colorAttrib->bindAttributeData(i);
	}
}

void RenderablesRasterizer::_bindTextureStack(GLSLProgramPtr prog, TextureStackPtr texStack) {
	int lastUvSet = -1;
	for (int i = 0; i < MAX_TEXTURES_STACK; i++) {
		TexturePtr tex = texStack->textures[i];
		if (tex && lastUvSet < texStack->texInputs[i].uvSet) {
			lastUvSet = texStack->texInputs[i].uvSet;
			VertexAttributeSemantic sem = Vertex_TexCoord0;
			switch (lastUvSet) {
				case 0:
				sem = Vertex_TexCoord0;
				case 1:
				sem = Vertex_TexCoord1;
				case 2:
				sem = Vertex_TexCoord2;
				case 3:
				sem = Vertex_TexCoord3;
				case 4:
				sem = Vertex_TexCoord4;
				case 5:
				sem = Vertex_TexCoord5;
				case 6:
				sem = Vertex_TexCoord6;
				case 7:
				sem = Vertex_TexCoord7;
				default:
				sem = Vertex_TexCoord0;
			}

			VertexAttribute* uvCoordsAttrib = vf->getAttributeBySemantic(sem);
			if (uvCoordsAttrib) {
				GLuint i = prog->getAttributeIndex(ShaderConstants::VATTR_TexCoords[lastUvSet]);
				uvCoordsAttrib->bindAttributeData(i);
			}
		}
	}
}

#endif

void RenderablesRasterizer::addLight(LightPtr l) {
	m_activeLights.push_back(l);
}

void RenderablesRasterizer::clearLights() {
	m_activeLights.clear();
}

void RenderablesRasterizer::setRender2D(uint width, uint height) {
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, width, 0, height);
}
