/*
 * RenderListener.h
 *
 *  Created on: Mar 17, 2010
 *      Author: giorgos
 */

#ifndef RENDERLISTENER_H_
#define RENDERLISTENER_H_

#include "Matrix4.h"
#include "Renderable.h"

class Renderable;

class RenderListener {
public:
	virtual void onPreRender(Renderable* r) {
	}
	virtual void onPostRender(Renderable* r) {
	}

	/**
	 * Invoked before the model-view matrix has been calculated.
	 * It allows one to modify the passed camera transformation before it is used in the calculation
	 * of the model-view transformation.
	 */
	virtual void onPreViewTransform(Renderable* r, Matrix4f& xform) {
	}

	/**
	 * Invoked before the model-view-projection matrix has been calculated.
	 * It allows one to modify the passed model-view transformation before it is used in the calculation
	 * of the model-view-projection transformation.
	 */
	virtual void onPostViewTransform(Renderable* r, Matrix4f& xform) {
	}
};

typedef shared_ptr<RenderListener> RenderListenerPtr;

#endif /* RENDERLISTENER_H_ */
