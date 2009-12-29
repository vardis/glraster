/*
 * GLXTimer.cpp
 *
 *  Created on: Dec 27, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"

#ifdef TARGET_PLATFORM_LINUX
#include <sys/time.h>
#include "GLXTimer.h"

GLXTimer::GLXTimer() {
	gettimeofday(&m_lastTime, 0);
}

GLXTimer::~GLXTimer() {
}

uint32_t GLXTimer::getMillis() {
	struct timeval tv;
	gettimeofday(&tv, 0);
	uint32_t millis = (tv.tv_sec - m_lastTime.tv_sec) * 1000 + (tv.tv_usec - m_lastTime.tv_usec) / 1000;
	m_lastTime = tv;
	return millis;
}

#endif /* TARGET_PLATFORM_LINUX */
