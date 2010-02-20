#include "Colour.h"

Colour Colour::WHITE(1.0f, 1.0f, 1.0f, 1.0f);

Colour::Colour() :
	r(rgba[0]), g(rgba[1]), b(rgba[2]), a(rgba[3]) {
	rgba[0] = 1.0f;
	rgba[1] = 1.0f;
	rgba[2] = 1.0f;
	rgba[3] = 1.0f;
}

Colour::Colour(float _r, float _g, float _b) :
	r(rgba[0]), g(rgba[1]), b(rgba[2]), a(rgba[3]) {
	rgba[0] = _r;
	rgba[1] = _g;
	rgba[2] = _b;
	rgba[3] = 1.0f;
}

Colour::Colour(float _r, float _g, float _b, float _a) :
	r(rgba[0]), g(rgba[1]), b(rgba[2]), a(rgba[3]) {
	rgba[0] = _r;
	rgba[1] = _g;
	rgba[2] = _b;
	rgba[3] = _a;

}

Colour::Colour(const Colour& _c) :
	r(_c.r), g(_c.g), b(_c.b), a(_c.a) {
	rgba[0] = _c.rgba[0];
	rgba[1] = _c.rgba[1];
	rgba[2] = _c.rgba[2];
	rgba[3] = _c.rgba[3];
}

void Colour::memCpyTo(float* ptrTo, int repeatCount) {
	for (int i = 0; i < repeatCount; i++) {
		*ptrTo++ = r;
		*ptrTo++ = g;
		*ptrTo++ = b;
		*ptrTo++ = a;
	}
}

void Colour::set(float _r, float _g, float _b, float _a) {
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

const Colour& Colour::operator=(const Colour& _c) {
	r = _c.r;
	g = _c.g;
	b = _c.b;
	a = _c.a;
	return *this;
}

const Colour& Colour::operator+=(const Colour& _c) {
	r += _c.r;
	g += _c.g;
	b += _c.b;
	a += _c.a;
	return *this;
}

Colour Colour::operator+(const Colour& _c) const {
	return Colour(r + _c.r, g + _c.g, b + _c.b);
}

const Colour& Colour::operator*=(float _r) {
	r *= _r;
	g *= _r;
	b *= _r;
	a *= _r;
	return *this;
}

const Colour& Colour::operator*=(const Colour& _c) {
	r *= _c.r;
	g *= _c.g;
	b *= _c.b;
	a *= _c.a;
	return *this;
}

const Colour& Colour::operator/=(float _r) {
	r /= _r;
	g /= _r;
	b /= _r;
	a /= _r;
	return *this;
}

Colour Colour::operator/(float _r) {
	float invr = 1.0f / _r;
	return Colour(invr * r, invr * g, invr * b, invr * a);
}

Colour Colour::operator*(const Colour& _c) const {
	return Colour(_c.r * r, _c.g * g, _c.b * b, _c.a * a);
}

Colour operator*(float _r, const Colour& _c) {
	return Colour(_r * _c.r, _r * _c.g, _r * _c.b, _r * _c.a);
}

