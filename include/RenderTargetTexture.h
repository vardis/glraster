/*
 * RenderTargetTexture.h
 *
 *  Created on: Dec 12, 2009
 *      Author: giorgos
 */

#ifndef RENDERTARGET_H_
#define RENDERTARGET_H_

#include "Texture.h"

class RenderTargetTexture {
public:
	TexturePtr m_texture;
	bool       m_useDepth;
	uint8_t    m_multisample;
	GLuint     m_fbo;
	GLuint     m_depthBuffer;

public:
	RenderTargetTexture(TexturePtr texture, bool useDepth, uint8_t multisample = 0);
	virtual ~RenderTargetTexture();

	/** Allocates graphics resources */
	bool allocate();

	/** Setups any graphics state in preparation for rendering to this target */
	void bind();

	/** Un-binds this target from any subsequent rendering operations */
	void unbind();

private:
	GLenum _checkFramebufferStatus();
};

#endif /* RENDERTARGET_H_ */
