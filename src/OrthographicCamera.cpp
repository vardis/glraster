/*
 * OrthographicCamera.cpp
 *
 *  Created on: Mar 13, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "OrthographicCamera.h"

OrthographicCamera::OrthographicCamera() : Camera() {

}

OrthographicCamera::OrthographicCamera(const OrthographicCamera& other)
: Camera(other), m_width(other.m_width), m_height(other.m_height) {

}

OrthographicCamera::~OrthographicCamera() {
}
