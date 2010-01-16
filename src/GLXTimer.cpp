/*
 * GLXTimer.cpp
 *
 *  Created on: Dec 27, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"

#ifdef TARGET_PLATFORM_LINUX
#include <time.h>
#include "GLXTimer.h"

GLXTimer::GLXTimer() {
//	gettimeofday(&m_lastTime, 0);
	clock_gettime(CLOCK_MONOTONIC, &m_lastTime);
}

GLXTimer::~GLXTimer() {
}

long GLXTimer::getMillis() {
	struct timespec ts;
	if (clock_gettime(CLOCK_MONOTONIC, &ts)) {
		std::cerr << "clock_gettime failed";
		return 0;
	}

	long millis = (ts.tv_sec - m_lastTime.tv_sec) * 1000 + (float) (ts.tv_nsec - m_lastTime.tv_nsec) / 1000000;
	m_lastTime = ts;
	return millis;
}

#endif /* TARGET_PLATFORM_LINUX */
