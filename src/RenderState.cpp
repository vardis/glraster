/*
 * RenderState.cpp
 *
 *  Created on: Feb 8, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "RenderState.h"

RenderState::RenderState() {
	m_clipPlanes[0] = {0.0f, 0.0f, 0.0f, 0.0f};
}

bool RenderState::operator==(const RenderState& rs) {
}
