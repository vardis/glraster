#include "GLTutor.h"
#include "Ray.h"
#include "AABB.h"

#define EPSILON_POS 0.0001f

template<typename T>
AABB<T>::AABB() :
min(FLT_MAX, FLT_MAX, FLT_MAX), max(-FLT_MAX, -FLT_MAX, -FLT_MAX) {

}

template<typename T>
AABB<T>::AABB(const AABB<T>& _aabb) :
min(_aabb.min), max(_aabb.max) {

}

template<typename T>
AABB<T>::AABB(const Vec3<T>& _p) :
min(_p.x, _p.y, _p.z), max(_p.x, _p.y, _p.z) {

}

template<typename T>
AABB<T>::AABB(const Vec3<T>& _p1, const Vec3<T>& _p2) :
min(std::min(_p1.x, _p2.x), std::min(_p1.y, _p2.y), std::min(_p1.z, _p2.z)),
max(std::max(_p1.x, _p2.x), std::max(_p1.y, _p2.y), std::max(_p1.z, _p2.z)) {
}

template<typename T>
void AABB<T>::addPoint(const Vec3<T>& _p) {
    min.x = std::min(_p.x, min.x);
    min.y = std::min(_p.y, min.y);
    min.z = std::min(_p.z, min.z);
    max.x = std::max(_p.x, max.x);
    max.y = std::max(_p.y, max.y);
    max.x = std::max(_p.z, max.z);
}

template<typename T>
void AABB<T>::addPoint(T _x, T _y, T _z) {
    min.x = std::min(_x, min.x);
    min.y = std::min(_y, min.y);
    min.z = std::min(_z, min.z);
    max.x = std::max(_x, max.x);
    max.y = std::max(_y, max.y);
    max.x = std::max(_z, max.z);
}

template<typename T>
AABB<T> AABB<T>::unionWith(const AABB<T>& _aabb) const {
    return AABB(
            Vec3<T > (std::min(min.x, _aabb.min.x), std::min(min.y, _aabb.min.y), std::min(min.z, _aabb.min.z)),
            Vec3<T > (std::max(max.x, _aabb.max.x), std::max(max.y, _aabb.max.y), std::max(max.z, _aabb.max.z))
            );
}

template<typename T>
bool AABB<T>::overlaps(const AABB<T>& _aabb) const {
    return
    (max.x >= _aabb.min.x) &&
            (min.x <= _aabb.max.x) &&
            (max.y >= _aabb.min.y) &&
            (min.y <= _aabb.max.y) &&
            (max.z >= _aabb.min.z) &&
            (min.z <= _aabb.max.z);
}

template<typename T>
bool AABB<T>::isInside(const Vec3<T>& _p) const {
    return
    (max.x >= _p.x) &&
            (min.x <= _p.x) &&
            (max.y >= _p.y) &&
            (min.y <= _p.y) &&
            (max.z >= _p.z) &&
            (min.z <= _p.z);
}

template<typename T>
void AABB<T>::expand(T _r) {
    min += Vec3<T > (_r, _r, _r);
    max += Vec3<T > (_r, _r, _r);
}

template<typename T>
T AABB<T>::volume() const {
    Vec3<T> diag = max - min;
    return diag.x * diag.y * diag.z;
}

template<typename T>
enum Axis AABB<T>::maximumAxis() const {
    Vec3<T> diag = max - min;
    if (diag.x > diag.y && diag.x > diag.z) {
        return X_Axis;
    } else if (diag.y > diag.z) {
        return Y_Axis;
    } else {
        return Z_Axis;
    }
}

template<typename T>
void AABB<T>::boundingSphere(Vec3<T>& _c, T& _rad) const {
    _c = (T) 0.5 * (min + max);
    _rad = isInside(_c) ? (_c - max).length() : 0.f;
}

template<typename T>
void AABB<T>::getDimensions(Vec3<T>& _dims) const {

    _dims.set(fabs(max.x - min.x), fabs(max.y - min.y), fabs(max.z - min.z));
}

template<typename T>
bool AABB<T>::intersect(const Ray<T>& _ray, T& _tmin, T& _tmax, Vec3<T>* _tminVec) {

    T tNear = -FLT_MAX;
    T tFar = FLT_MAX;

    const Vec3<T>& origin = _ray.getOrigin();
    const Vec3<T>& dir = _ray.getDirection();

    // XY slab
    if (abs(dir.z) < EPSILON_POS) {
        // ray parallel to slab's XY bounding planes, check if within the slab
        if (origin.z < min.z && origin.z > max.z) {
            return false;
        }
    }

    T t0 = (min.z - origin.z) / dir.z;
    T t1 = (max.z - origin.z) / dir.z;
    if (t0 > t1) {
        std::swap(t0, t1);
    }

    if (_tminVec) _tminVec->x = t0;

    if (t0 > tNear) {
        tNear = t0;
    }
    if (t1 < tFar) {
        tFar = t1;
    }

    if (tNear > tFar || tFar < 0) {
        return false;
    }

    // YZ
    if (fabs(dir.x) < EPSILON_POS) {
        // ray parallel to slab's YZ bounding planes, check if within the slab
        if (origin.x < min.x && origin.x > max.x) {
            return false;
        }
    }

    t0 = (min.x - origin.x) / dir.x;
    t1 = (max.x - origin.x) / dir.x;
    if (t0 > t1) {
        std::swap(t0, t1);
    }
    if (_tminVec) _tminVec->y = t0;

    if (t0 > tNear) {
        tNear = t0;
    }
    if (t1 < tFar) {
        tFar = t1;
    }

    if (tNear > tFar || tFar < 0) {
        return false;
    }

    // XZ
    if (fabs(dir.y) < EPSILON_POS) {
        // ray parallel to slab's XZ bounding planes, check if within the slab
        if (origin.y < min.y && origin.y > max.y) {
            return false;
        }
    }

    t0 = (min.y - origin.y) / dir.y;
    t1 = (max.y - origin.y) / dir.y;
    if (t0 > t1) {
        std::swap(t0, t1);
    }
    if (_tminVec) _tminVec->z = t0;

    if (t0 > tNear) {
        tNear = t0;
    }

    if (t1 < tFar) {
        tFar = t1;
    }

    if (tNear > tFar || tFar < 0 || tNear > _ray.getMax() || tFar < _ray.getMin()) {
        return false;
    }

    _tmin = tNear;
    _tmax = tFar;

    return true;
}

template<typename T>
AABB<T>& AABB<T>::operator=(const AABB<T>& rhs) {
	min = rhs.min;
	max = rhs.max;
	return *this;
}

template class AABB<float>;
template class AABB<double>;

