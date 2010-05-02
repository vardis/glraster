#include "GLTutor.h"
#include <gtest/gtest.h>

#include "Matrix4.h"
#include "PinholeCamera.h"
#include "tests-utils.h"

std::ostream& operator<<(std::ostream& os, Vec3f v) {
	os << "(" << v.x << " , " << v.y << " , " << v.z << ")\n";
	return os;
}

std::ostream& operator<<(std::ostream& os, PinholeCamera cam) {
	os << "(" << cam.getView() << ")\n";
	return os;
}

std::ostream& operator<<(std::ostream& os, Matrix4f mat) {
	os << "\n[" << mat.m[0][0] << " , " << mat.m[0][1] << " , " << mat.m[0][2] << " , " << mat.m[0][3] << "]\n";
	os << "[" << mat.m[1][0] << " , " << mat.m[1][1] << " , " << mat.m[1][2] << " , " << mat.m[1][3] << "]\n";
	os << "[" << mat.m[2][0] << " , " << mat.m[2][1] << " , " << mat.m[2][2] << " , " << mat.m[2][3] << "]\n";
	os << "[" << mat.m[3][0] << " , " << mat.m[3][1] << " , " << mat.m[3][2] << " , " << mat.m[3][3] << "]\n";
	return os;
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
