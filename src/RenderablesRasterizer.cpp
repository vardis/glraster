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
#include "ShaderGenerator.h"
#include "RenderablesRasterizer.h"

#define Colour_to_float4(c, f) f[0] = c.r; f[1] = c.g; f[2] = c.b; f[3] = c.a;

RenderablesRasterizer::RenderablesRasterizer(ITextureManager* texMgr) :
	m_texMgr(texMgr), m_shaders(), m_activeLights() {

}

RenderablesRasterizer::~RenderablesRasterizer() {
}

void RenderablesRasterizer::beginFrame(CameraPtr cam) {
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

void _invokePreViewTransformListeners(Renderable* r, Matrix4f& xform) {
	std::list<RenderListener*> listeners = r->getListeners();
	if (listeners.size() > 0) {
		std::list<RenderListener*>::iterator i = listeners.begin();
		while (i != listeners.end()) {
			(*i++)->onPreViewTransform(r, xform);
		}
	}
}

void _invokePostViewTransformListeners(Renderable* r, Matrix4f& xform) {
	std::list<RenderListener*> listeners = r->getListeners();
	if (listeners.size() > 0) {
		std::list<RenderListener*>::iterator i = listeners.begin();
		while (i != listeners.end()) {
			(*i++)->onPostViewTransform(r, xform);
		}
	}
}

void RenderablesRasterizer::render(Renderable* r, CameraPtr cam, uint16_t materialOverride) {

	// get the existing material and if one has not been set, use the default one
	MaterialPtr material = r->getMaterial();

	// get the material's shader and if no custom shader files have been specified, then generate
	// a new shader and assign it to the material
	GLSLProgramPtr prog = material->getGpuProgram();
	if (material->hasCustomShaders()) {
		material->setupShaderProgram();
	} else if (!material->getGpuProgram()->isLinked()) {
		ShaderGenerator shaderGen;
		shaderGen.generateShaders(*material, r->getRenderState(), *r->getVertexFormat());
		material->setupShaderProgram();
	}
	material->bindShaderData();

	RenderState& rs = r->getRenderState();
	this->_bindRenderState(rs);

	// pre-viewing transformation stage
	Matrix4f viewXForm = cam->getView();
	_invokePreViewTransformListeners(r, viewXForm);

	Matrix4f modelView = viewXForm * r->getTransform();

	// post viewing transformation stage
	//_invokePostViewTransformListeners(r, modelView);

	if (!material->m_shadeless) {
		this->_bindLights(prog, cam->getView());
	}

	this->_bindShaderViewingData(prog, cam, modelView, r->getTransform());

	// bind vertex attributes
	this->bindVertexFormat(prog, r->getVertexFormat());
	this->_bindUVCoords(prog, material->getTexStack(), r->getVertexFormat());

	// bind output to the current render target
	glBindFragDataLocation(prog->m_progID, 0, ShaderConstants::Fragment_Color);

	// pre-rendering stage
	r->preRender();

	r->renderGeometry();

	// post render stage
	r->postRender();

	glUseProgram(0);
}

void RenderablesRasterizer::_bindRenderState(RenderState& rs) {

	glColorMask(rs.getColorMask(), rs.getColorMask(), rs.getColorMask(), rs.getColorMask());
	glDepthMask(rs.getDepthMask());

	if (rs.getDepthTest()) {
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(rs.getDepthFunc());
	} else {
		glDisable(GL_DEPTH_TEST);
	}

	if (rs.getCulling()) {
		glEnable(GL_CULL_FACE);
		glCullFace(rs.getCullFace());
		glFrontFace(rs.getFrontFace());
	} else {
		glDisable(GL_CULL_FACE);
	}

	glPolygonMode(GL_FRONT_AND_BACK, rs.getRenderMode());

	int clearBits = 0;
	if (rs.getClearColorBuffer()) {
		clearBits |= GL_COLOR_BUFFER_BIT;
		Colour clr = rs.getClearColor();
		glClearColor(clr.r, clr.g, clr.b, clr.a);
	}

	if (rs.getClearDepthBuffer()) {
		clearBits |= GL_DEPTH_BUFFER_BIT;
		glClearDepth(rs.getClearDepth());
	}

	if (rs.getClearStencilBuffer()) {
		clearBits |= GL_STENCIL_BUFFER_BIT;
		glClearStencil(rs.getClearStencil());
	}

	if (clearBits != 0) {
		glClear(clearBits);
	}

	if (rs.getScissor()) {
		glEnable(GL_SCISSOR_TEST);
		glScissor(rs.getScissorLeft(), rs.getScissorBottom(), rs.getScissorWidth(), rs.getScissorHeight());
		glStencilFunc(rs.getStencilFunc(), rs.getStencilRef(), rs.getStencilMask());
		glStencilOp(rs.getStencilOpFail(), rs.getStencilOpFail(), rs.getStencilOpPass());
	} else {
		glDisable(GL_SCISSOR_TEST);
	}

	if (rs.getStencil()) {
		glEnable(GL_STENCIL_TEST);
	} else {
		glDisable(GL_STENCIL_TEST);
	}

	if (rs.getBlend()) {
		glEnable(GL_BLEND);
		glBlendFunc(rs.getBlenSrcFunc(), rs.getBlenDstFunc());
	} else {
		glDisable(GL_BLEND);
	}
}

void RenderablesRasterizer::_bindLights(GLSLProgramPtr program, Matrix4f& view) {

	size_t numLights = m_activeLights.size();
	program->setUniform(ShaderConstants::UNIFORM_NumLights, numLights);
	for (size_t i = 0; i < numLights; i++) {
		LightPtr l = m_activeLights[i];
		if (!l->m_active)
			continue;

		program->setUniform(ShaderConstants::UNIFORM_LightsType[i], l->m_type);
		program->setUniform(ShaderConstants::UNIFORM_LightsWorldPos[i], l->m_position);
		program->setUniform(ShaderConstants::UNIFORM_LightsWorldDirection[i], l->m_direction);

		Vec3f lp = view * l->m_position;
		program->setUniform(ShaderConstants::UNIFORM_LightsPos[i], lp);

		Vec3f ld = view.upperLeft().inversed().transposed() * l->m_direction;
		ld.normalize();
		program->setUniform(ShaderConstants::UNIFORM_LightsDirection[i], ld);

		program->setUniform(ShaderConstants::UNIFORM_LightsAmbient[i], l->m_ambient);
		program->setUniform(ShaderConstants::UNIFORM_LightsDiffuse[i], l->m_diffuse);
		program->setUniform(ShaderConstants::UNIFORM_LightsSpecular[i], l->m_specular);
		program->setUniform(ShaderConstants::UNIFORM_LightsCosSpotCutOff[i], l->m_cosSpotCutOff);
		program->setUniform(ShaderConstants::UNIFORM_LightsConstAttenuation[i], l->m_attenuation);
		program->setUniform(ShaderConstants::UNIFORM_LightsLinearAttenuation[i], l->m_linearAttenuation);
		program->setUniform(ShaderConstants::UNIFORM_LightsQuadraticAttenuation[i], l->m_quadraticAttenuation);
	}
}

void RenderablesRasterizer::_bindShaderViewingData(GLSLProgramPtr program, CameraPtr cam, Matrix4f& modelView,
		Matrix4f& model) {

	Matrix3f modelInv(model.upperLeft().inversed().transposed());
	program->setUniform(ShaderConstants::UNIFORM_Model, model);
	program->setUniform(ShaderConstants::UNIFORM_ModelInverse, modelInv);

	program->setUniform(ShaderConstants::UNIFORM_ModelView, modelView);

//			float* p = modelView.inversed().transposed().ptr();
//				std::cout << "modelView matrix { ";
//				for (int i = 0; i < 16; i++, p++) {
//					if (i % 4 == 0) std::cout << "\n";
//					std::cout << *p << " , ";
//				}
//				std::cout << "}\n";
	program->setUniform(ShaderConstants::UNIFORM_View, cam->getView());
	program->setUniform(ShaderConstants::UNIFORM_Proj, cam->getProj());

	//	float* p = cam->getProj().ptr();
	//	std::cout << "Projection matrix { ";
	//	for (int i = 0; i < 16; i++, p++) {
	//		if (i % 4 == 0) std::cout << "\n";
	//		std::cout << *p << " , ";
	//	}
	//	std::cout << "}\n";

	Matrix4f modelViewProj = cam->getProj() * modelView;
	program->setUniform(ShaderConstants::UNIFORM_ModelViewProj, modelViewProj);

	//TODO: check if I need to use the inverse transpose here or i can trust this matrix is orthonormal
	Matrix3f normalMat = modelView.upperLeft().inversed().transposed();
	program->setUniform(ShaderConstants::UNIFORM_NormalMatrix, normalMat);
}

// This should go to the Renderable interface
void RenderablesRasterizer::bindVertexFormat(GLSLProgramPtr prog, VertexFormatPtr vf) {

	enum VertexAttributeSemantic vaSem[] = { Vertex_Pos, Vertex_Normal, Vertex_BiNormal, Vertex_Tangent, Vertex_Color };
	const char* shaderConsts[] = { ShaderConstants::VATTR_Pos, ShaderConstants::VATTR_Normal,
									ShaderConstants::VATTR_BiNormal, ShaderConstants::VATTR_Tangent,
									ShaderConstants::VATTR_Color };
	for (int j = 0; j < 5; j++) {
		VertexAttribute* attrib = vf->getAttributeBySemantic(vaSem[j]);
		if (attrib) {
			GLuint i = prog->getAttributeIndex(shaderConsts[j]);
			attrib->m_vbo->bindAttributeData(i);
		}
	}
}

void RenderablesRasterizer::_bindUVCoords(GLSLProgramPtr prog, TextureStackPtr texStack, VertexFormatPtr vf) {
	int lastUvSet = -1;
	for (int i = 0; i < MAX_TEXTURES_STACK; i++) {
		TexturePtr tex = texStack->textures[i];
		if (tex && lastUvSet < texStack->texInputs[i].uvSet) {
			lastUvSet = texStack->texInputs[i].uvSet;
			VertexAttributeSemantic sem = Vertex_TexCoord0;
			switch (lastUvSet) {
			case 0:
				sem = Vertex_TexCoord0;
				break;
			case 1:
				sem = Vertex_TexCoord1;
				break;
			case 2:
				sem = Vertex_TexCoord2;
				break;
			case 3:
				sem = Vertex_TexCoord3;
				break;
			case 4:
				sem = Vertex_TexCoord4;
				break;
			case 5:
				sem = Vertex_TexCoord5;
				break;
			case 6:
				sem = Vertex_TexCoord6;
				break;
			case 7:
				sem = Vertex_TexCoord7;
				break;
			default:
				sem = Vertex_TexCoord0;
				break;
			}

			VertexAttribute* uvCoordsAttrib = vf->getAttributeBySemantic(sem);
			if (uvCoordsAttrib) {
				GLint i = prog->getAttributeIndex(ShaderConstants::VATTR_TexCoords[lastUvSet]);
				if (i == -1) {
					//					;std::cout << "VERTEX ATTRIB INDEX[" << lastUvSet << "]: " << i << std::endl;
				} else {
					uvCoordsAttrib->m_vbo->bindAttributeData(i);
				}
			}
		}
	}
}

void RenderablesRasterizer::addLight(LightPtr l) {
	m_activeLights.push_back(l);
}

void RenderablesRasterizer::clearLights() {
	m_activeLights.clear();
}
