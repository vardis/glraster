/*
 * SceneManager.h
 *
 *  Created on: Nov 23, 2009
 *      Author: giorgos
 */

#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include "ITextureManager.h"

typedef enum SkyMapType {
	Sky_None, Sky_SphereMap, Sky_CubeMap
} SkyMapType;


class SceneManager {
public:
	SceneManager(ITextureManager* texMgr = 0);
	virtual ~SceneManager();

	void setSphereSkyMap(String mapFilename);
	void setCubeSkyMap(String mapFilename);
	void renderSky();

	void dispose();

	void setTextureManager(ITextureManager* texMgr) {
		m_texMgr = texMgr;
	}

	void setActiveSkyMapType(SkyMapType mapType) {
		m_skyMapType = mapType;
	}

	SkyMapType getActiveSkyMapType() const {
		return m_skyMapType;
	}

private:
	void _renderCubeMapSky();
	void _renderSphereSky();

protected:
	ITextureManager* m_texMgr;
	SkyMapType m_skyMapType;
	TexturePtr m_sphereSkyTex;
	GLUquadric* m_sphereSkyQuadric;

	// texture name of the skybox cubemap's textures
	GLuint m_skyCubemap[6];
};

#endif /* SCENEMANAGER_H_ */
