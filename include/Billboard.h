/*
 * Billboard.h
 *
 *  Created on: Dec 3, 2009
 *      Author: giorgos
 */

#ifndef BILLBOARD_H_
#define BILLBOARD_H_

#include "Renderable.h"
#include "RenderPrimitive.h"

typedef enum BillboardType {
	Billboard_Cylindrical,
	Billboard_Spherical
}BillboardType;

class Billboard: public Renderable {
private:
	BillboardType m_type;
	Vec3f         m_upAxis;

#ifdef EXPERIMENTAL_CODE
	RenderPrimitive<TrianglesPrimitiveType>* m_render;
#endif

public:
	Billboard(BillboardType type, float width, float height);

	virtual void renderGeometry();
	virtual void postViewTransform(PinholeCameraPtr cam);
};

#endif /* BILLBOARD_H_ */
