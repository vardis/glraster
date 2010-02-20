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
	ShaderGenerator();
	virtual ~ShaderGenerator();

	void generateShaders(const Material& mat, const VertexFormat& vf);
};

#endif /* SHADERGENERATOR_H_ */
