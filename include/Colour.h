#ifndef COLOUR_H_
#define COLOUR_H_


class Colour {

public:
	float rgba[4];

	/** references rgba[0] */
	float& r;

	/** references rgba[1] */
	float& g;

	/** references rgba[2] */
	float& b;

	/** references rgba[3] */
	float& a;

	static Colour WHITE;

public:
	Colour();
	Colour(float _r, float _g, float _b);
	Colour(float _r, float _g, float _b, float _a);
	Colour(const Colour& _c);

	/**
	 * Copies repeatCount copies of the rgba data to the provided memory location
	 * @param ptrTo the destination memory location
	 * @param repeatCount number of times to copy the data
	 */
	void memCpyTo(float* ptrTo, int repeatCount = 1);

	/**
	 * Returns a pointer to the rgba data. This is used to interface with GLSL uniform setters.
	 * @see GLSLProgram
	 */
	float* ptr() {
		return rgba;
	}

	void set(float _r, float _g, float _b, float _a = 1.0f);
	inline void set(float _r) {
		r = g = b = a = _r;
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
	friend bool operator==(const Colour& c1, const Colour& c2);
};

bool operator==(const Colour& c1, const Colour& c2);

#endif /*COLOUR_H_*/
