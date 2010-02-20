/*
 * SceneManager.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: giorgos
 */
#include "GLTutor.h"
#include "SceneManager.h"
#include "Material.h"

SceneManager::SceneManager(ITextureManager* texMgr) :
	m_texMgr(texMgr), m_skyMapType(Sky_None), m_sphereSkyQuadric(0), m_sphereSkyMat(), m_skyboxMat(), m_skybox(), m_sphereSkyModel() {

#ifdef EXPERIMENTAL_CODE
	m_sphereSkyMat.m_texStack->texInputs[0].mapping = TexMapInput_Spherical;


	m_skyboxMat.m_texStack->texInputs[0].mapping = TexMapInput_UV;

	m_spherem_sphereSkyModel.reset(new MeshModel(MeshFactory::createSphere(radius, 32, 32)));

	m_skyboxModels[CubeMap_Face_Front]  = MeshModelPtr(new MeshModel(MeshFactory::createQuad(Vec3f(0.0f, 0.0f, -1.0f), Vec3f::Z_Axis)));
	m_skyboxModels[CubeMap_Face_Back]   = MeshModelPtr(new MeshModel(MeshFactory::createQuad(Vec3f(0.0f, 0.0f, 1.0f), Vec3f::Z_Neg_Axis)));

	m_skyboxModels[CubeMap_Face_Right]  = MeshModelPtr(new MeshModel(MeshFactory::createQuad(Vec3f(1.0f, 0.0f, 0.0f), Vec3f::X_Neg_Axis)));
	m_skyboxModels[CubeMap_Face_Left]   = MeshModelPtr(new MeshModel(MeshFactory::createQuad(Vec3f(-1.0f, 0.0f, 0.0f), Vec3f::X_Axis)));

	m_skyboxModels[CubeMap_Face_Top]    = MeshModelPtr(new MeshModel(MeshFactory::createQuad(Vec3f(0.0f, 1.0f, 0.0f), Vec3f::Y_Neg_Axis)));
	m_skyboxModels[CubeMap_Face_Bottom] = MeshModelPtr(new MeshModel(MeshFactory::createQuad(Vec3f(0.0f, -1.0f, 0.0f), Vec3f::Y_Axis)));

	m_skybox = new Entity();
	m_skybox->addRenderable(frontFace);
	m_skybox->addRenderable(backFace);
	m_skybox->addRenderable(rightFace);
	m_skybox->addRenderable(leftFace);
	m_skybox->addRenderable(topFace);
	m_skybox->addRenderable(bottomFace);

#endif

}

SceneManager::~SceneManager() {

}

void SceneManager::setSphereSkyMap(String mapFilename) {
	Texture* sphereSkyTex = new Texture();
	sphereSkyTex->m_filename = mapFilename;
	sphereSkyTex->setUseMipmaps(false);
	sphereSkyTex->setMinFilter(TexFilter_Bilinear);
	sphereSkyTex->setMagFilter(TexFilter_Bilinear);

	m_texMgr->loadTexture(mapFilename, sphereSkyTex);

	sphereSkyTex->bind();
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	m_sphereSkyMat.m_texStack->textures[0].reset(sphereSkyTex);
}

void SceneManager::setCubeSkyMap(String mapFilename) {
	m_texMgr->loadCubeMapTextures(mapFilename, m_skyboxMat.m_texStack->textures);
}

void SceneManager::renderSky() {
	if (m_skyMapType == Sky_SphereMap) {
		_renderSphereSky();
	} else if (m_skyMapType == Sky_CubeMap) {
		_renderCubeMapSky();
	}
#ifdef EXPERIMENTAL_CODE
	for (uint i = 0; i < m_skybox->getNumRenderables(); i++) {
		Renderable* r = m_skybox->getRenderable(i);
		// add it to a render layer
	}
#endif
}

void SceneManager::_renderCubeMapSky() {
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDepthMask(false);

	glColor3f(1.0f, 1.0f, 1.0f);
	glScalef(10,10,10);
	m_skyboxMat.m_texStack->textures[CubeMap_Face_Front]->bind();
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glEnd();

	m_skyboxMat.m_texStack->textures[CubeMap_Face_Back]->bind();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glEnd();

	m_skyboxMat.m_texStack->textures[CubeMap_Face_Right]->bind();
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glEnd();

	m_skyboxMat.m_texStack->textures[CubeMap_Face_Left]->bind();
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glEnd();

	m_skyboxMat.m_texStack->textures[CubeMap_Face_Top]->bind();
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, 1.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, 1.0f, -1.0f);
	glEnd();

	m_skyboxMat.m_texStack->textures[CubeMap_Face_Bottom]->bind();
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f, -1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glEnd();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	glDisable(GL_TEXTURE_2D);
}

void SceneManager::_renderSphereSky() {
	if (!m_sphereSkyQuadric) {
		m_sphereSkyQuadric = gluNewQuadric();
		gluQuadricTexture(m_sphereSkyQuadric, true);
	}
	glEnable(GL_TEXTURE_2D);
	m_sphereSkyMat.m_texStack->textures[0]->bind();
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDepthMask(false);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glRotatef(90, 1, 0, 0);

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glScalef(1, -1, 1);
	//gluQuadricDrawStyle(m_sphereSkyQuadric, GLU_LINE);
	gluSphere(m_sphereSkyQuadric, 1, 24, 12);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	glDisable(GL_TEXTURE_2D);
}

void SceneManager::dispose() {
	if (m_sphereSkyQuadric) {
		gluDeleteQuadric(m_sphereSkyQuadric);
	}
}
