/*
 * Camera.cpp
 *
 *  Created on: Apr 25, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "Camera.h"

void Camera::updateView() {
	m_rotation.x = wrap(m_rotation.x, -90.0f, 90.0f);
	m_rotation.y = wrap(m_rotation.y, -360.0f, 360.0f);
	m_rotation.z = wrap(m_rotation.z, -360.0f, 360.0f);
	//
	float yawRads = deg_to_rads(m_rotation.y);
	yawRads = clamp(yawRads, -2.0f * M_PI, 2.0f * M_PI);

	float pitchRads = deg_to_rads(m_rotation.x);
	pitchRads = clamp(pitchRads, -M_PI_2, M_PI_2);

	float rollRads = deg_to_rads(m_rotation.z);
	rollRads = clamp(rollRads, -2.0f * M_PI, 2.0f * M_PI);

	Matrix4f rotY = Matrix4f::RotationY(yawRads);
	Matrix4f rotX = Matrix4f::Rotation(m_right, pitchRads);

	m_look = rotX * rotY * Vec3f::Z_Axis;
	m_up = rotX * m_up;
	m_right = rotY * Vec3f::X_Axis;

	// translate camera
	m_pos += m_translation.x * m_right + m_translation.y * m_up + m_translation.z * m_look;

	// ortho-normalize basis vectors
	m_look.normalize();
	m_up = Vec3f::Y_Axis - (Vec3f::Y_Axis * m_look) * m_look;
	m_up.normalize();
	m_right = m_look.cross(m_up);
	m_right.normalize();

	m_up = m_right.cross(m_look);
	m_up.normalize();

	// m_view is the inverse of the camera transformation, transforms from
	m_view = Matrix4f::FromBasis(m_right, m_up, -m_look);
	m_view.transpose();

	Vec3f p = m_view * m_pos;
	m_view.setW(-p);

	m_translation.set(0.0f, 0.0f, 0.0f);
}

void Camera::setLook(Vec3f look) {
	Vec3f fwd = look - m_pos;
	fwd.normalize();

	float theta = acos(fwd.z);
	float phi = atan2(fwd.y, fwd.x);
	m_rotation.x = rads_to_deg(phi);
	m_rotation.y = rads_to_deg(theta);
	m_rotation.z = 0.0f;

	updateView();
}
