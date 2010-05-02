/*
 * Matrix4Test.h
 *
 *  Created on: Apr 10, 2010
 *      Author: giorgos
 */

#ifndef MATRIX4FTEST_H_
#define MATRIX4FTEST_H_

#include "Matrix4.h"

class Matrix4Test : public ::testing::Test {

protected:
	float m_data[4][4];

	/** a correct matrix for CCW rotation of 45 degrees around the X-axis */
	Matrix4f m_matRotX45CCW;

	/** a correct matrix for CW rotation of 240 degrees around the Y-axis */
	Matrix4f m_matRotY240CW;

	/** a correct matrix for CCW rotation of 120 degrees around the Z-axis */
	Matrix4f m_matRotZ120CCW;

	/**
	 * a correct matrix for concatenation of a series of rotations:
	 * 30 degrees around X, then 40 around Y and then 50 around Z
	 */
	Matrix4f m_matRotY40X30Z50;

	/** a correct matrix for rotating 60 degrees around the axis (1,1,-1) */
	Matrix4f m_axisRot;

	/** a correct matrix for a uniform scale by 2, a translations by 1,2,3 and a rotation according to m_axisRot */
	Matrix4f m_xforms;

	/** the correct inverse of the above matrix */
	Matrix4f m_xformsInv;

	/** a matrix with nonzero elements */
	Matrix4f m_simpleMat;

	/* equals to m_simpleMat * m_simpleMat */
	Matrix4f m_simpleMat2;

public:
	Matrix4Test() :
		m_matRotX45CCW(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.707107f, -0.707107f, 0.0f, 0.0f, 0.707107f, 0.707107f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f),
		m_matRotY240CW(-0.50, 0.0, 0.866025, 0.0, 0.0, 1.0,
				0.0, 0.0, -0.866025, -0.0, -0.50, 0.0, 0.0, 0.0, 0.0, 1.0),
		m_matRotZ120CCW(-0.50, -0.866025, 0.0, 0.0,0.866025, -0.50, 0.0, 0.0,0.0, 0.0, 1.0, 0.0,0.0, 0.0, 0.0, 1.0),
		m_matRotY40X30Z50(0.246202, -0.663414, 0.706588, 0.0,0.793412, 0.556670, 0.246202, 0.0,-0.556670, 0.50, 0.663414, 0.0,0.0, 0.0, 0.0, 1.0),
		m_axisRot(0.666667, 0.666667, 0.333333, 0.0,-0.333333, 0.666667, -0.666667, 0.0,-0.666667, 0.333333, 0.666667, 0.0,0.0, 0.0, 0.0, 1.0),
		m_xforms(1.333333, 1.333333, 0.666667, 3.000000,-0.666667, 1.333333, -1.333333, -1.000000,-1.333333, 0.666667, 1.333333, 2.000000,0.000000, 0.000000, 0.000000, 1.000000),
		m_xformsInv(0.333333, -0.166667, -0.333333, -0.500000,0.333333, 0.333333, 0.166667, -1.000000,0.166667, -0.333333, 0.333333, -1.500000,0.000000, 0.000000, -0.000000, 1.000000),
		m_simpleMat(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16),
		m_simpleMat2(90.0, 100.0, 110.0, 120.0,202.0, 228.0, 254.0, 280.0,314.0, 356.0, 398.0, 440.0,426.0, 484.0, 542.0, 600.0)
	{
		float data[4][4] = { { 1, 2, 3, 4 }, { 5, 6, 7, 8 }, { 9, 10, 11, 12 }, { 13, 14, 15, 16 } };
		memcpy(m_data, data, 16 * sizeof(float));
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp() {
		// Code here will be called immediately after the constructor (right
		// before each test).
	}

	virtual void TearDown() {
		// Code here will be called immediately after each test (right
		// before the destructor).
	}
};

#endif /* MATRIX4FTEST_H_ */
