#ifndef COLOUR_H_
#define COLOUR_H_


class Colour {

public:
	float r, g, b, a;

public:
	Colour();
	Colour(float _r, float _g, float _b);
	Colour(float _r, float _g, float _b, float _a);
	Colour(const Colour& _c);

	void set(float _r, float _g, float _b, float _a = 1.0f);
	inline void set(float _r) {
		r = g = b = _r;
	}

	const Colour& operator=(const Colour& _c);

	const Colour& operator+=(const Colour& _c);

	const Colour& operator*=(float _r);

	const Colour& operator*=(const Colour& _c);

	const Colour& operator/=(float _r);

	Colour operator/(float _r);

	Colour operator+(const Colour& _c) const;

	Colour operator*(const Colour& _c) const;

	friend Colour operator*(float _r, const Colour& _c);
};

#endif /*COLOUR_H_*/
