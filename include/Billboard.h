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

class Billboard;
typedef shared_ptr<Billboard> BillboardPtr;

class Billboard: public Renderable, public RenderListener {
private:
	BillboardType m_type;
	Vec3f         m_upAxis;
	RenderPrimitive<TrianglesPrimitiveType>* m_render;


public:
	static BillboardPtr create(BillboardType type, float width, float height);

	virtual void renderGeometry();

	/** Override that manipulates the model-view matrix in order to get a billboard effect */
	virtual void onPostViewTransform(Renderable* r, Matrix4f& xform);

protected:
	Billboard();
};

#endif /* BILLBOARD_H_ */
