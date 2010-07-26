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
	m_texMgr(texMgr), m_skyMapType(Sky_None), m_sphereSkyMat(), m_skyboxMat(), m_skybox(), m_sphereSkyModel(), m_skyboxModel() {

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
//	rs.setRenderMode(GL_LINE);

	m_sphereSkyModel.reset(new MeshModel(mf.createSphere(10.0f, 16, 16)));
//	m_sphereSkyModel->getVertexFormat()->printData();
//	m_sphereSkyModel->getMesh()->getIbo()->printData();
	m_sphereSkyModel->setRenderState(rs);
	m_sphereSkyModel->registerListener(this);

	m_sphereSkyMat = MaterialPtr(new Material());
	m_sphereSkyMat->m_shadeless = true;
	m_sphereSkyMat->m_texStack->texInputs[0].mapping = TexMapInput_Spherical;
	m_sphereSkyModel->setMaterial(m_sphereSkyMat);

	m_skyboxModel = MeshModelPtr(new MeshModel(mf.createCube()));
	m_skyboxModel->registerListener(this);
	m_skyboxModel->getMaterial()->m_shadeless = true;
	m_skyboxModel->setRenderState(rs);
	m_skyboxModel->setTransform(Matrix4f::Scale(100.0f, 100.0f, 100.0f));

	m_skybox = new Entity();
	m_skybox->addRenderable(m_skyboxModel);

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
		re->getRenderLayer(0).addRenderable(m_skyboxModel.get());
	}
}

void SceneManager::onPreViewTransform(Renderable* r, Matrix4f& xform) {
	xform.setW(Vec3f::Zero);
}

void SceneManager::_loadCubeMapTextures(String mapFilename) {
	size_t dotPos = mapFilename.rfind('.');
	if (dotPos >= 0) {
		String basename = mapFilename.substr(0, dotPos);
		String ext = mapFilename.substr(dotPos, mapFilename.length() - dotPos);

		ImagePtr frontImg = Image::load(basename + "_FR" + ext);
		ImagePtr backImg = Image::load(basename + "_BK" + ext);
		ImagePtr leftImg = Image::load(basename + "_LF" + ext);
		ImagePtr rightImg = Image::load(basename + "_RT" + ext);
		ImagePtr topImg = Image::load(basename + "_UP" + ext);
		ImagePtr bottomImg = Image::load(basename + "_DN" + ext);

		TextureCubeMapPtr cubeMapTex = TextureCubeMapPtr(new TextureCubeMap());
		cubeMapTex->setWrapping(TexWrapMode_ClampEdge);
		cubeMapTex->fromImages(*frontImg, *backImg, *leftImg, *rightImg, *bottomImg, *topImg);
		m_skyboxModel->getMaterial()->addTexture(cubeMapTex, TexMapTo_Diffuse, TexBlendOp_Decal, TexMapInput_Cube);
	}
}
