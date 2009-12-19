#ifndef _GLTUTOR_H
#define _GLTUTOR_H

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>

#include <iostream>
#include <memory>
#include <memory.h>
#include <algorithm>
#include <vector>
#include <list>

#include <GL/glew.h>
#include <GL/freeglut.h>


using boost::shared_ptr;

typedef unsigned int uint32_t;
typedef uint8_t UBYTE;
typedef shared_ptr<UBYTE> UBYTEPtr;
typedef std::string String;

#define deg_to_rads(x) x * (M_PI / 180.0)
#define clamp(x, l, h) x < l ? l : (x > h ? h : x);
#define wrap(x, l, h) (x < l) ? x + h : ( (x > h) ? x - h : x );

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

enum Axis {
	X_Axis = 0,
	Y_Axis = 1,
	Z_Axis = 2
};

#define NULL_MATERIAL_INDEX 0

#endif	/* _GLTUTOR_H */
