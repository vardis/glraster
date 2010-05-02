/*
 * OrthographicCamera.h
 *
 *  Created on: Mar 13, 2010
 *      Author: giorgos
 */

#ifndef ORTHOGRAPHICCAMERA_H_
#define ORTHOGRAPHICCAMERA_H_

#include "Camera.h"

class OrthographicCamera : public Camera {
protected:
	float m_width;
	float m_height;

public:
	OrthographicCamera();
	OrthographicCamera(const OrthographicCamera& other);
	virtual ~OrthographicCamera();

	virtual void updateView() {
		m_view.identity();
	}
	virtual void updateProj() {
		m_proj.identity();
		m_proj.m[0][0] = 2.0f / m_width;
		m_proj.m[1][1] = 2.0f / m_height;
		m_proj.m[2][2] = -2.0f / (m_far - m_near);
		m_proj.m[3][2] = -(m_near + m_far) / (m_far - m_near);
	}

	float getHeight() const {
		return m_height;
	}

	float getWidth() const {
		return m_width;
	}

	void setHeight(float height) {
		this->m_height = height;
	}

	void setWidth(float width) {
		this->m_width = width;
	}
};

typedef shared_ptr<OrthographicCamera> OrthographicCameraPtr;

#endif /* ORTHOGRAPHICCAMERA_H_ */
