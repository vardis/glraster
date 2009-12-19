#include "Colour.h"

Colour::Colour() :
	r(0.0f), g(0.0f), b(0.0f), a(1.0f) {
}

Colour::Colour(float _r, float _g, float _b) :
	r(_r), g(_g), b(_b), a(1.0f) {

}

Colour::Colour(float _r, float _g, float _b, float _a) :
	r(_r), g(_g), b(_b), a(_a) {

}

Colour::Colour(const Colour& _c) :
	r(_c.r), g(_c.g), b(_c.b), a(_c.a) {

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
	return Colour(invr*r, invr*g, invr*b, invr*a);
}

Colour Colour::operator*(const Colour& _c) const {
	return Colour(_c.r*r, _c.g*g, _c.b*b, _c.a*a);
}

Colour operator*(float _r, const Colour& _c) {
	return Colour(_r*_c.r, _r*_c.g, _r*_c.b, _r*_c.a);
}

