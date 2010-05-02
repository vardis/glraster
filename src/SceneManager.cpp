/*
 * SceneManager.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: giorgos
 */
#include "GLTutor.h"
#include "MeshFactory.h"
#include "SceneManager.h"
#include "Material.h"

SceneManager::SceneManager(ITextureManager* texMgr) :
	m_texMgr(texMgr), m_skyMapType(Sky_None), m_sphereSkyMat(), m_skyboxMat(), m_skybox(), m_sphereSkyModel() {

}

SceneManager::~SceneManager() {

}

bool SceneManager::initialize() {
	MeshFactory mf(m_texMgr);

	// common render states for both types of sky
	RenderState rs;
	rs.setCulling(false);
	rs.setDepthTest(false);
	rs.setDepthMask(false);
	rs.setRenderMode(GL_LINE);

	m_sphereSkyModel.reset(new MeshModel(mf.createSphere(10.0f, 16, 16)));
	m_sphereSkyModel->getVertexFormat()->printData();
	m_sphereSkyModel->getMesh()->getIbo()->printData();
	m_sphereSkyModel->setRenderState(rs);
	m_sphereSkyModel->registerListener(this);

	m_sphereSkyMat = MaterialPtr(new Material());
	m_sphereSkyMat->m_shadeless = true;
	m_sphereSkyMat->m_texStack->texInputs[0].mapping = TexMapInput_Spherical;
	m_sphereSkyModel->setMaterial(m_sphereSkyMat);

	m_skyboxModels[CubeMap_Face_Front] = MeshModelPtr(new MeshModel(mf.createQuad(Vec3f(0.0f, 0.0f, -1.0f),
			Vec3f::Z_Axis)));
	m_skyboxModels[CubeMap_Face_Front]->getMaterial()->m_texStack->texInputs[0].mapping = TexMapInput_UV;
	m_skyboxModels[CubeMap_Face_Front]->getMaterial()->m_shadeless = true;
	m_skyboxModels[CubeMap_Face_Front]->setRenderState(rs);

	m_skyboxModels[CubeMap_Face_Back] = MeshModelPtr(new MeshModel(mf.createQuad(Vec3f(0.0f, 0.0f, 1.0f),
			Vec3f::Z_Neg_Axis)));
	m_skyboxModels[CubeMap_Face_Back]->getMaterial()->m_texStack->texInputs[0].mapping = TexMapInput_UV;
	m_skyboxModels[CubeMap_Face_Back]->getMaterial()->m_shadeless = true;
	m_skyboxModels[CubeMap_Face_Back]->setRenderState(rs);

	m_skyboxModels[CubeMap_Face_Right] = MeshModelPtr(new MeshModel(mf.createQuad(Vec3f(1.0f, 0.0f, 0.0f),
			Vec3f::X_Neg_Axis)));
	m_skyboxModels[CubeMap_Face_Right]->getMaterial()->m_texStack->texInputs[0].mapping = TexMapInput_UV;
	m_skyboxModels[CubeMap_Face_Right]->getMaterial()->m_shadeless = true;
	m_skyboxModels[CubeMap_Face_Right]->setRenderState(rs);

	m_skyboxModels[CubeMap_Face_Left] = MeshModelPtr(new MeshModel(mf.createQuad(Vec3f(-1.0f, 0.0f, 0.0f),
			Vec3f::X_Axis)));
	m_skyboxModels[CubeMap_Face_Left]->getMaterial()->m_texStack->texInputs[0].mapping = TexMapInput_UV;
	m_skyboxModels[CubeMap_Face_Left]->getMaterial()->m_shadeless = true;
	m_skyboxModels[CubeMap_Face_Left]->setRenderState(rs);

	m_skyboxModels[CubeMap_Face_Top] = MeshModelPtr(new MeshModel(mf.createQuad(Vec3f(0.0f, 1.0f, 0.0f),
			Vec3f::Y_Neg_Axis)));
	m_skyboxModels[CubeMap_Face_Top]->getMaterial()->m_texStack->texInputs[0].mapping = TexMapInput_UV;
	m_skyboxModels[CubeMap_Face_Top]->getMaterial()->m_shadeless = true;
	m_skyboxModels[CubeMap_Face_Top]->setRenderState(rs);

	m_skyboxModels[CubeMap_Face_Bottom] = MeshModelPtr(new MeshModel(mf.createQuad(Vec3f(0.0f, -1.0f, 0.0f),
			Vec3f::Y_Axis)));
	m_skyboxModels[CubeMap_Face_Bottom]->getMaterial()->m_texStack->texInputs[0].mapping = TexMapInput_UV;
	m_skyboxModels[CubeMap_Face_Bottom]->getMaterial()->m_shadeless = true;
	m_skyboxModels[CubeMap_Face_Bottom]->setRenderState(rs);

	m_skybox = new Entity();
	m_skybox->addRenderable(m_skyboxModels[CubeMap_Face_Front]);
	m_skybox->addRenderable(m_skyboxModels[CubeMap_Face_Back]);
	m_skybox->addRenderable(m_skyboxModels[CubeMap_Face_Right]);
	m_skybox->addRenderable(m_skyboxModels[CubeMap_Face_Left]);
	m_skybox->addRenderable(m_skyboxModels[CubeMap_Face_Top]);
	m_skybox->addRenderable(m_skyboxModels[CubeMap_Face_Bottom]);

	return true;
}

void SceneManager::setSphereSkyMap(String mapFilename) {

	TexturePtr sphereSkyTex = TexturePtr(new Texture());
	sphereSkyTex->m_filename = mapFilename;

	sphereSkyTex->setUseMipmaps(false);
	sphereSkyTex->setMinFilter(TexFilter_Bilinear);
	sphereSkyTex->setMagFilter(TexFilter_Bilinear);
	sphereSkyTex->setWrapping(TexWrapMode_Repeat);
	m_texMgr->loadTexture(mapFilename, sphereSkyTex.get());
	m_sphereSkyMat->m_texStack->textures[0] = sphereSkyTex;
}

void SceneManager::setCubeSkyMap(String mapFilename) {
	_loadCubeMapTextures(mapFilename);
}

void SceneManager::renderSky(RenderablesRasterizer* re) {
	if (m_skyMapType == Sky_SphereMap) {
		re->getRenderLayer(0).addRenderable(m_sphereSkyModel.get());
	} else if (m_skyMapType == Sky_CubeMap) {
		for (uint i = 0; i < m_skybox->getNumRenderables(); i++) {
			Renderable* r = m_skybox->getRenderable(i);
			re->getRenderLayer(0).addRenderable(r);
		}
	}
}

void SceneManager::onPreViewTransform(Renderable* r, Matrix4f& xform) {
//	xform.setW(Vec3f::Zero);
}

void SceneManager::_loadCubeMapTextures(String mapFilename) {
	size_t dotPos = mapFilename.rfind('.');
	if (dotPos >= 0) {
		String basename = mapFilename.substr(0, dotPos);
		String ext = mapFilename.substr(dotPos, mapFilename.length() - dotPos);

		TexturePtr frontTex = TexturePtr(new Texture());
		frontTex->setWrapping(TexWrapMode_ClampEdge);
		m_texMgr->loadTexture(basename + "_FR" + ext, frontTex.get());
		m_skyboxModels[CubeMap_Face_Front]->getMaterial()->m_texStack->textures[0] = frontTex;

		TexturePtr backTex = TexturePtr(new Texture());
		backTex->setWrapping(TexWrapMode_ClampEdge);
		m_texMgr->loadTexture(basename + "_BK" + ext, backTex.get());
		m_skyboxModels[CubeMap_Face_Back]->getMaterial()->m_texStack->textures[0] = backTex;

		TexturePtr rightTex = TexturePtr(new Texture());
		rightTex->setWrapping(TexWrapMode_ClampEdge);
		m_texMgr->loadTexture(basename + "_RT" + ext, rightTex.get());
		m_skyboxModels[CubeMap_Face_Right]->getMaterial()->m_texStack->textures[0] = rightTex;

		TexturePtr leftTex = TexturePtr(new Texture());
		leftTex->setWrapping(TexWrapMode_ClampEdge);
		m_texMgr->loadTexture(basename + "_LF" + ext, leftTex.get());
		m_skyboxModels[CubeMap_Face_Left]->getMaterial()->m_texStack->textures[0] = leftTex;

		TexturePtr upTex = TexturePtr(new Texture());
		upTex->setWrapping(TexWrapMode_ClampEdge);
		m_texMgr->loadTexture(basename + "_UP" + ext, upTex.get());
		m_skyboxModels[CubeMap_Face_Top]->getMaterial()->m_texStack->textures[0] = upTex;

		TexturePtr bottomTex = TexturePtr(new Texture());
		bottomTex->setWrapping(TexWrapMode_ClampEdge);
		m_texMgr->loadTexture(basename + "_DN" + ext, bottomTex.get());
		m_skyboxModels[CubeMap_Face_Bottom]->getMaterial()->m_texStack->textures[0] = bottomTex;
	}
}
