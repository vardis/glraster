#include "GLTutor.h"
#include "RenderLayer.h"

RenderLayer::RenderLayer() :
	m_name(""), m_renderables(), m_overrideMaterial(), m_zorder(0) {
}

void RenderLayer::clear() {
	m_renderables.clear();
}

void RenderLayer::addRenderable(Renderable* r) {
	m_renderables.push_back(r);
}

RenderablesList& RenderLayer::depthSort() {
	return m_renderables;
}
