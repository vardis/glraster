/*
 * GLXTimer.h
 *
 *  Created on: Dec 27, 2009
 *      Author: giorgos
 */

#ifndef GLXTIMER_H_
#define GLXTIMER_H_

#ifdef TARGET_PLATFORM_LINUX

#include "ITimeSource.h"

class GLXTimer : public ITimeSource {
public:
//	struct timeval m_lastTime;
	struct timespec m_lastTime;

public:
	GLXTimer();
	virtual ~GLXTimer();

	virtual long getMillis();
};

#endif /* TARGET_PLATFORM_LINUX */

#endif /* GLXTIMER_H_ */
