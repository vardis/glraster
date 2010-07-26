#include "GLTutor.h"
#include "Vector.h"
#include "Matrix4.h"

#define APPROX_EQ(a, b, eps) fabs(a - b) < eps

template<typename U>
bool operator==(const Matrix4<U>& m1, const Matrix4<U>& m2) {
return
	APPROX_EQ(m1.m[0][0], m2.m[0][0], 0.00001f) &&
	APPROX_EQ(m1.m[0][1], m2.m[0][1], 0.00001f) &&
	APPROX_EQ(m1.m[0][2], m2.m[0][2], 0.00001f) &&
	APPROX_EQ(m1.m[0][3], m2.m[0][3], 0.00001f) &&

	APPROX_EQ(m1.m[1][0], m2.m[1][0], 0.00001f) &&
	APPROX_EQ(m1.m[1][1], m2.m[1][1], 0.00001f) &&
	APPROX_EQ(m1.m[1][2], m2.m[1][2], 0.00001f) &&
	APPROX_EQ(m1.m[1][3], m2.m[1][3], 0.00001f) &&

	APPROX_EQ(m1.m[2][0], m2.m[2][0], 0.00001f) &&
	APPROX_EQ(m1.m[2][1], m2.m[2][1], 0.00001f) &&
	APPROX_EQ(m1.m[2][2], m2.m[2][2], 0.00001f) &&
	APPROX_EQ(m1.m[2][3], m2.m[2][3], 0.00001f) &&

	APPROX_EQ(m1.m[3][0], m2.m[3][0], 0.00001f) &&
	APPROX_EQ(m1.m[3][1], m2.m[3][1], 0.00001f) &&
	APPROX_EQ(m1.m[3][2], m2.m[3][2], 0.00001f) &&
	APPROX_EQ(m1.m[3][3], m2.m[3][3], 0.00001f);
}

template<typename T>
Matrix4<T>::Matrix4() {
	//memset(m, 0, 16*sizeof(T));
	//m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
}

template<typename T>
Matrix4<T>::Matrix4(T _m[4][4]) {
	memcpy(m, _m, 16 * sizeof(T));
}

template<typename T>
Matrix4<T>::Matrix4(T _r00, T _r01, T _r02, T _r03, T _r10, T _r11, T _r12, T _r13, T _r20, T _r21, T _r22, T _r23,
		T _r30, T _r31, T _r32, T _r33) {
	m[0][0] = _r00;
	m[0][1] = _r01;
	m[0][2] = _r02;
	m[0][3] = _r03;
	m[1][0] = _r10;
	m[1][1] = _r11;
	m[1][2] = _r12;
	m[1][3] = _r13;
	m[2][0] = _r20;
	m[2][1] = _r21;
	m[2][2] = _r22;
	m[2][3] = _r23;
	m[3][0] = _r30;
	m[3][1] = _r31;
	m[3][2] = _r32;
	m[3][3] = _r33;
}

template<typename T>
Matrix4<T>::Matrix4(const Matrix4<T>& _mat) {
	memcpy(m, _mat.m, 16 * sizeof(T));
}

template<typename T>
const Matrix4<T>& Matrix4<T>::operator=(const Matrix4<T>& _mat) {
	memcpy(m, _mat.m, 16 * sizeof(T));
	return *this;
}

template<typename T>
const Matrix4<T>& Matrix4<T>::identity() {
	memset(m, 0, 16 * sizeof(T));
	m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
	return *this;
}

template<typename T>
const Matrix4<T>& Matrix4<T>::transpose() {

	std::swap(m[0][1], m[1][0]);
	std::swap(m[0][2], m[2][0]);
	std::swap(m[0][3], m[3][0]);

	std::swap(m[1][2], m[2][1]);
	std::swap(m[1][3], m[3][1]);

	std::swap(m[2][3], m[3][2]);

	return *this;
}

template<typename T>
Matrix4<T> Matrix4<T>::transposed() const {
	return Matrix4<T> (m[0][0], m[1][0], m[2][0], m[3][0], m[0][1], m[1][1], m[2][1], m[3][1], m[0][2], m[1][2],
			m[2][2], m[3][2], m[0][3], m[1][3], m[2][3], m[3][3]);
}

template<typename T>
const Matrix4<T>& Matrix4<T>::inverse() {
	*this = this->inversed();
	return *this;
}

template<typename T>
Matrix4<T> Matrix4<T>::inversed() const {

	int indxc[4], indxr[4];
	int ipiv[4] = { 0, 0, 0, 0 };
	T minv[4][4];

	memcpy(minv, m, 16 * sizeof(T));

	for (int i = 0; i < 4; i++) {
		int irow = -1, icol = -1;
		T big = 0.;
		// Choose pivot
		for (int j = 0; j < 4; j++) {
			if (ipiv[j] != 1) {
				for (int k = 0; k < 4; k++) {
					if (ipiv[k] == 0) {
						if (fabsf(minv[j][k]) >= big) {
							big = T(fabsf(minv[j][k]));
							irow = j;
							icol = k;
						}
					} else if (ipiv[k] > 1) {
						std::cerr << "Singular matrix in MatrixInvert";
						return Matrix4<T> ();
					}
				}
			}
		}
		++ipiv[icol];
		// Swap rows _irow_ and _icol_ for pivot
		if (irow != icol) {
			for (int k = 0; k < 4; ++k) {
				std::swap(minv[irow][k], minv[icol][k]);
			}
		}
		indxr[i] = irow;
		indxc[i] = icol;
		if (minv[icol][icol] == 0.) {
			std::cerr << "Singular matrix in MatrixInvert";
			return Matrix4<T> ();
		}
		// Set $m[icol][icol]$ to one by scaling row _icol_ appropriately
		T pivinv = 1.f / minv[icol][icol];
		minv[icol][icol] = 1.f;
		for (int j = 0; j < 4; j++) {
			minv[icol][j] *= pivinv;
		}
		// Subtract this row from others to zero out their columns
		for (int j = 0; j < 4; j++) {
			if (j != icol) {
				T save = minv[j][icol];
				minv[j][icol] = 0;
				for (int k = 0; k < 4; k++) {
					minv[j][k] -= minv[icol][k] * save;
				}
			}
		}
	}
	// Swap columns to reflect permutation
	for (int j = 3; j >= 0; j--) {
		if (indxr[j] != indxc[j]) {
			for (int k = 0; k < 4; k++) {
				std::swap(minv[k][indxr[j]], minv[k][indxc[j]]);
			}
		}
	}
	return Matrix4<T> (minv);
}

template<typename T>
Matrix4<T> Matrix4<T>::operator*(const Matrix4<T>& _mat) const {

	T r[4][4];
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			r[i][j] = m[i][0] * _mat.m[0][j] + m[i][1] * _mat.m[1][j] + m[i][2] * _mat.m[2][j] + m[i][3] * _mat.m[3][j];
		}
	}
	return Matrix4<T> (r);
}

template<typename T>
Vec4<T> Matrix4<T>::operator*(const Vec4<T>& _v) const {
	return Vec4<T> (
			m[0][0] * _v.x + m[0][1] * _v.y + m[0][2] * _v.z + m[0][3] * _v.w,
			m[1][0] * _v.x + m[1][1] * _v.y + m[1][2] * _v.z + m[1][3] * _v.w,
			m[2][0] * _v.x + m[2][1] * _v.y + m[2][2] * _v.z + m[2][3] * _v.w,
			m[3][0] * _v.x + m[3][1] * _v.y + m[3][2] * _v.z + m[3][3] * _v.w);
}

template<typename T>
Vec3<T> Matrix4<T>::operator*(const Vec3<T>& _v) const {
	return Vec3<T> (m[0][0] * _v.x + m[0][1] * _v.y + m[0][2] * _v.z, m[1][0] * _v.x + m[1][1] * _v.y + m[1][2] * _v.z,
			m[2][0] * _v.x + m[2][1] * _v.y + m[2][2] * _v.z);
}

template<typename T>
T Matrix4<T>::operator()(uint row, uint column) const {
	assert(row < 4 && column < 4);
	return m[row][column];
}

template<typename T>
Matrix4<T> Matrix4<T>::FromBasis(const Vec3<T>& _x, const Vec3<T>& _y, const Vec3<T>& _z) {
	return Matrix4<T> (_x.x, _y.x, _z.x, 0.0f, _x.y, _y.y, _z.y, 0.0f, _x.z, _y.z, _z.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

template<typename T>
Matrix4<T> Matrix4<T>::Translation(const Vec3<T>& _v) {
	return Matrix4<T> (1.0f, 0.0f, 0.0f, _v.x, 0.0f, 1.0f, 0.0f, _v.y, 0.0f, 0.0f, 1.0f, _v.z, 0.0f, 0.0f, 0.0f, 1.0f);
}

template<typename T>
Matrix4<T> Matrix4<T>::Translation(T _x, T _y, T _z) {
	return Matrix4<T> (1.0f, 0.0f, 0.0f, _x, 0.0f, 1.0f, 0.0f, _y, 0.0f, 0.0f, 1.0f, _z, 0.0f, 0.0f, 0.0f, 1.0f);
}

template<typename T>
Matrix4<T> Matrix4<T>::Scale(const Vec3<T>& _v) {
	return Matrix4<T> (_v.x, 0.0f, 0.0f, 0.0f, 0.0f, _v.y, 0.0f, 0.0f, 0.0f, 0.0f, _v.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

template<typename T>
Matrix4<T> Matrix4<T>::Scale(T _x, T _y, T _z) {
	return Matrix4<T> (_x, 0.0f, 0.0f, 0.0f, 0.0f, _y, 0.0f, 0.0f, 0.0f, 0.0f, _z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

template<typename T>
Matrix4<T> Matrix4<T>::Rotation(const Vec3<T>& _axis, T _angle) {

	Vec3<T> a = _axis.normalized();
	T s = sinf(_angle);
	T c = cosf(_angle);

	T m[4][4];
	m[0][0] = a.x * a.x + (1.f - a.x * a.x) * c;
	m[0][1] = a.x * a.y * (1.f - c) - a.z * s;
	m[0][2] = a.x * a.z * (1.f - c) + a.y * s;
	m[0][3] = 0;
	m[1][0] = a.x * a.y * (1.f - c) + a.z * s;
	m[1][1] = a.y * a.y + (1.f - a.y * a.y) * c;
	m[1][2] = a.y * a.z * (1.f - c) - a.x * s;
	m[1][3] = 0;
	m[2][0] = a.x * a.z * (1.f - c) - a.y * s;
	m[2][1] = a.y * a.z * (1.f - c) + a.x * s;
	m[2][2] = a.z * a.z + (1.f - a.z * a.z) * c;
	m[2][3] = 0;
	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;
	m[3][3] = 1;

	return Matrix4<T> (m);
}

template<typename T>
Matrix4<T> Matrix4<T>::RotationX(T _angle) {
	T sin_x = sinf(_angle);
	T cos_x = cosf(_angle);
	return Matrix4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, cos_x, -sin_x, 0.0f, 0.0f, sin_x, cos_x, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

template<typename T>
Matrix4<T> Matrix4<T>::RotationY(T _angle) {
	T sin_y = sinf(_angle);
	T cos_y = cosf(_angle);
	return Matrix4(cos_y, 0.0f, sin_y, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -sin_y, 0.0f, cos_y, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}

template<typename T>
Matrix4<T> Matrix4<T>::RotationZ(T _angle) {
	T sin_z = sinf(_angle);
	T cos_z = cosf(_angle);
	return Matrix4(cos_z, -sin_z, 0.0f, 0.0f, sin_z, cos_z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
}




#ifndef USE_EXPORT_KEYWORD

template
bool operator==(const Matrix4<float>& m1, const Matrix4<float>& m2);

template
bool operator==(const Matrix4<double>& m1, const Matrix4<double>& m2);

template class Matrix4<float> ;
template class Matrix4<double> ;
#endif
