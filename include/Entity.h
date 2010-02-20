/*
 * Entity.h
 *
 *  Created on: Nov 29, 2009
 *      Author: giorgos
 */

#ifndef ENTITY_H_
#define ENTITY_H_

#include "Renderable.h"

class Entity {
public:
	Entity();
	virtual ~Entity();

	virtual Matrix4f& getTransform() {
		return m_transform;
	}

	virtual void setTransform(const Matrix4f& tr) {
		m_transform = tr;
	}

	AABB<float>& getBounds() {
		return m_bounds;
	}

	void addRenderable(Renderable* r);
	void clearRenderables();

	uint32_t getNumRenderables() const {
		return m_renderables.size();
	}
	Renderable* getRenderable(uint32_t i) {
		assert(i < m_renderables.size());
		return m_renderables[i].get();
	}

protected:
	Matrix4f m_transform;
	std::vector<RenderablePtr> m_renderables;
	AABB<float> m_bounds;
};

#endif /* ENTITY_H_ */
