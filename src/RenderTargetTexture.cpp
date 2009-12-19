/*
 * RenderTargetTexture.cpp
 *
 *  Created on: Dec 12, 2009
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "RenderTargetTexture.h"

RenderTargetTexture::RenderTargetTexture(TexturePtr texture, bool useDepth, uint8_t multisample) :
	m_texture(texture), m_useDepth(useDepth), m_multisample(multisample), m_fbo(0), m_depthBuffer(0) {
}

RenderTargetTexture::~RenderTargetTexture() {
	if (m_fbo && glIsFramebuffer(m_fbo)) {
		glDeleteFramebuffers(1, &m_fbo);
	}

	if (m_depthBuffer && glIsRenderbuffer(m_depthBuffer)) {
		glDeleteRenderbuffers(1, &m_depthBuffer);
	}
}

bool RenderTargetTexture::allocate() {
	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

//	glBindTexture(m_texture->m_textureTarget, m_texture->m_texID);
	switch (m_texture->m_textureTarget) {
	case GL_TEXTURE_1D:
		glFramebufferTexture1D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texture->m_textureTarget,
				m_texture->m_texID, 0);
		break;
	case GL_TEXTURE_2D:
	default:
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texture->m_textureTarget,
				m_texture->m_texID, 0);
	}
	if (m_texture->m_useMipmaps) {
		glGenerateMipmap(m_texture->m_textureTarget);
	}
	if (m_useDepth) {
		glGenRenderbuffers(1, &m_depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_multisample, GL_DEPTH_COMPONENT32, m_texture->m_width,
				m_texture->m_height);
		glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
	}
	GLenum status = _checkFramebufferStatus();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	return status == GL_FRAMEBUFFER_COMPLETE;
}

GLenum RenderTargetTexture::_checkFramebufferStatus() {
	GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	switch (status) {
	case GL_FRAMEBUFFER_UNDEFINED:
		std::cerr << "GL_FRAMEBUFFER_UNDEFINED" << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT" << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << std::endl;
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		std::cerr << "GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << std::endl;
		break;
	case 0:
		std::cerr << "Failed to check status of the Framebuffer object " << m_fbo << std::endl;
		break;
	}
	return status;
}

void RenderTargetTexture::bind() {
	// bind buffer for writing
	assert(m_fbo && glIsFramebuffer(m_fbo));
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);

	switch (m_texture->m_textureTarget) {
	case GL_TEXTURE_1D:
		glFramebufferTexture1D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texture->m_textureTarget,
				m_texture->m_texID, 0);
		break;
	case GL_TEXTURE_2D:
	default:
		glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, m_texture->m_textureTarget,
				m_texture->m_texID, 0);
	}

	GLbitfield clearMask = GL_COLOR_BUFFER_BIT;
	if (m_useDepth) {
		clearMask |= GL_DEPTH_BUFFER_BIT;
		glEnable(GL_DEPTH_TEST);
	}

	glClear(clearMask);

}

void RenderTargetTexture::unbind() {
	if (m_texture->m_useMipmaps) {
		glGenerateMipmap(m_texture->m_textureTarget);
	}

	if (m_fbo) {
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	}
}
