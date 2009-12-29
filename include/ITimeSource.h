/*
 * ITimeSource.h
 *
 *  Created on: Dec 27, 2009
 *      Author: giorgos
 */

#ifndef ITIMESOURCE_H_
#define ITIMESOURCE_H_

class ITimeSource {
public:
	virtual uint32_t getMillis() = 0;
};

#endif /* ITIMESOURCE_H_ */
