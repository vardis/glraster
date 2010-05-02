/*
 * Entity.cpp
 *
 *  Created on: Nov 29, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "Entity.h"

Entity::Entity() :	m_transform(), m_renderables(), m_bounds() {
	m_transform.identity();
}

Entity::~Entity() {
}

void Entity::addRenderable(Renderable* r) {
	this->addRenderable(RenderablePtr(r));
}

void Entity::addRenderable(RenderablePtr r) {
	m_renderables.push_back(r);
	m_bounds.unionWith(r->getBounds());
}

void Entity::clearRenderables() {
	m_renderables.clear();
	m_bounds = AABB<float>::EmptyAABB();
}
