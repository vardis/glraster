/*
 * Matrix4Test.cpp
 *
 *  Created on: Apr 10, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include <gtest/gtest.h>
#include "tests-utils.h"
#include "Matrix4Test.h"

TEST_F(Matrix4Test, TestVectorAccessors)
{
	Matrix4f mat(m_data);
	Vec3f xcol = mat.x();
	ASSERT_FLOAT_EQ(xcol.coords[0], 1.0f);
	ASSERT_FLOAT_EQ(xcol.coords[1], 5.0f);
	ASSERT_FLOAT_EQ(xcol.coords[2], 9.0f);
}

TEST_F(Matrix4Test, TestFromBasis)
{
	Matrix4f mat = Matrix4f::FromBasis(Vec3f::X_Axis, Vec3f::Y_Axis, Vec3f::Z_Axis);
	ASSERT_EQ(mat, Matrix4f().identity());
}

TEST_F(Matrix4Test, TestTranspose)
{
	Matrix4f mat(m_data);
	Matrix4f trmat = mat.transposed();
	ASSERT_EQ(mat.transpose(), trmat);
}

TEST_F(Matrix4Test, TestTranslations)
{
	Matrix4f matTr1 = Matrix4f::Translation(Vec3f(1, 2, 3));
	Matrix4f matTr2 = Matrix4f::Translation(1, 2, 3);
	ASSERT_EQ(matTr1, matTr2);

	ASSERT_EQ(matTr1.w(), Vec3f(1,2,3));
	ASSERT_EQ(matTr2.w(), Vec3f(1,2,3));
}

TEST_F(Matrix4Test, TestScale)
{
	Matrix4f matSc1 = Matrix4f::Scale(Vec3f(1, 2, 3));
	Matrix4f matSc2 = Matrix4f::Scale(1, 2, 3);
	ASSERT_EQ(matSc1, matSc2);

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (i == j && i < 3 && j < 3) {
				ASSERT_FLOAT_EQ(matSc1.m[i][j], i + 1.0f);
			} else if (i == 3 && j == 3) {
				ASSERT_FLOAT_EQ(matSc1.m[i][j], 1.0f);

			} else {
				ASSERT_FLOAT_EQ(matSc1.m[i][j], 0.0f);
			}
		}
	}
}

TEST_F(Matrix4Test, TestRotations)
{
	Matrix4f mat = Matrix4f::RotationX(deg_to_rads(45.0f));
	ASSERT_EQ(mat, m_matRotX45CCW);

	mat = Matrix4f::RotationY(deg_to_rads(-240.0f));
	ASSERT_EQ(mat, m_matRotY240CW);

	mat = Matrix4f::RotationZ(deg_to_rads(120.0f));
	ASSERT_EQ(mat, m_matRotZ120CCW);

	mat = Matrix4f::Rotation(Vec3f(1.0f, 1.0f, -1.0f), deg_to_rads(60.0f));
	ASSERT_EQ(mat, m_axisRot);
}

TEST_F(Matrix4Test, TestMultiplication)
{
	Matrix4f m2 = m_simpleMat * m_simpleMat;
	ASSERT_EQ(m2, m_simpleMat2);
}

TEST_F(Matrix4Test, TestMultipleRotations)
{
	Matrix4f matX30 = Matrix4f::RotationX(deg_to_rads(30.0f));
	Matrix4f matY40 = Matrix4f::RotationY(deg_to_rads(40.0f));
	Matrix4f matZ50 = Matrix4f::RotationZ(deg_to_rads(50.0f));
	Matrix4f concatMat = matZ50 * matX30 * matY40;
	ASSERT_EQ(concatMat, m_matRotY40X30Z50);
}

TEST_F(Matrix4Test, TestXFormsConcatenations)
{
	Matrix4f xforms = Matrix4f::Rotation(Vec3f(1.0f, 1.0f, -1.0f), deg_to_rads(60.0f)) * Matrix4f::Translation(1.0f,
			2.0f, 3.0f) * Matrix4f::Scale(2.0f, 2.0f, 2.0f);
	ASSERT_EQ(xforms, m_xforms);
}

TEST_F(Matrix4Test, TestInverse)
{
	Matrix4f inv = m_xforms.inverse();
	ASSERT_EQ(inv, m_xformsInv);
}

TEST_F(Matrix4Test, TestVectorMultiplication)
{
	Matrix4f rot = Matrix4f::RotationY(deg_to_rads(270));
	Vec3f z(0.0f, 0.0f, 1.0f);
	ASSERT_EQ(z, rot * Vec3f::X_Axis);
}

