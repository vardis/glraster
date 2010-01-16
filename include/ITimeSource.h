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
	virtual long getMillis() = 0;
};

#endif /* ITIMESOURCE_H_ */
