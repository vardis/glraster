#ifndef MATRIX4_H_
#define MATRIX4_H_

#ifdef USE_EXPORT_KEYWORD
export
#endif
template<typename T>
class Matrix4 {
public:
	T m[4][4];

public:
	Matrix4();
	Matrix4(T _m[4][4]);
	Matrix4(T _r00, T _r01, T _r02, T _r03, T _r10, T _r11, T _r12, T _r13, T _r20, T _r21, T _r22, T _r23, T _r30,
			T _r31, T _r32, T _r33);
	Matrix4(const Matrix4<T>& _mat);

	const Matrix4<T>& operator=(const Matrix4<T>& _mat);

	const Matrix4<T>& identity();

	const Matrix4<T>& transpose();
	Matrix4<T> transposed() const;

	const Matrix4<T>& inverse();
	Matrix4<T> inversed() const;

	Matrix4<T> operator*(const Matrix4<T>& _mat) const;
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

	Vec3<T> w() const {
		return Vec3<T> (m[0][3], m[1][3], m[2][3]);
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

	void setW(const Vec3<T>& _w) {
		m[0][3] = _w.x;
		m[1][3] = _w.y;
		m[2][3] = _w.z;
	}

	static Matrix4<T> FromBasis(const Vec3<T>& _x, const Vec3<T>& _y, const Vec3<T>& _z);

	static Matrix4<T> Translation(const Vec3<T>& _v);
	static Matrix4<T> Translation(T _x, T _y, T _z);

	static Matrix4<T> Scale(const Vec3<T>& _v);
	static Matrix4<T> Scale(T _x, T _y, T _z);

	static Matrix4<T> Rotation(const Vec3<T>& _axis, T _angle);
	static Matrix4<T> RotationX(T _angle);
	static Matrix4<T> RotationY(T _angle);
	static Matrix4<T> RotationZ(T _angle);

	static Matrix4<T> RotationEuler(T _yaw, T _pitch, T _roll);
};

typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

#endif /*MATRIX4_H_*/
