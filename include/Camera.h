/*
 * Camera.h
 *
 *  Created on: Dec 4, 2009
 *      Author: giorgos
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "Vector.h"
#include "Matrix4.h"

class Camera {
protected:
	Vec3f m_pos;
	Vec3f m_right;
	Vec3f m_up;
	Vec3f m_look;
	Vec3f m_translation;
	Vec3f m_rotation;
	Matrix4f m_view;
	Matrix4f m_proj;
	float m_near;
	float m_far;

public:
	Camera() :
		m_pos(), m_right(Vec3f::X_Axis), m_up(Vec3f::Y_Axis), m_look(Vec3f::Z_Neg_Axis), m_translation(), m_rotation(),
				m_view(), m_proj(), m_near(1.0f), m_far(1000.0f) {
	}

	Camera(const Camera& orig) :
		m_pos(orig.m_pos), m_right(orig.m_right), m_up(orig.m_up), m_look(orig.m_look), m_translation(
				orig.m_translation), m_rotation(orig.m_rotation), m_view(orig.m_view), m_proj(orig.m_proj), m_near(
				orig.m_near), m_far(orig.m_far) {

	}

	virtual ~Camera() {
	}

	/**
	 * Updates the position and basis vectors of the camera and recalculates the viewing matrix.
	 * All invocations to methods that move or rotate the camera after the last updateView call
	 * will take effect now.
	 */
	virtual void updateView();

	virtual void updateProj() = 0;

	virtual void updateViewProj() {
		updateView();
		updateProj();
	}

	/**
	 * All camera movements correspond to opposite movement of the scene, so all methods below (with the exception of up and down)
	 * translate to opposite movements than they names imply.
	 */
	void forward(float dist) {
		m_translation.z += dist;
		//		m_pos += dist * m_look;
	}

	void backward(float dist) {
		m_translation.z -= dist;
		//		m_pos -= dist * m_look;
	}

	Vec3f getLook() const {
		return m_look;
	}

	Vec3f getRight() const {
		return m_right;
	}

	Vec3f getUp() const {
		return m_up;
	}

	/**
	 * Orients the camera to look at the given point.
	 * The focus point's cartesian coordinates are used to calculate the respective spherical coordinates
	 * and the elevation angle is assigned to the camera's yaw angle, while the azimuth angle is assigned
	 * to the camera's pitch angle.
	 *
	 * @param look the position of the focus point
	 */
	void setLook(Vec3f look);

	void setRight(Vec3f right) {
		this->m_right = right;
	}

	void setUp(Vec3f up) {
		this->m_up = up;
	}

	void left(float dist) {
		m_pos -= dist * m_right;
	}

	void right(float dist) {
		m_pos += dist * m_right;
	}

	void up(float dist) {
		m_pos.y += dist;
	}

	void down(float dist) {
		m_pos.y -= dist;
	}

	void turnLeft(float angle) {
		m_rotation.y += angle;
	}

	void turnRight(float angle) {
		m_rotation.y -= angle;
	}

	void lookUp(float angle) {
		m_rotation.x += angle;
	}

	void lookDown(float angle) {
		m_rotation.x -= angle;
	}

	void rollCW(float angle) {
		m_rotation.z -= angle;
	}

	void rollCCW(float angle) {
		m_rotation.z += angle;
	}

	void setFar(float far) {
		this->m_far = far;
	}

	float getFar() const {
		return m_far;
	}

	void setNear(float near) {
		this->m_near = near;
	}

	float getNear() const {
		return m_near;
	}

	void setProj(const Matrix4f& proj) {
		this->m_proj = proj;
	}

	Matrix4f& getProj() {
		return m_proj;
	}

	void setView(const Matrix4f& view) {
		this->m_view = view;
	}

	Matrix4f& getView() {
		return m_view;
	}

	void setPos(float x, float y, float z) {
		this->m_pos.set(x, y, z);
	}

	void setPos(const Vec3f& pos) {
		this->m_pos = pos;
	}

	Vec3f& getPos() {
		return m_pos;
	}

	void setRotation(const Vec3f& rotation) {
		this->m_rotation = rotation;
	}

	Vec3f& getRotation() {
		return m_rotation;
	}
};
typedef shared_ptr<Camera> CameraPtr;

#endif /* CAMERA_H_ */
