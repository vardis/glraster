/*
 * tests-utils.h
 *
 *  Created on: Apr 13, 2010
 *      Author: giorgos
 */

#ifndef TESTSUTILS_H_
#define TESTSUTILS_H_

#include "Matrix4.h"

class PinholeCamera;

std::ostream& operator<<(std::ostream& os, Matrix4f mat);
std::ostream& operator<<(std::ostream& os, Vec3f v);
std::ostream& operator<<(std::ostream& os, PinholeCamera v);

#endif /* TESTSUTILS_H_ */
