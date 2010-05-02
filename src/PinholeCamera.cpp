/* 
 * File:   PinholeCamera.cpp
 * Author: giorgos
 * 
 * Created on November 13, 2009, 12:02 AM
 */

#include "GLTutor.h"
#include "PinholeCamera.h"

PinholeCamera::PinholeCamera() :
	Camera(), m_fov(45.0f), m_aspectRatio(0.75f) {
}

PinholeCamera::PinholeCamera(const PinholeCamera& orig) :
	Camera(orig), m_fov(orig.m_fov), m_aspectRatio(orig.m_aspectRatio) {
}

PinholeCamera::~PinholeCamera() {
}

