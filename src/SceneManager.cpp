/*
 * SceneManager.cpp
 *
 *  Created on: Nov 23, 2009
 *      Author: giorgos
 */
#include "GLTutor.h"
#include "SceneManager.h"

SceneManager::SceneManager(ITextureManager* texMgr) :
	m_texMgr(texMgr), m_skyMapType(Sky_None), m_sphereSkyTex(), m_sphereSkyQuadric(0) {

}

SceneManager::~SceneManager() {

}

void SceneManager::setSphereSkyMap(String mapFilename) {
	m_sphereSkyTex.reset(new Texture());
	m_sphereSkyTex->m_filename = mapFilename;
	m_sphereSkyTex->m_useMipmaps = false;
	m_sphereSkyTex->m_minFilter = TexFilter_Bilinear;
	m_sphereSkyTex->m_magFilter = TexFilter_Bilinear;
	m_texMgr->loadTexture(mapFilename, m_sphereSkyTex.get());

	glBindTexture(GL_TEXTURE_2D, m_sphereSkyTex->m_texID);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
}

void SceneManager::setCubeSkyMap(String mapFilename) {
	m_texMgr->loadCubeMapTextures(mapFilename, &m_skyCubemap[0]);
}

void SceneManager::renderSky() {
	if (m_skyMapType == Sky_SphereMap) {
		_renderSphereSky();
	} else if (m_skyMapType == Sky_CubeMap) {
		_renderCubeMapSky();
	}
}

void SceneManager::_renderCubeMapSky() {
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDepthMask(false);

	glColor3f(1.0f, 1.0f, 1.0f);
	glScalef(10,10,10);
	glBindTexture(GL_TEXTURE_2D, m_skyCubemap[CubeMap_Face_Front]);
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

	glBindTexture(GL_TEXTURE_2D, m_skyCubemap[CubeMap_Face_Back]);
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

	glBindTexture(GL_TEXTURE_2D, m_skyCubemap[CubeMap_Face_Right]);
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

	glBindTexture(GL_TEXTURE_2D, m_skyCubemap[CubeMap_Face_Left]);
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

	glBindTexture(GL_TEXTURE_2D, m_skyCubemap[CubeMap_Face_Up]);
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

	glBindTexture(GL_TEXTURE_2D, m_skyCubemap[CubeMap_Face_Down]);
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
	glBindTexture(GL_TEXTURE_2D, m_sphereSkyTex->m_texID);
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
	if (m_sphereSkyTex) {
		m_texMgr->disposeTexture(m_sphereSkyTex->m_texID);
	}
	for (int i = 0; i < 6; i++) {
		GLuint tex = m_skyCubemap[CubeMap_Face_Front + i];
		if (tex) {
			m_texMgr->disposeTexture(tex);
		}
	}
}
