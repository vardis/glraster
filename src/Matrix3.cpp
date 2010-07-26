#include "GLTutor.h"
#include "Vector.h"
#include "Matrix3.h"

#define APPROX_EQ(a, b, eps) fabs(a - b) < eps

template<typename U>
bool operator==(const Matrix3<U>& m1, const Matrix3<U>& m2) {
	return APPROX_EQ(m1.m[0][0], m2.m[0][0], 0.00001f) && APPROX_EQ(m1.m[0][1], m2.m[0][1], 0.00001f)
			&& APPROX_EQ(m1.m[0][2], m2.m[0][2], 0.00001f) &&

	APPROX_EQ(m1.m[1][0], m2.m[1][0], 0.00001f) && APPROX_EQ(m1.m[1][1], m2.m[1][1], 0.00001f)
			&& APPROX_EQ(m1.m[1][2], m2.m[1][2], 0.00001f) &&

	APPROX_EQ(m1.m[2][0], m2.m[2][0], 0.00001f) && APPROX_EQ(m1.m[2][1], m2.m[2][1], 0.00001f)
			&& APPROX_EQ(m1.m[2][2], m2.m[2][2], 0.00001f);
}

template<typename T>
Matrix3<T>::Matrix3() {
	//memset(m, 0, 16*sizeof(T));
	//m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
}

template<typename T>
Matrix3<T>::Matrix3(T _m[3][3]) {
	memcpy(m, _m, 9 * sizeof(T));
}

template<typename T>
Matrix3<T>::Matrix3(T _r00, T _r01, T _r02, T _r10, T _r11, T _r12, T _r20, T _r21, T _r22) {
	m[0][0] = _r00;
	m[0][1] = _r01;
	m[0][2] = _r02;
	m[1][0] = _r10;
	m[1][1] = _r11;
	m[1][2] = _r12;
	m[2][0] = _r20;
	m[2][1] = _r21;
	m[2][2] = _r22;
}

template<typename T>
Matrix3<T>::Matrix3(const Matrix3<T>& _mat) {
	memcpy(m, _mat.m, 9 * sizeof(T));
}

template<typename T>
const Matrix3<T>& Matrix3<T>::operator=(const Matrix3<T>& _mat) {
	memcpy(m, _mat.m, 9 * sizeof(T));
	return *this;
}

template<typename T>
const Matrix3<T>& Matrix3<T>::identity() {
	memset(m, 0, 9 * sizeof(T));
	m[0][0] = m[1][1] = m[2][2] = 1.0f;
	return *this;
}

template<typename T>
const Matrix3<T>& Matrix3<T>::transpose() {

	std::swap(m[0][1], m[1][0]);
	std::swap(m[0][2], m[2][0]);
	std::swap(m[1][2], m[2][1]);
	return *this;
}

template<typename T>
Matrix3<T> Matrix3<T>::transposed() const {
	return Matrix3<T> (m[0][0], m[1][0], m[2][0], m[0][1], m[1][1], m[2][1], m[0][2], m[1][2], m[2][2]);
}

template<typename T>
const Matrix3<T>& Matrix3<T>::inverse() {
	*this = this->inversed();
	return *this;
}

// Taken from blender 3D source code
template<typename T>
Matrix3<T> Matrix3<T>::inversed() const {
	// calculate the adjoint matrix in adjoint
	T adjoint[3][3];
	adjoint[0][0] = m[1][1] * m[2][2] - m[1][2] * m[2][1];
	adjoint[0][1] = -m[0][1] * m[2][2] + m[0][2] * m[2][1];
	adjoint[0][2] = m[0][1] * m[1][2] - m[0][2] * m[1][1];

	adjoint[1][0] = -m[1][0] * m[2][2] + m[1][2] * m[2][0];
	adjoint[1][1] = m[0][0] * m[2][2] - m[0][2] * m[2][0];
	adjoint[1][2] = -m[0][0] * m[1][2] + m[0][2] * m[1][0];

	adjoint[2][0] = m[1][0] * m[2][1] - m[1][1] * m[2][0];
	adjoint[2][1] = -m[0][0] * m[2][1] + m[0][1] * m[2][0];
	adjoint[2][2] = m[0][0] * m[1][1] - m[0][1] * m[1][0];

	T det= m[0][0]* (m[1][1]*m[2][2] - m[1][2]*m[2][1])
	    -m[1][0]* (m[0][1]*m[2][2] - m[0][2]*m[2][1])
	    +m[2][0]* (m[0][1]*m[1][2] - m[0][2]*m[1][1]);

	if (det == 0) {
		det = T(1);
	}

	det = T(1) / det;

	for (int a = 0; a < 3; a++) {
		for(int b = 0; b < 3; b++) {
			adjoint[a][b] *= det;
		}
	}
	return Matrix3<T> (adjoint);
}

template<typename T>
Matrix3<T> Matrix3<T>::operator*(const Matrix3<T>& _mat) const {

	T r[3][3];
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			r[i][j] = m[i][0] * _mat.m[0][j] + m[i][1] * _mat.m[1][j] + m[i][2] * _mat.m[2][j];
		}
	}
	return Matrix3<T> (r);
}

template<typename T>
Vec3<T> Matrix3<T>::operator*(const Vec3<T>& _v) const {
	return Vec3<T> (m[0][0] * _v.x + m[0][1] * _v.y + m[0][2] * _v.z, m[1][0] * _v.x + m[1][1] * _v.y + m[1][2] * _v.z,
			m[2][0] * _v.x + m[2][1] * _v.y + m[2][2] * _v.z);
}

template<typename T>
T Matrix3<T>::operator()(uint row, uint column) const {
	assert(row < 3 && column < 3);
	return m[row][column];
}

template<typename T>
Matrix3<T> Matrix3<T>::FromBasis(const Vec3<T>& _x, const Vec3<T>& _y, const Vec3<T>& _z) {
	return Matrix3<T> (_x.x, _y.x, _z.x, _x.y, _y.y, _z.y, _x.z, _y.z, _z.z);
}

template<typename T>
Matrix3<T> Matrix3<T>::Scale(const Vec3<T>& _v) {
	return Matrix3<T> (_v.x, 0.0f, 0.0f, 0.0f, _v.y, 0.0f, 0.0f, 0.0f, _v.z);
}

template<typename T>
Matrix3<T> Matrix3<T>::Scale(T _x, T _y, T _z) {
	return Matrix3<T> (_x, 0.0f, 0.0f, 0.0f, _y, 0.0f, 0.0f, 0.0f, _z);
}

template<typename T>
Matrix3<T> Matrix3<T>::Rotation(const Vec3<T>& _axis, T _angle) {

	Vec3<T> a = _axis.normalized();
	T s = sinf(_angle);
	T c = cosf(_angle);

	T m[3][3];
	m[0][0] = a.x * a.x + (1.f - a.x * a.x) * c;
	m[0][1] = a.x * a.y * (1.f - c) - a.z * s;
	m[0][2] = a.x * a.z * (1.f - c) + a.y * s;
	m[1][0] = a.x * a.y * (1.f - c) + a.z * s;
	m[1][1] = a.y * a.y + (1.f - a.y * a.y) * c;
	m[1][2] = a.y * a.z * (1.f - c) - a.x * s;
	m[2][0] = a.x * a.z * (1.f - c) - a.y * s;
	m[2][1] = a.y * a.z * (1.f - c) + a.x * s;
	m[2][2] = a.z * a.z + (1.f - a.z * a.z) * c;

	return Matrix3<T> (m);
}

template<typename T>
Matrix3<T> Matrix3<T>::RotationX(T _angle) {
	T sin_x = sinf(_angle);
	T cos_x = cosf(_angle);
	return Matrix3(1.0f, 0.0f, 0.0f, 0.0f, cos_x, -sin_x, 0.0f, sin_x, cos_x);
}

template<typename T>
Matrix3<T> Matrix3<T>::RotationY(T _angle) {
	T sin_y = sinf(_angle);
	T cos_y = cosf(_angle);
	return Matrix3(cos_y, 0.0f, sin_y, 0.0f, 1.0f, 0.0f, -sin_y, 0.0f, cos_y);
}

template<typename T>
Matrix3<T> Matrix3<T>::RotationZ(T _angle) {
	T sin_z = sinf(_angle);
	T cos_z = cosf(_angle);
	return Matrix3(cos_z, -sin_z, 0.0f, sin_z, cos_z, 0.0f, 0.0f, 0.0f, 1.0f);
}

#ifndef USE_EXPORT_KEYWORD

template
bool operator==(const Matrix3<float>& m1, const Matrix3<float>& m2);

template
bool operator==(const Matrix3<double>& m1, const Matrix3<double>& m2);

template class Matrix3<float> ;
template class Matrix3<double> ;
#endif
