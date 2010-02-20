#ifndef _RENDERABLE_H
#define	_RENDERABLE_H

#include "AABB.h"
#include "VertexFormat.h"
#include "Material.h"
#include "PinholeCamera.h"
#include <list>

class Renderable {
protected:
	Matrix4f m_transform;
	AABB<float> m_bounds;
	MaterialPtr m_material;
	VertexFormatPtr m_vf;

public:

	Renderable() :
		m_transform(), m_bounds(), m_material(), m_vf() {
		m_transform.identity();
	}

	virtual void renderGeometry() = 0;

	/**
	 * Invoked before the modelview matrix has been setup.
	 */
	virtual void preViewTransform(PinholeCameraPtr cam) {

	}

	/**
	 * Invoked after the modelview matrix has been setup to allow the renderable to override
	 * any viewing parameters
	 */
	virtual void postViewTransform(PinholeCameraPtr cam) {

	}

	/**
	 * Invoked just before this instance gets rendered.
	 */
	virtual void preRender() {

	}

	/**
	 * Invoked right after rendering this instance.
	 */
	virtual void postRender() {

	}

	virtual VertexFormatPtr getVertexFormat() {
		return m_vf;
	}
//	virtual void setVertexFormat(VertexFormatPtr vf) {
//		m_vf = vf;
//	}

	virtual AABB<float>& getBounds() {
		return m_bounds;
	}
	virtual void setBounds(const AABB<float>& bounds) {
		m_bounds = bounds;
	}

	virtual const Matrix4f& getTransform() const {
		return m_transform;
	}

	virtual void setTransform(const Matrix4f& tr) {
		m_transform = tr;
	}

	virtual MaterialPtr getMaterial() {
		return m_material;
	}

	virtual void setMaterial(MaterialPtr material) {
		m_material = material;
	}

	//virtual uint32_t getRenderLayer() = 0;
};

typedef shared_ptr<Renderable> RenderablePtr;
typedef std::list<Renderable*> RenderablesList;

#endif	/* _RENDERABLE_H */

