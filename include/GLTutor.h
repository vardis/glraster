#ifndef _GLTUTOR_H
#define _GLTUTOR_H

#if defined( __WIN32__ ) || defined( _WIN32 )
#define TARGET_PLATFORM WIN32

#elif defined( __APPLE_CC__)
#define TARGET_PLATFORM_APPLE

#else
#define TARGET_PLATFORM_LINUX
#endif

#include <stdexcept>

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>

#define _USE_MATH_DEFINES
#include <math.h>
#include <float.h>

#include <iostream>
#include <sstream>
#include <memory>
#include <memory.h>
#include <algorithm>
#include <vector>
#include <map>
#include <list>

#include <GL/glew.h>
#include <GL/freeglut.h>

using boost::shared_ptr;
using boost::shared_array;

typedef unsigned int uint32_t;
typedef uint8_t UBYTE;
typedef shared_ptr<UBYTE> UBYTEPtr;
typedef std::string String;

typedef boost::shared_ptr<uint32_t> IndexPtr;
typedef boost::shared_array<uint32_t> IndexArrayPtr;

#define deg_to_rads(x) x * (M_PI / 180.0)
#define clamp(x, l, h) x < l ? l : (x > h ? h : x);
#define wrap(x, l, h) (x < l) ? x + h : ( (x > h) ? x - h : x );

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

enum Axis {
	X_Axis = 0, Y_Axis = 1, Z_Axis = 2
};

#define NULL_MATERIAL_INDEX 0

#define MAX_TEXTURES_STACK 8
#define MAX_LIGHTS 8

static uint32_t getClosestPow2(uint32_t i) {
	--i;
	i |= i >> 16;
	i |= i >> 8;
	i |= i >> 4;
	i |= i >> 2;
	i |= i >> 1;
	++i;
	return i;
}

#include "GLException.h"

#define SAFE_THROW(x) if (!std::uncaught_exception()) {\
	throw x;\
}

#endif	/* _GLTUTOR_H */
