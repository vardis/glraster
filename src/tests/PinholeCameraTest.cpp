/*
 * PinholeCameraTest.cpp
 *
 *  Created on: Apr 13, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include <gtest/gtest.h>
#include "tests-utils.h"
#include "PinholeCameraTest.h"

TEST_F(PinholeCameraTest, TestMovements)
{
	PinholeCamera cam;
	cam.forward(10.0f);
	cam.backward(5.0f);
	cam.up(10.0f);
	cam.down(5.0f);
	cam.left(5.0f);
	cam.right(10.0f);
	cam.updateView();

	Vec3f camPos(5.0f, 5.0f, -5.0f);
	ASSERT_EQ(cam.getPos(), camPos);
	ASSERT_EQ(cam.getView().w(), -camPos);
}


TEST_F(PinholeCameraTest, TestOrientation)
{
	PinholeCamera cam;
	cam.setPos(Vec3f(2.0f, 2.0f, -5.0f));
	cam.setUp(Vec3f::Y_Axis);
	cam.setLook(Vec3f(0.0f, 0.0f, 5.0f));
	cam.updateView();

	Matrix4f tr = Matrix4f::Translation(-cam.getPos().x, -cam.getPos().y, -cam.getPos().z);
	std::cout << tr;
	ASSERT_EQ(cam.getView(), m_testOrientationMat);
}
