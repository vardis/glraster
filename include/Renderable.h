#ifndef _RENDERABLE_H
#define	_RENDERABLE_H

#include <list>

#include "AABB.h"
#include "RenderState.h"
#include "VertexFormat.h"
#include "Material.h"
#include "Matrix4.h"
#include "RenderListener.h"


class Renderable {
protected:
	Matrix4f m_transform;
	AABB<float> m_bounds;
	RenderState m_state;
	MaterialPtr m_material;
	VertexFormatPtr m_vf;
	std::list<RenderListener*> m_listeners;

public:

	Renderable() :
		m_transform(), m_bounds(), m_material(), m_vf(), m_listeners() {
		m_transform.identity();
	}

	virtual void renderGeometry() = 0;

	/**
	 * Invoked before the modelview matrix has been setup.
	 */
	virtual void preViewTransform(Matrix4f& xform) {

	}

	/**
	 * Invoked after the modelview matrix has been setup to allow the renderable to override
	 * any viewing parameters
	 */
	virtual void postViewTransform(Matrix4f& xform) {

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

	RenderState& getRenderState() {
		return m_state;
	}

	void setRenderState(const RenderState& rs) {
		m_state = rs;
	}

	VertexFormatPtr getVertexFormat() {
		return m_vf;
	}

	AABB<float>& getBounds() {
		return m_bounds;
	}
	void setBounds(const AABB<float>& bounds) {
		m_bounds = bounds;
	}

	const Matrix4f& getTransform() const {
		return m_transform;
	}

	Matrix4f& getTransform() {
		return m_transform;
	}

	void setTransform(const Matrix4f& tr) {
		m_transform = tr;
	}

	virtual MaterialPtr getMaterial() {
		return m_material;
	}

	virtual void setMaterial(MaterialPtr material) {
		m_material = material;
	}

	void registerListener(RenderListener* r) {
		m_listeners.push_back(r);
	}

	void unregisterListener(RenderListener* r) {
		std::list<RenderListener*>::iterator i = std::find(m_listeners.begin(), m_listeners.end(), r);
		if (i != m_listeners.end()) {
			m_listeners.erase(i);
		}
	}

	std::list<RenderListener*> getListeners() {
		return m_listeners;
	}
};

typedef shared_ptr<Renderable> RenderablePtr;
typedef std::list<Renderable*> RenderablesList;

#endif	/* _RENDERABLE_H */

