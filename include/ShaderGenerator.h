/*
 * ShaderGenerator.h
 *
 *  Created on: Jan 30, 2010
 *      Author: giorgos
 */

#ifndef SHADERGENERATOR_H_
#define SHADERGENERATOR_H_

#include "VertexFormat.h"
#include "Renderable.h"

class ShaderGenerator {
public:
	static const char* const TEX_COORDS;
	static const char* const TEX_INDEX;
	static const char* const UV_SET_INDEX;
	static const char* const S_COORD;
	static const char* const ST_COORDS;
	static const char* const STP_COORDS;
	static const char* const TANGENT_SPACE_NMAP_TPL;

	ShaderGenerator();
	virtual ~ShaderGenerator();

	void generateShaders(Material& mat, const RenderState& rs, const VertexFormat& vf);
};

#endif /* SHADERGENERATOR_H_ */
