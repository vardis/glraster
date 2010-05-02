/*
 * RenderState.h
 *
 *  Created on: Feb 8, 2010
 *      Author: giorgos
 */

#ifndef RENDERSTATE_H_
#define RENDERSTATE_H_

#include "Colour.h"

/**
 * Encapsulates the values of the various OGL render states and offers a simple interface for changing
 * a set of render states or capture the current OGL render state in an instance of this class.
 */
class RenderState {
public:
	static const int MAX_CLIP_PLANES = 8;

protected:
	/** user clip planes */
	float m_clipPlanes[MAX_CLIP_PLANES][4];

	/** render mode for polygons: GL_POINT, GL_LINE, GL_FILL */
	GLenum m_renderMode;

	/** indicates if cull should be enabled */
	bool m_culling;

	/** specifies which faces are culled: GL_FRONT, GL_BACK, GL_FRONT_AND_BACK */
	GLenum m_cullFace;

	/** specifies the winding order that defines front facing facesL GL_CW, GL_CCW */
	GLenum m_frontFace;

	/** width of lines for line drawing render mode */
	float m_lineWidth;

	/** size of points */
	float m_pointSize;

	/** indicates if scissor test is enabled or not */
	bool m_scissor;

	/** origin and dimensions of the scissor rectangle */
	float m_scissorLeft, m_scissorBottom, m_scissorWidth, m_scissorHeight;

	/** indicates if stencil test is enabled or not */
	bool m_stencil;

	/** the type of the stencil function: NEVER, ALWAYS, LESS, LEQUAL, EQUAL, GREATER, GEQUAL */
	GLenum m_stencilFunc;

	/** the reference value used in the stencil tests */
	int m_stencilRef;

	/** the stencil mask used in the stencil tests */
	uint m_stencilMask;

	/** stencil operation to perform upon stencil fail: KEEP, ZERO, REPLACE, INCR, DECR, INVERT, INCR_WRAP, DECR_WRAP */
	GLenum m_stencilOpFail;

	/** stencil operation to perform upon stencil pass: KEEP, ZERO, REPLACE, INCR, DECR, INVERT, INCR_WRAP, DECR_WRAP */
	GLenum m_stencilOpPass;

	/** indicates if depth testing is enabled or not */
	bool m_depthTest;

	/** depth comparison function to use: NEVER, ALWAYS, LESS, LEQUAL, EQUAL, GREATER, GEQUAL */
	GLenum m_depthFunc;

	/** indicates if writing to the individual channels of the color  buffer is enabled or not */
	bool m_colorMask[4];

	/** indicates if writing to the depth buffer is enabled or not */
	bool m_depthMask;

	/** clear value for the color buffer */
	Colour m_clearColor;

	/** indicates if the color buffer should be cleared or not */
	bool m_clearColorBuffer;

	/** clear value for the depth buffer */
	float m_clearDepth;

	/** indicates if the depth buffer should be cleared or not */
	bool m_clearDepthBuffer;

	/** clear value for the stencil buffer */
	int m_clearStencil;

	/** indicates if the stencil buffer should be cleared or not */
	bool m_clearStencilBuffer;

	/** indicates if lighting should not be used */
	bool m_shadeless;

	/** indicates if alpha blending should be used */
	bool m_blend;

	/** blending function too use for incoming fragments */
	GLenum m_blenSrcFunc;

	/** blending function too use for existing fragments */
	GLenum m_blenDstFunc;

public:
	RenderState();
	RenderState(const RenderState& rs);

	bool operator==(const RenderState& rs);

	GLenum getBlenDstFunc() const {
		return m_blenDstFunc;
	}

	GLenum getBlenSrcFunc() const {
		return m_blenSrcFunc;
	}

	bool getBlend() const {
		return m_blend;
	}

	bool getClearColorBuffer() const {
		return m_clearColorBuffer;
	}

	bool getClearDepthBuffer() const {
		return m_clearDepthBuffer;
	}

	bool getClearStencilBuffer() const {
		return m_clearStencilBuffer;
	}

	bool getStencil() const {
		return m_stencil;
	}

	void setClearColorBuffer(bool clearColorBuffer) {
		this->m_clearColorBuffer = clearColorBuffer;
	}

	void setClearDepthBuffer(bool clearDepthBuffer) {
		this->m_clearDepthBuffer = clearDepthBuffer;
	}

	void setClearStencilBuffer(bool clearStencilBuffer) {
		this->m_clearStencilBuffer = clearStencilBuffer;
	}

	void setStencil(bool stencil) {
		this->m_stencil = stencil;
	}

	Colour getClearColor() const {
		return m_clearColor;
	}

	float getClearDepth() const {
		return m_clearDepth;
	}

	int getClearStencil() const {
		return m_clearStencil;
	}

	bool getColorMask() const {
		return m_colorMask;
	}

	bool getCulling() const {
		return m_culling;
	}

	GLenum getCullFace() const {
		return m_cullFace;
	}

	GLenum getDepthFunc() const {
		return m_depthFunc;
	}

	bool getDepthMask() const {
		return m_depthMask;
	}

	bool getDepthTest() const {
		return m_depthTest;
	}

	GLenum getFrontFace() const {
		return m_frontFace;
	}

	float getLineWidth() const {
		return m_lineWidth;
	}

	float getPointSize() const {
		return m_pointSize;
	}

	GLenum getRenderMode() const {
		return m_renderMode;
	}

	bool getScissor() const {
		return m_scissor;
	}

	float getScissorLeft() const {
		return m_scissorLeft;
	}

	float getScissorBottom() const {
		return this->m_scissorBottom;
	}

	float getScissorWidth() const {
		return this->m_scissorWidth;
	}

	float getScissorHeight() const {
		return this->m_scissorHeight;
	}

	GLenum getStencilFunc() const {
		return m_stencilFunc;
	}

	uint getStencilMask() const {
		return m_stencilMask;
	}

	GLenum getStencilOpFail() const {
		return m_stencilOpFail;
	}

	GLenum getStencilOpPass() const {
		return m_stencilOpPass;
	}

	int getStencilRef() const {
		return m_stencilRef;
	}

	bool isShadeless() const {
		return m_shadeless;
	}

	void setClearColor(Colour clearColor) {
		this->m_clearColor = clearColor;
	}

	void setClearDepth(float clearDepth) {
		this->m_clearDepth = clearDepth;
	}

	void setClearStencil(int clearStencil) {
		this->m_clearStencil = clearStencil;
	}

	void setCulling(bool cull) {
		this->m_culling = cull;
	}

	void setCullFace(GLenum cullFace) {
		this->m_cullFace = cullFace;
	}

	void setDepthFunc(GLenum depthFunc) {
		this->m_depthFunc = depthFunc;
	}

	void setDepthMask(bool depthMask) {
		this->m_depthMask = depthMask;
	}

	void setDepthTest(bool depthTest) {
		this->m_depthTest = depthTest;
	}

	void setFrontFace(GLenum frontFace) {
		this->m_frontFace = frontFace;
	}

	void setLineWidth(float lineWidth) {
		this->m_lineWidth = lineWidth;
	}

	void setPointSize(float pointSize) {
		this->m_pointSize = pointSize;
	}

	void setRenderMode(GLenum renderMode) {
		this->m_renderMode = renderMode;
	}

	void setScissor(bool scissor) {
		this->m_scissor = scissor;
	}

	void setScissorLeft(float scissorLeft) {
		this->m_scissorLeft = scissorLeft;
	}

	void setScissorBottom(float scissorBottom) {
		this->m_scissorBottom = scissorBottom;
	}

	void setScissorWidth(float scissorWidth) {
		this->m_scissorWidth = scissorWidth;
	}

	void setScissorHeight(float scissorHeight) {
		this->m_scissorHeight = scissorHeight;
	}

	void setStencilFunc(GLenum stencilFunc) {
		this->m_stencilFunc = stencilFunc;
	}

	void setStencilMask(uint stencilMask) {
		this->m_stencilMask = stencilMask;
	}

	void setStencilOpFail(GLenum stencilOpFail) {
		this->m_stencilOpFail = stencilOpFail;
	}

	void setStencilOpPass(GLenum stencilOpPass) {
		this->m_stencilOpPass = stencilOpPass;
	}

	void setStencilRef(int stencilRef) {
		this->m_stencilRef = stencilRef;
	}

	void setShadeless(bool val) {
		this->m_shadeless = val;
	}

	void setBlenDstFunc(GLenum blenDstFunc) {
		this->m_blenDstFunc = blenDstFunc;
	}

	void setBlenSrcFunc(GLenum blenSrcFunc) {
		this->m_blenSrcFunc = blenSrcFunc;
	}

	void setBlend(bool blend) {
		this->m_blend = blend;
	}
};

#endif /* RENDERSTATE_H_ */
