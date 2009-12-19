/* 
 * File:   PinholeCamera.cpp
 * Author: giorgos
 * 
 * Created on November 13, 2009, 12:02 AM
 */

#include "GLTutor.h"
#include "PinholeCamera.h"

PinholeCamera::PinholeCamera() :
m_pos(), m_right(1.0f, 0.0f, 0.0f), m_up(0.0f, 1.0f, 0.0f), m_look(0.0f, 0.0f, 1.0f), m_rotation(), m_view(), m_proj(), m_fov(45.0f), m_near(1.0f), m_far(1000.0f), m_aspectRatio(0.75f) {
}

PinholeCamera::PinholeCamera(const PinholeCamera& orig) :
m_pos(orig.m_pos), m_right(orig.m_right), m_up(orig.m_up), m_look(orig.m_look), m_rotation(orig.m_rotation), m_view(orig.m_view), m_proj(orig.m_proj), m_fov(orig.m_fov), m_near(orig.m_near), m_far(orig.m_far), m_aspectRatio(orig.m_aspectRatio) {
}

PinholeCamera::~PinholeCamera() {
}


