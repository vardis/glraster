#ifndef AABB_H_
#define AABB_H_

#include "Vector.h"

template<typename T> class Ray;

template<typename T>
class AABB {
public:
    Vec3<T> min, max;

public:
    static const AABB<T>& EmptyAABB();

    AABB();
    AABB(const Vec3<T>& _p1, const Vec3<T>& _p2);
    AABB(const AABB<T>& _aabb);
    AABB(const Vec3<T>& _p);

    void addPoint(const Vec3<T>& _p);
    void addPoint(T _x, T _y, T _z);

    bool intersect(const Ray<T>& _ray, T& _tmin, T& _tmax, Vec3<T>* _tminVec = 0);    

    AABB<T> unionWith(const AABB<T>& _aabb) const;
    bool overlaps(const AABB<T>& _aabb) const;
    bool isInside(const Vec3<T>& _p) const;
    void expand(T _r);
    T volume() const;
    enum Axis maximumAxis() const;
    void boundingSphere(Vec3<T>& _c, T& _rad) const;
    void getDimensions(Vec3<T>& _dims) const;

    AABB<T>& operator=(const AABB<T>& rhs);
};

template <typename T> const AABB<T>& AABB<T>::EmptyAABB() {
    static AABB<T> emptyAABB;
    return emptyAABB;
}


#endif /*AABB_H_*/
