/* 
 * File:   PinholeCamera.h
 * Author: giorgos
 *
 * Created on November 13, 2009, 12:02 AM
 */

#ifndef _PINHOLECAMERA_H
#define	_PINHOLECAMERA_H

#include "Camera.h"

/**
 * Implements perspective viewing according the pinhole camera model (i.e. infinitely small lens)
 * The view space has its origin at the camera position, the Right vector is +X, the Up vector
 * is +Y and the Forward vector is -Z.
 */
class PinholeCamera : public Camera {
protected:
	float m_fov;
	float m_aspectRatio;

public:
	PinholeCamera();
	PinholeCamera(const PinholeCamera& orig);
	virtual ~PinholeCamera();



	void updateProj() {
		// calculate the extends of the view on the near plane
		float fovRads = deg_to_rads(m_fov);
		float l = m_near * tan(fovRads / 2.0f);
		float u = m_aspectRatio * l;
		m_proj.identity();
		m_proj.m[0][0] = m_near / l;
		m_proj.m[1][1] = m_near / u;
		m_proj.m[2][2] = -(m_far + m_near) / (m_far - m_near);
		m_proj.m[2][3] = -(2.0f * m_near * m_far) / (m_far - m_near);
		m_proj.m[3][2] = -1.0f;
	}

	void setFov(float fov) {
		this->m_fov = fov;
	}

	float getFov() const {
		return m_fov;
	}

	void setAspectRatio(float aspectRatio) {
		this->m_aspectRatio = aspectRatio;
	}

	float getAspectRatio() const {
		return m_aspectRatio;
	}

};
typedef shared_ptr<PinholeCamera> PinholeCameraPtr;

#endif	/* _PINHOLECAMERA_H */

