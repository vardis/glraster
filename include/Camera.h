/*
 * Camera.h
 *
 *  Created on: Dec 4, 2009
 *      Author: giorgos
 */

#ifndef CAMERA_H_
#define CAMERA_H_

class Camera {
public:
	Camera();
	virtual ~Camera();

	virtual void updateView() = 0;
	virtual void updateProj() = 0;
	virtual void updateViewProj() = 0;
	virtual void setProj(const Matrix4f& proj) = 0;

	virtual Matrix4f& getProj() = 0;

	virtual void setView(const Matrix4f& view) = 0;

	virtual Matrix4f& getView() = 0;

	virtual void setPos(const Vec3f& pos) = 0;

	virtual Vec3f& getPos() = 0;

	virtual void setAspectRatio(float aspectRatio) = 0;

	virtual float getAspectRatio() = 0;
};
typedef shared_ptr<Camera> CameraPtr;

#endif /* CAMERA_H_ */
