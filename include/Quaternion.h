/*
 * Quaternion.h
 *
 *  Created on: Feb 15, 2010
 *      Author: giorgos
 */

#ifndef QUATERNION_H_
#define QUATERNION_H_

#include "Vector.h"
#include "Matrix4.h"

#ifdef USE_EXPORT_KEYWORD
export
#endif
template<typename T>
class Quaternion {

private:
	T m_x, m_y, m_z, m_w;
	T m_angle;
	Vec3<T> m_axis;

public:
	static Quaternion<T> Identity;

	Quaternion(T w, T x, T y, T z);
	Quaternion(T angle, const Vec3<T>& axis);

	/**
	 * Returns the quaternion that will align the normalized input vector src with the normalized
	 * input vector dst.
	 * The implementation is based on Stan Melax's article "The Shortest Arc Quaternion" in Game Programming Gems.
	 * @param src a normalized vector to be aligned with dst
	 * @param dst the target direction
	 * @return a quaternion that when applied to src will align it with dst
	 */
	static Quaternion<T> getRotationArc(const Vec3<T>& src, const Vec3<T>& dst);

	void setFromAngleAxis(T angle, const Vec3<T>& axis);
	void setValues(T w, T x, T y, T z);

	Matrix4f toMatrix() const;

	T squareMagnitude() const {
			return sqrtf(m_w*m_w + m_x*m_x + m_y*m_y + m_z*m_z);
		}

	T magnitude() const {
		return sqrtf(m_w*m_w + m_x*m_x + m_y*m_y + m_z*m_z);
	}

	void normalize() {
		T invMag = T(1.0) / this->magnitude();
		m_w *= invMag;
		m_x *= invMag;
		m_y *= invMag;
		m_z *= invMag;
	}

	Quaternion<T> normalized() const;

	T x() const {
		return m_x;
	}

	T y() const {
		return m_y;
	}

	T z() const {
		return m_z;
	}

	T w() const {
		return m_w;
	}

	T getAngle() const {
		return m_angle;
	}

	const Vec3<T>& getAxis() const {
		return m_axis;
	}
};

typedef Quaternion<float> Quaternionf;

#endif /* QUATERNION_H_ */
