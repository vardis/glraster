/*
 * Lights.h
 *
 *  Created on: Jan 25, 2010
 *      Author: giorgos
 */

#ifndef LIGHTS_H_
#define LIGHTS_H_

#include "Vector.h"

typedef struct Light {

public:
	/** defines the type of light, e.g. directional or lamp. For safety, use the values declared in ShaderConstants. */
	int m_type;
	Vec3f m_position;
	Vec3f m_direction;
	Colour m_ambient;
	Colour m_diffuse;
	Colour m_specular;
	float m_cosSpotCutOff; // cosine of the spot cut-off angle
	float m_attenuation;
	float m_linearAttenuation;
	float m_quadraticAttenuation;

	bool m_active;

	/** indicates if the light affects only objects of this layer */
	bool m_layerOnly;

	/** the render layer's name */
	String m_layer;

	/** indicates if this lights casts shadows */
	bool m_castShadows;

	/** valid for pre 3.2 OGL */
	virtual void configureGLState() = 0;
}Light;

typedef shared_ptr<Light> LightPtr;

#endif /* LIGHTS_H_ */
