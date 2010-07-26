/*
 * RenderState.cpp
 *
 *  Created on: Feb 8, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "RenderState.h"

RenderState::RenderState() :
	m_renderMode(GL_FILL), m_culling(true), m_cullFace(GL_BACK), m_frontFace(GL_CCW), m_lineWidth(1.0f), m_pointSize(
			1.0f), m_scissor(false), m_scissorLeft(0.0f), m_scissorBottom(0.0f), m_scissorWidth(0.0f), m_scissorHeight(
			0.0f), m_stencil(false), m_stencilFunc(GL_ALWAYS), m_stencilRef(0), m_stencilMask(0), m_stencilOpFail(
			GL_KEEP), m_stencilOpPass(GL_KEEP), m_depthTest(true), m_depthFunc(GL_LEQUAL), m_depthMask(true),
			m_clearColorBuffer(false), m_clearDepth(1000000.0f), m_clearDepthBuffer(false), m_clearStencil(0),
			m_clearStencilBuffer(false), m_shadeless(false) {
	m_colorMask[0] = m_colorMask[1] = m_colorMask[2] = m_colorMask[3] = true;

}

RenderState::RenderState(const RenderState& rs) :
	m_renderMode(rs.m_renderMode), m_culling(rs.m_culling), m_cullFace(rs.m_cullFace), m_frontFace(rs.m_frontFace),
			m_lineWidth(rs.m_lineWidth), m_pointSize(rs.m_pointSize), m_scissor(rs.m_scissor), m_scissorLeft(
					rs.m_scissorLeft), m_scissorBottom(rs.m_scissorBottom), m_scissorWidth(rs.m_scissorWidth),
			m_scissorHeight(rs.m_scissorHeight), m_stencil(rs.m_stencil), m_stencilFunc(rs.m_stencilFunc),
			m_stencilRef(rs.m_stencilRef), m_stencilMask(rs.m_stencilMask), m_stencilOpFail(rs.m_stencilOpFail),
			m_stencilOpPass(rs.m_stencilOpPass), m_depthTest(rs.m_depthTest), m_depthFunc(rs.m_depthFunc), m_depthMask(
					rs.m_depthMask), m_clearColorBuffer(rs.m_clearColorBuffer), m_clearDepth(rs.m_clearDepth),
			m_clearDepthBuffer(rs.m_clearDepthBuffer), m_clearStencil(rs.m_clearStencil), m_clearStencilBuffer(
					rs.m_clearStencilBuffer), m_shadeless(rs.m_shadeless) {
	m_colorMask[0] = rs.m_colorMask[0];
	m_colorMask[1] = rs.m_colorMask[1];
	m_colorMask[2] = rs.m_colorMask[2];
	m_colorMask[3] = rs.m_colorMask[3];
}

bool RenderState::operator==(const RenderState& rs) {
	return m_depthTest == rs.m_depthTest && m_scissor == rs.m_scissor && m_stencil == rs.m_stencil && m_cullFace
			== rs.m_cullFace && m_frontFace == rs.m_frontFace && m_renderMode == rs.m_renderMode && m_shadeless
			== rs.m_shadeless && m_clearColorBuffer == rs.m_clearColorBuffer && m_clearDepthBuffer
			== rs.m_clearDepthBuffer && m_clearStencilBuffer == rs.m_clearStencilBuffer && m_depthMask
			== rs.m_depthMask && m_depthFunc == rs.m_depthFunc && m_clearColor == rs.m_clearColor
			&& m_colorMask[0] == rs.m_colorMask[0] && m_colorMask[1] == rs.m_colorMask[1]
            && m_colorMask[2] == rs.m_colorMask[2] && m_colorMask[3] == rs.m_colorMask[3];
}
