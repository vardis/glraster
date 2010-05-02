/*
 * SceneManager.h
 *
 *  Created on: Nov 23, 2009
 *      Author: giorgos
 */

#ifndef SCENEMANAGER_H_
#define SCENEMANAGER_H_

#include "Material.h"
#include "ITextureManager.h"
#include "MeshModel.h"
#include "Entity.h"
#include "RenderablesRasterizer.h"

typedef enum SkyMapType {
	Sky_None, Sky_SphereMap, Sky_CubeMap
} SkyMapType;


class SceneManager : public RenderListener {
public:
	SceneManager(ITextureManager* texMgr = 0);
	virtual ~SceneManager();

	bool initialize();

	void setSphereSkyMap(String mapFilename);
	void setCubeSkyMap(String mapFilename);
	void renderSky(RenderablesRasterizer* re);

	void setTextureManager(ITextureManager* texMgr) {
		m_texMgr = texMgr;
	}

	void setActiveSkyMapType(SkyMapType mapType) {
		m_skyMapType = mapType;
	}

	SkyMapType getActiveSkyMapType() const {
		return m_skyMapType;
	}

	virtual void onPreViewTransform(Renderable* r, Matrix4f& xform);

protected:
	ITextureManager* m_texMgr;
	SkyMapType m_skyMapType;

	MaterialPtr m_sphereSkyMat;
	MaterialPtr m_skyboxMat;

	Entity* m_skybox;
	MeshModelPtr m_sphereSkyModel;
	MeshModelPtr m_skyboxModels[6];

	void _loadCubeMapTextures(String mapFilename);
};

typedef shared_ptr<SceneManager> SceneManagerPtr;

#endif /* SCENEMANAGER_H_ */
