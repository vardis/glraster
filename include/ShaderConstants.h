/*
 * ShaderConstants.h
 *
 *  Created on: Feb 2, 2010
 *      Author: giorgos
 */

#ifndef SHADERCONSTANTS_H_
#define SHADERCONSTANTS_H_

class ShaderConstants {
public:
	static const char* const VATTR_Pos;
	static const char* const VATTR_Normal;
	static const char* const VATTR_Color;
	static const char* const VATTR_TexCoords[8];

	static const char* const Fragment_Color;

	static const char* const UNIFORM_ModelViewProj;
	static const char* const UNIFORM_ModelView;
	static const char* const UNIFORM_Model;
	static const char* const UNIFORM_View;
	static const char* const UNIFORM_Proj;
	static const char* const UNIFORM_NormalMatrix;
	static const char* const UNIFORM_NumTextures;

	static const char* const UNIFORM_MaterialAmbient;
	static const char* const UNIFORM_MaterialDiffuse;
	static const char* const UNIFORM_MaterialSpecular;
	static const char* const UNIFORM_MaterialShininess;
	static const char* const UNIFORM_MaterialOpacity;

	static const char* const UNIFORM_ObjectPlanes;
	static const char* const UNIFORM_EyePlanes;
	static const char* const UNIFORM_TextureMatrices;

	static const char* const UNIFORM_Samplers[MAX_TEXTURES_STACK];

	static const char* const UNIFORM_TexEnvColors[MAX_TEXTURES_STACK];

	static const char* const UNIFORM_NumLights;


	static const char* const UNIFORM_LightsType[MAX_LIGHTS];
	static const char* const UNIFORM_LightsPos[MAX_LIGHTS];
	static const char* const UNIFORM_LightsDirection[MAX_LIGHTS];
	static const char* const UNIFORM_LightsAmbient[MAX_LIGHTS];
	static const char* const UNIFORM_LightsDiffuse[MAX_LIGHTS];
	static const char* const UNIFORM_LightsSpecular[MAX_LIGHTS];
	static const char* const UNIFORM_LightsCosSpotCutOff[MAX_LIGHTS];
	static const char* const UNIFORM_LightsConstAttenuation[MAX_LIGHTS];
	static const char* const UNIFORM_LightsLinearAttenuation[MAX_LIGHTS];
	static const char* const UNIFORM_LightsQuadraticAttenuation[MAX_LIGHTS];
};

#endif /* SHADERCONSTANTS_H_ */
