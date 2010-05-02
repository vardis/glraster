/*
 * PinholeCameraTest.h
 *
 *  Created on: Apr 13, 2010
 *      Author: giorgos
 */

#ifndef PINHOLECAMERATEST_H_
#define PINHOLECAMERATEST_H_

#include "PinholeCamera.h"

class PinholeCameraTest: public ::testing::Test {

protected:
	Matrix4f m_testOrientationMat;

public:
	PinholeCameraTest() :
		m_testOrientationMat(-0.980581, -0.037743, 0.192450, -2.000000, -0.000000, 0.981307, 0.192450, -2.000000,-0.196116, 0.188713, -0.962250, 5.000000,0.000000, 0.000000, 0.000000, 1.000000) {

	}

	virtual void SetUp() {
	}

	virtual void TearDown() {
	}
};

#endif /* PINHOLECAMERATEST_H_ */
