#ifndef _RAY_H
#define _RAY_H

#include "Vector.h"

template<typename T>
class Ray {

public:
	Vec3<T> origin;
	Vec3<T> direction;
	T min, max;

public:

	// CONSTRUCTOR & DESTRUCTOR
	inline Ray() {
	}

	Ray(const Vec3<T>& _origin, const Vec3<T>& _direction) :
		origin(_origin), direction(_direction), min(0.0), max(100.0) {
	}

	Ray(const Vec3<T>& _origin, const Vec3<T>& _direction, T _min, T _max) :
		origin(_origin), direction(_direction), min(_min), max(_max) {
	}

	Ray(const Ray& _ray) {
		origin = _ray.origin;
		direction = _ray.direction;
		min = _ray.min;
		max = _ray.max;
//		*this = _ray;
	}

	inline const Vec3<T>& getOrigin() const {
		return origin;
	}

	inline const Vec3<T>& getDirection() const {
		return direction;
	}

	inline const Vec3<T>& getDirection() {
			return direction;
	}

	inline T getMin() const {
		return min;
	}

	inline T getMax() const {
		return max;
	}

	inline void setOrigin(const Vec3<T>& _p) {
		origin = _p;
	}

	inline void setDirection(const Vec3<T>& _v) {
		direction = _v;
	}

	inline void setMin(T _m) {
		min = _m;
	}

	inline void setMax(T _m) {
		max = _m;
	}

	inline void VecAtDistance(T _t, Vec3<T>& _resultVec) const {
		_resultVec.x = origin.x + _t * direction.x;
		_resultVec.y = origin.y + _t * direction.y;
		_resultVec.z = origin.z + _t * direction.z;
	}

};


#endif
