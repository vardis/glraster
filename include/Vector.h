#ifndef VECTOR_H_
#define VECTOR_H_

#ifdef USE_EXPORT_KEYWORD
export
#endif
template<typename T>
class Vec2 {

public:
	T coords[3];

	/** references coords[0] */
	T& x;

	/** references coords[1] */
	T& y;

	Vec2() : x(coords[0]), y(coords[1]) {
		coords[0] = coords[1] =  0.0f;
	}

	Vec2(T _v) :
		x(coords[0]), y(coords[1]) {
		coords[0] = coords[1] =  _v;
	}

	Vec2(T _x, T _y) :
		x(coords[0]), y(coords[1]) {
		coords[0] = _x;
		coords[1] = _y;
	}

	Vec2(const Vec2<T>& _v) :
		x(coords[0]), y(coords[1]) {
		coords[0] = _v.x;
		coords[1] = _v.y;
	}

	T* ptr() {
		return &coords[0];
	}

	void get(float& _x, float& _y) const {
		_x = x;
		_y = y;
	}

	void set(float _x, float _y) {
		x = _x;
		y = _y;
	}

	T length() const {
		return sqrt(x * x + y * y);
	}

	T squaredLength() const {
		return (x * x + y * y);
	}

	T dot(const Vec2<T>& _v) const {
		return x * _v.x + y * _v.y;
	}

	T dot(T _x, T _y) const {
		return x * _x + y * _y;
	}

	void negate() {
		x *= -1;
		y *= -1;
	}

	void normalize() {
		T l = this->length();
		if (l > 0.0000001) {
			T rl = 1.0 / l;
			x *= rl;
			y *= rl;
		}
	}

	Vec2& operator=(const Vec2<T>& _v) {
		x = _v.x;
		y = _v.y;
		return *this;
	}

	Vec2 operator+(const Vec2<T>& _v) const {
		return Vec2(x + _v.x, y + _v.y);
	}

	Vec2 operator-(const Vec2<T>& _v) const {
		return Vec2(x - _v.x, y - _v.y);
	}

	void operator+=(const Vec2<T>& _v) {
		x += _v.x;
		y += _v.y;
	}

	void operator-=(const Vec2<T>& _v) {
		x -= _v.x;
		y -= _v.y;
	}

	Vec2 operator*(const Vec2<T>& _v) const {
		return Vec2(x * _v.x, y * _v.y);
	}

	Vec2 operator*(T _factor) const {
		return Vec2(x * _factor, y * _factor);
	}

	Vec2 operator*=(T _factor) {
		x *= _factor;
		y *= _factor;
		return *this;
	}

	float operator[](uint8_t i) const {
		if (i) {
			return x;
		} else {
			return y;
		}
	}
};

typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;
typedef Vec2<double> Vec2d;


/*---------------------- Vec3 -----------------*/

#ifdef USE_EXPORT_KEYWORD
export
#endif
template<typename T>
class Vec3 {

public:
	T coords[3];

	/** references coords[0] */
	T& x;

	/** references coords[1] */
	T& y;

	/** references coords[2] */
	T& z;

	static const Vec3<T> Zero;
	static const Vec3<T> Front;

	static const Vec3<T> X_Axis;
	static const Vec3<T> Y_Axis;
	static const Vec3<T> Z_Axis;

	static const Vec3<T> X_Neg_Axis;
	static const Vec3<T> Y_Neg_Axis;
	static const Vec3<T> Z_Neg_Axis;

	Vec3() :
		x(coords[0]), y(coords[1]), z(coords[2]) {
		coords[0] = coords[1] = coords[2] = 0.0f;
	}

	explicit Vec3(T _v) :
		x(coords[0]), y(coords[1]), z(coords[2]) {
		coords[0] = coords[1] = coords[2] = _v;
	}

	Vec3(T _x, T _y, T _z) :
		x(coords[0]), y(coords[1]), z(coords[2]) {
		coords[0] = _x;
		coords[1] = _y;
		coords[2] = _z;
	}

	Vec3(const Vec3<T>& _v) :
		x(coords[0]), y(coords[1]), z(coords[2]) {
		coords[0] = _v.coords[0];
		coords[1] = _v.coords[1];
		coords[2] = _v.coords[2];
	}

	T* ptr() {
		return &coords[0];
	}

	void get(T& _x, T& _y, T& _z) const {
		_x = x;
		_y = y;
		_z = z;
	}

	void set(T _x, T _y, T _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	T length() const {
		return sqrt(x * x + y * y + z * z);
	}

	T squaredLength() const {
		return (x * x + y * y + z * z);
	}

	T dot(const Vec3<T>& _v) const {
		return x * _v.x + y * _v.y + z * _v.z;
	}

	T dot(T _x, T _y, T _z) const {
		return x * _x + y * _y + z * _z;
	}

	void negate() {
		x *= -1;
		y *= -1;
		z *= -1;
	}

	void normalize() {
		T l = this->length();
		if (l > 0.0000001) {
			T rl = 1.0 / l;
			x *= rl;
			y *= rl;
			z *= rl;
		}
	}

	Vec3<T> normalized() const {
		T len = this->length();
		if (len > 0.000001) {
			T rl = 1.0 / len;
			return Vec3<T> (x * rl, y * rl, z * rl);
		} else {
			return Vec3<T> ();
		}
	}

	bool approximatelyEqual(const Vec3<T>& _v) const {
		return (fabs(x - _v.x) < 0.0001f) && (fabs(y - _v.y) < 0.0001f) && (fabs(z - _v.z) < 0.0001f);
	}

	Vec3<T>& operator=(const Vec3<T>& _v) {
		x = _v.x;
		y = _v.y;
		z = _v.z;
		return *this;
	}

	Vec3<T> operator+(const Vec3<T>& _v) const {
		return Vec3<T> (x + _v.x, y + _v.y, z + _v.z);
	}

	Vec3<T> operator-() const {
		return Vec3<T> (-x, -y, -z);
	}

	Vec3<T> operator-(const Vec3<T>& _v) const {
		return Vec3<T> (x - _v.x, y - _v.y, z - _v.z);
	}

	void operator+=(const Vec3<T>& _v) {
		x += _v.x;
		y += _v.y;
		z += _v.z;
	}

	void operator-=(const Vec3<T>& _v) {
		x -= _v.x;
		y -= _v.y;
		z -= _v.z;
	}

	Vec3<T> operator*(const Vec3<T>& _v) const {
		return Vec3<T> (x * _v.x, y * _v.y, z * _v.z);
	}

	Vec3<T> operator*(T _factor) const {
		return Vec3<T> (x * _factor, y * _factor, z * _factor);
	}

	Vec3<T> operator*=(T _factor) {
		x *= _factor;
		y *= _factor;
		z *= _factor;
		return *this;
	}

	float operator[](uint8_t i) const {
		if (!i) {
			return x;
		} else if (i == 1) {
			return y;
		} else {
			return z;
		}
	}

	Vec3<T> cross(const Vec3<T>& _v) const {
		return Vec3<T> ((y * _v.z) - (z * _v.y), (z * _v.x) - (x * _v.z), (x * _v.y) - (y * _v.x));
	}

	void memCpyTo(T* ptrTo, size_t repeatCount = 1) {
		for (size_t i = 0; i < repeatCount; i++) {
			*ptrTo++ = x;
			*ptrTo++ = y;
			*ptrTo++ = z;
		}
	}

	template<typename U>
	friend bool operator==(const Vec3<U>& lhs, const Vec3<U>& rhs);

	template<typename U>
	friend Vec3<U> operator*(float _r, const Vec3<U>& _v);

	template<typename U>
	friend Vec3<U> operator*(double _r, const Vec3<U>& _v);

	template<typename U>
	friend Vec3<U> operator*(int _r, const Vec3<U>& _v);
};

typedef Vec3<float> Vec3f;
typedef Vec3<double> Vec3d;
typedef Vec3<uint32_t> Vec3ui;



/*---------------------- Vec4 -----------------*/

#ifdef USE_EXPORT_KEYWORD
export
#endif
template<typename T>
class Vec4 {

public:
	T coords[4];

	/** references coords[0] */
	T& x;

	/** references coords[1] */
	T& y;

	/** references coords[2] */
	T& z;

	/** references coords[3] */
	T& w;


	Vec4() :
		x(coords[0]), y(coords[1]), z(coords[2]), w(coords[3]) {
		coords[0] = coords[1] = coords[2] = coords[3] = 0.0f;
	}

	explicit Vec4(T _v) :
		x(coords[0]), y(coords[1]), z(coords[2]), w(coords[3]) {
		coords[0] = coords[1] = coords[2] = coords[3] = _v;
	}

	Vec4(const Vec3<T>& _v) :
			x(coords[0]), y(coords[1]), z(coords[2]), w(coords[3]) {
			coords[0] = _v.coords[0];
			coords[1] = _v.coords[1];
			coords[2] = _v.coords[2];
			coords[3] = 1.0f;
		}

	Vec4(T _x, T _y, T _z) :
			x(coords[0]), y(coords[1]), z(coords[2]), w(coords[3]) {
			coords[0] = _x;
			coords[1] = _y;
			coords[2] = _z;
			coords[3] = 1.0f;
		}

	Vec4(T _x, T _y, T _z, T _w) :
		x(coords[0]), y(coords[1]), z(coords[2]), w(coords[3]) {
		coords[0] = _x;
		coords[1] = _y;
		coords[2] = _z;
		coords[3] = _w;
	}

	Vec4(const Vec4<T>& _v) :
		x(coords[0]), y(coords[1]), z(coords[2]), w(coords[3]) {
		coords[0] = _v.coords[0];
		coords[1] = _v.coords[1];
		coords[2] = _v.coords[2];
		coords[3] = _v.coords[3];
	}

	T* ptr() {
		return &coords[0];
	}

	void get(T& _x, T& _y, T& _z, T& _w) const {
		_x = x;
		_y = y;
		_z = z;
		_w = w;
	}

	void set(T _x, T _y, T _z, T _w) {
		x = _x;
		y = _y;
		z = _z;
		w = _w;
	}

	T length() const {
		return sqrt(x * x + y * y + z * z + w * w);
	}

	T squaredLength() const {
		return (x * x + y * y + z * z + w * w);
	}

	T dot(const Vec4<T>& _v) const {
		return x * _v.x + y * _v.y + z * _v.z + w * _v.w;
	}

	T dot(T _x, T _y, T _z, T _w) const {
		return x * _x + y * _y + z * _z + w * _w;
	}

	void negate() {
		x *= -1;
		y *= -1;
		z *= -1;
		w *= -1;
	}

	void normalize() {
		T l = this->length();
		if (l > 0.0000001) {
			T rl = 1.0 / l;
			x *= rl;
			y *= rl;
			z *= rl;
			w *= rl;
		}
	}

	Vec4<T> normalized() const {
		T len = this->length();
		if (len > 0.000001) {
			T rl = 1.0 / len;
			return Vec4<T> (x * rl, y * rl, z * rl, w * rl);
		} else {
			return Vec4<T> ();
		}
	}

	bool approximatelyEqual(const Vec4<T>& _v) const {
		return (fabs(x - _v.x) < 0.0001f) && (fabs(y - _v.y) < 0.0001f) && (fabs(z - _v.z) < 0.0001f) && (fabs(w - _v.w) < 0.0001f);
	}

	Vec4<T>& operator=(const Vec4<T>& _v) {
		x = _v.x;
		y = _v.y;
		z = _v.z;
		w = _v.w;
		return *this;
	}

	Vec4<T> operator+(const Vec4<T>& _v) const {
		return Vec4<T> (x + _v.x, y + _v.y, z + _v.z, w + _v.w);
	}

	Vec4<T> operator-() const {
		return Vec4<T> (-x, -y, -z, -w);
	}

	Vec4<T> operator-(const Vec4<T>& _v) const {
		return Vec4<T> (x - _v.x, y - _v.y, z - _v.z, w - _v.w);
	}

	void operator+=(const Vec4<T>& _v) {
		x += _v.x;
		y += _v.y;
		z += _v.z;
		w += _v.w;
	}

	void operator-=(const Vec4<T>& _v) {
		x -= _v.x;
		y -= _v.y;
		z -= _v.z;
		w -= _v.w;
	}

	Vec4<T> operator*(const Vec4<T>& _v) const {
		return Vec4<T> (x * _v.x, y * _v.y, z * _v.z, w * _v.w);
	}

	Vec4<T> operator*(T _factor) const {
		return Vec4<T> (x * _factor, y * _factor, z * _factor, w * _factor);
	}

	Vec4<T> operator*=(T _factor) {
		x *= _factor;
		y *= _factor;
		z *= _factor;
		w *= _factor;
		return *this;
	}

	float operator[](uint8_t i) const {
		if (!i) {
			return x;
		} else if (i == 1) {
			return y;
		} else if (i == 2) {
			return z;
		} else {
			return w;
		}
	}

	void memCpyTo(T* ptrTo, size_t repeatCount = 1) {
		for (size_t i = 0; i < repeatCount; i++) {
			*ptrTo++ = x;
			*ptrTo++ = y;
			*ptrTo++ = z;
			*ptrTo++ = w;
		}
	}

	template<typename U>
	friend bool operator==(const Vec4<U>& lhs, const Vec4<U>& rhs);

	template<typename U>
	friend Vec4<U> operator*(float _r, const Vec4<U>& _v);

	template<typename U>
	friend Vec4<U> operator*(double _r, const Vec4<U>& _v);

	template<typename U>
	friend Vec4<U> operator*(int _r, const Vec4<U>& _v);
};

typedef Vec4<float> Vec4f;
typedef Vec4<double> Vec4d;
typedef Vec4<uint32_t> Vec4ui;

#endif /*VECTOR_H_*/
