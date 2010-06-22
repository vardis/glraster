#ifndef Matrix3_H_
#define Matrix3_H_

#include <iostream>
#include "Vector.h"

#ifdef USE_EXPORT_KEYWORD
export
#endif
template<typename T>
class Matrix3 {
public:
	T m[3][3];

public:
	Matrix3();
	Matrix3(T _m[3][3]);
	Matrix3(T _r00, T _r01, T _r02, T _r10, T _r11, T _r12, T _r20, T _r21, T _r22);
	Matrix3(const Matrix3<T>& _mat);

	/**
	 * Returns a pointer to the rgba data. This is used to interface with GLSL uniform setters.
	 * @see GLSLProgram
	 */
	T* ptr() {
		return &m[0][0];
	}

	const Matrix3<T>& operator=(const Matrix3<T>& _mat);

	const Matrix3<T>& identity();

	const Matrix3<T>& transpose();
	Matrix3<T> transposed() const;

	const Matrix3<T>& inverse();
	Matrix3<T> inversed() const;

	Matrix3<T> operator*(const Matrix3<T>& _mat) const;
	Vec3<T> operator*(const Vec3<T>& _v) const;

	Vec3<T> x() const {
		return Vec3<T> (m[0][0], m[1][0], m[2][0]);
	}

	Vec3<T> y() const {
		return Vec3<T> (m[0][1], m[1][1], m[2][1]);
	}

	Vec3<T> z() const {
		return Vec3<T> (m[0][2], m[1][2], m[2][2]);
	}

	void setX(const Vec3<T>& _x) {
		m[0][0] = _x.x;
		m[1][0] = _x.y;
		m[2][0] = _x.z;
	}

	void setY(const Vec3<T>& _y) {
		m[0][1] = _y.x;
		m[1][1] = _y.y;
		m[2][1] = _y.z;
	}

	void setZ(const Vec3<T>& _z) {
		m[0][2] = _z.x;
		m[1][2] = _z.y;
		m[2][2] = _z.z;
	}



	static Matrix3<T> FromBasis(const Vec3<T>& _x, const Vec3<T>& _y, const Vec3<T>& _z);

	static Matrix3<T> Scale(const Vec3<T>& _v);
	static Matrix3<T> Scale(T _x, T _y, T _z);

	static Matrix3<T> Rotation(const Vec3<T>& _axis, T _angle);
	static Matrix3<T> RotationX(T _angle);
	static Matrix3<T> RotationY(T _angle);
	static Matrix3<T> RotationZ(T _angle);

	template<typename U>
	friend bool operator==(const Matrix3<U>& m1, const Matrix3<U>& m2);
};


typedef Matrix3<float> Matrix3f;
typedef Matrix3<double> Matrix3d;

#endif /*Matrix3_H_*/
