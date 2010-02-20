/*
 * Quaternion.cpp
 *
 *  Created on: Feb 15, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "Quaternion.h"

template <typename T>
Quaternion<T> Quaternion<T>::Identity(T(1.0), T(0.0), T(0.0), T(0.0));

template <typename T>
Quaternion<T>::Quaternion(T w, T x, T y, T z) {
	this->setValues(w, x, y, z);
}

template <typename T>
Quaternion<T>::Quaternion(T angle, const Vec3<T>& axis) {
	this->setFromAngleAxis(angle, axis);
}

template <typename T>
Quaternion<T> Quaternion<T>::getRotationArc(const Vec3<T>& src, const Vec3<T>& dst) {
	Vec3<T> c = src.cross(src);
	T d = src.dot(dst);
	T s = T(1.0) / (T) sqrtf(T(2) * (T(1) + d));
	return Quaternion(c.x * s, c.y * s, c.z * s, T(1.0) / (T(2.0) * s));
}

template <typename T>
void Quaternion<T>::setFromAngleAxis(T angle, const Vec3<T>& axis) {
	m_angle = angle;
	m_axis = axis;
	m_w = cosf(angle / T(2.0));

	T sinTheta = sinf(angle / T(2.0));
	m_x = axis.x * sinTheta;
	m_y = axis.y * sinTheta;
	m_z = axis.z * sinTheta;
}

template <typename T>
void Quaternion<T>::setValues(T w, T x, T y, T z) {
	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;
	m_angle = T(2.0) * acos(w);

	T invSqLength = T(1.0) / (x*x + y*y + z*z);
	m_axis.set(m_x * invSqLength, m_y * invSqLength, m_z * invSqLength);
}

template <typename T>
Matrix4f Quaternion<T>::toMatrix() const {
	Matrix4f m;
	float xy = 2.0f * m_x * m_y;
	float xz = 2.0f * m_x * m_z;
	float wz = 2.0f * m_w * m_z;
	float wx = 2.0f * m_w * m_x;
	float yz = 2.0f * m_y * m_z;
	float wy = 2.0f * m_w * m_y;
	float y2 = 2.0f * m_y * m_y;
	float x2 = 2.0f * m_x * m_x;
	float z2 = 2.0f * m_z * m_z;
	m.m[0][0] = 1.0f - y2 - z2;
	m.m[0][1] = xy + wz;
	m.m[0][2] = xz + wy;
	m.m[0][3] = 0.0f;
	m.m[1][0] = xy + wz;
	m.m[1][1] = 1.0f - x2 - z2;
	m.m[1][2] = yz - wx;
	m.m[1][3] = 0.0f;
	m.m[2][0] = xz - wy;
	m.m[2][1] = yz + wx;
	m.m[2][2] = 1.0f - x2- y2;
	m.m[2][3] = 0.0f;
	m.m[3][0] = 0.0f;
	m.m[3][1] = 0.0f;
	m.m[3][2] = 0.0f;
	m.m[3][3] = 1.0f;
	return m;
}

#ifndef USE_EXPORT_KEYWORD
// explicit template instanciations
template class Quaternion<float>;
#endif

