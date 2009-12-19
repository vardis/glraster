#ifndef RENDERLAYER_H_
#define RENDERLAYER_H_

#include "Renderable.h"
#include "Material.h"

class RenderLayer {

public:
	RenderLayer();

	void clear();
	void addRenderable(Renderable* r);

	RenderablesList& depthSort();

	//setReceiveShadows(bool);
	//setLightMask(int);

private:
	String          m_name;
	RenderablesList m_renderables;
	MaterialPtr     m_overrideMaterial;
	uint8_t         m_zorder;
	//enum RenderMode m_renderMode;
};
typedef shared_ptr<RenderLayer> RenderLayerPtr;

#endif /* RENDERLAYER_H_ */
