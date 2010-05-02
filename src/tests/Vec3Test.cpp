/*
 * Vec3Test.cpp
 *
 *  Created on: Apr 13, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include <gtest/gtest.h>
#include "Vec3Test.h"
#include "tests-utils.h"

TEST_F(Vec3Test, TestLength)
{
	Vec3f v(64, 12, 76);
	ASSERT_FLOAT_EQ(v.length(), 100.079968f);
	ASSERT_FLOAT_EQ(v.squaredLength(), 100.079968f * 100.079968f);
}

TEST_F(Vec3Test, TestDot)
{
	Vec3f v1(1, 2, 3);
	ASSERT_FLOAT_EQ(v1.dot(Vec3f(4,5,6)), 32.0f);
	ASSERT_FLOAT_EQ(v1.dot(4,5,6), 32.0f);
}

TEST_F(Vec3Test, TestNormalization)
{
	Vec3f v1(1, 2, 3);
	ASSERT_FLOAT_EQ(v1.normalized().length(), 1.0f);

	v1.normalize();
	ASSERT_FLOAT_EQ(v1.length(), 1.0f);
}

TEST_F(Vec3Test, TestEquality)
{
	Vec3f v1(1, 2, 3), v2(1.0000f, 2.000001f, 3.0000001f);
	ASSERT_TRUE(v1.approximatelyEqual(v2));
	ASSERT_TRUE(v1 == v2);

	v1 = v2;
	ASSERT_TRUE(v1.approximatelyEqual(v2));
	ASSERT_TRUE(v1 == v2);
}

TEST_F(Vec3Test, TestAddSubtract)
{
	Vec3f v1(1, 2, 3), v2(4, 5, 6);
	Vec3f vadd(5, 7, 9), vminus(3, 3, 3);
	ASSERT_EQ(v1+v2, vadd);
	ASSERT_EQ(v2-v1, vminus);

	v1 += v2;
	ASSERT_EQ(v1, vadd);

	v1.set(1,2,3);
	v2 -= v1;
	ASSERT_EQ(v2, vminus);
}

TEST_F(Vec3Test, TestMult)
{
	Vec3f v1(1, 2, 3), v2(2, 4, 6), v3(2, 2, 2);
	ASSERT_EQ(2.0f*v1, v2);
	ASSERT_EQ(v1*v3, v2);

	v1 *= 2.0f;
	ASSERT_EQ(v1, v2);
}

TEST_F(Vec3Test, TestIndexing)
{
	Vec3f v(1, 2, 3);

	ASSERT_FLOAT_EQ(v.x, v.coords[0]);
	ASSERT_FLOAT_EQ(v.y, v.coords[1]);
	ASSERT_FLOAT_EQ(v.z, v.coords[2]);

	ASSERT_FLOAT_EQ(v[0], 1.0f);
	ASSERT_FLOAT_EQ(v[1], 2.0f);
	ASSERT_FLOAT_EQ(v[2], 3.0f);

	float fd[6];
	v.memCpyTo(fd, 2);
	ASSERT_FLOAT_EQ(fd[0], 1.0f);
	ASSERT_FLOAT_EQ(fd[1], 2.0f);
	ASSERT_FLOAT_EQ(fd[2], 3.0f);
	ASSERT_FLOAT_EQ(fd[3], 1.0f);
	ASSERT_FLOAT_EQ(fd[4], 2.0f);
	ASSERT_FLOAT_EQ(fd[5], 3.0f);
}

TEST_F(Vec3Test, TestCross)
{
	ASSERT_EQ(Vec3f::X_Axis.cross(Vec3f::Y_Axis), Vec3f::Z_Axis);
	ASSERT_EQ(Vec3f::X_Axis.cross(Vec3f::Y_Neg_Axis), Vec3f::Z_Neg_Axis);
	ASSERT_EQ(Vec3f::X_Neg_Axis.cross(Vec3f::Y_Axis), Vec3f::Z_Neg_Axis);
	ASSERT_EQ(Vec3f::X_Neg_Axis.cross(Vec3f::Y_Neg_Axis), Vec3f::Z_Axis);
}
