#include "GLTutor.h"
#include "Vector.h"

template<typename T>
Vec3<T> Vec3<T>::Front(0.0f, 0.0f, 1.0f);

template<typename T>
Vec3<T> operator*(float _r, const Vec3<T>& _v) {
	return Vec3<T> (_r * _v.x, _r * _v.y, _r * _v.z);
}

template<typename T>
Vec3<T> operator*(double _r, const Vec3<T>& _v) {
	return Vec3<T> (_r * _v.x, _r * _v.y, _r * _v.z);
}

template<typename T>
Vec3<T> operator*(int _r, const Vec3<T>& _v) {
	return Vec3<T> (_r * _v.x, _r * _v.y, _r * _v.z);
}

// Classes templates instantiations
#ifndef USE_EXPORT_KEYWORD
template class Vec2<float>;
template class Vec2<double>;
template class Vec3<float>;
template class Vec3<double>;
#endif

// Function templates instantiations
template Vec3<float> operator*(float _r, const Vec3<float>& _v);
template Vec3<float> operator*(double _r, const Vec3<float>& _v);
template Vec3<float> operator*(int _r, const Vec3<float>& _v);

template Vec3<double> operator*(float _r, const Vec3<double>& _v);
template Vec3<double> operator*(double _r, const Vec3<double>& _v);
template Vec3<double> operator*(int _r, const Vec3<double>& _v);

