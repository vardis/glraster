/* 
 * File:   PinholeCamera.h
 * Author: giorgos
 *
 * Created on November 13, 2009, 12:02 AM
 */

#ifndef _PINHOLECAMERA_H
#define	_PINHOLECAMERA_H

#include "Vector.h"
#include "Matrix4.h"

/**
 * Implements perspective viewing according the pinhole camera model (i.e. infinitely small lens)
 * The view space has its origin at the camera position, the Right vector is +X, the Up vector
 * is +Y and the Forward vector is -Z.
 */
class PinholeCamera {
public:
	PinholeCamera();
	PinholeCamera(const PinholeCamera& orig);
	virtual ~PinholeCamera();

	void updateView() {
		m_rotation.x = wrap(m_rotation.x, -90.0f, 90.0f);
		m_rotation.y = wrap(m_rotation.y, -360.0f, 360.0f);
		m_rotation.z = wrap(m_rotation.z, -360.0f, 360.0f);

		float yawRads = deg_to_rads(m_rotation.y);
		yawRads = clamp(yawRads, -2.0f * M_PI, 2.0f * M_PI);
		float pitchRads = deg_to_rads(m_rotation.x);
		pitchRads = clamp(pitchRads, -M_PI_2, M_PI_2);
		float rollRads = deg_to_rads(m_rotation.z);
		rollRads = clamp(rollRads, -2.0f * M_PI, 2.0f * M_PI);
		Matrix4f rotY = Matrix4f::RotationY(yawRads);
		Matrix4f rotX = Matrix4f::RotationX(pitchRads);
		Matrix4f tr = Matrix4f::Translation(-m_pos.x, -m_pos.y, -m_pos.z);
		Matrix4f basis = Matrix4f::FromBasis(Vec3f(1,0,0), Vec3f(0,1,0), Vec3f(0,0,-1));
		m_view = rotX * rotY * tr;

		m_look = m_view.z();
		m_look.normalize();
		m_look.z *= -1.0f;
		m_right = m_look.cross(Vec3f(0,1,0));
		m_right.normalize();
		m_up = m_look.cross(m_right);
		m_up.normalize();
	}

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

	void updateViewProj() {
		updateView();
		updateProj();
	}

	/**
	 * All camera movements correspond to opposite movement of the scene, so all methods below (with the exception of up and down)
	 * translate to opposite movements than they names imply.
	 */
	void forward(float dist) {
		m_pos += dist * m_look;
	}

	void backward(float dist) {
		m_pos -= dist * m_look;
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

	void setLook(Vec3f m_look) {
		this->m_look = m_look;
	}

	void setRight(Vec3f m_right) {
		this->m_right = m_right;
	}

	void setUp(Vec3f m_up) {
		this->m_up = m_up;
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
		m_rotation.y -= angle;
	}

	void turnRight(float angle) {
		m_rotation.y += angle;
	}

	void lookUp(float angle) {
		m_rotation.x -= angle;
	}

	void lookDown(float angle) {
		m_rotation.x += angle;
	}

	void rollCW(float angle) {
		m_rotation.z += angle;
	}

	void rollCCW(float angle) {
		m_rotation.z -= angle;
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

	void setFov(float fov) {
		this->m_fov = fov;
	}

	float getFov() const {
		return m_fov;
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

	void setPos(const Vec3f& pos) {
		this->m_pos = pos;
	}

	Vec3f& getPos() {
		return m_pos;
	}

	void setAspectRatio(float aspectRatio) {
		this->m_aspectRatio = aspectRatio;
	}

	float getAspectRatio() const {
		return m_aspectRatio;
	}

	void setRotation(const Vec3f& rotation) {
		this->m_rotation = rotation;
	}

	Vec3f& getRotation() {
		return m_rotation;
	}

private:
	Vec3f m_pos;
	Vec3f m_right;
	Vec3f m_up;
	Vec3f m_look;
	Vec3f m_rotation;
	Matrix4f m_view;
	Matrix4f m_proj;
	float m_fov;
	float m_near;
	float m_far;
	float m_aspectRatio;
};
typedef shared_ptr<PinholeCamera> PinholeCameraPtr;

#endif	/* _PINHOLECAMERA_H */

