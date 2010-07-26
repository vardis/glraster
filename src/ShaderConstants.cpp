/*
 * ShaderConstants.cpp
 *
 *  Created on: Feb 2, 2010
 *      Author: giorgos
 */

#include "GLTutor.h"
#include "ShaderConstants.h"

const char* const ShaderConstants::VATTR_Pos = "vs_VertexPos";
const char* const ShaderConstants::VATTR_Normal = "vs_Normal";
const char* const ShaderConstants::VATTR_Tangent = "vs_Tangent";
const char* const ShaderConstants::VATTR_BiNormal = "vs_Binormal";
const char* const ShaderConstants::VATTR_Color = "vs_Color";
const char* const ShaderConstants::VATTR_TexCoords[8] = { "vs_TexCoords[0]", "vs_TexCoords[1]", "vs_TexCoords[2]",
															"vs_TexCoords[3]", "vs_TexCoords[4]", "vs_TexCoords[5]",
															"vs_TexCoords[6]", "vs_TexCoords[7]" };

const char* const ShaderConstants::Fragment_Color = "fs_FragColor";

const char* const ShaderConstants::UNIFORM_CameraPos = "u_camPos";

const char* const ShaderConstants::UNIFORM_ModelViewProj = "u_ModelViewProjection";
const char* const ShaderConstants::UNIFORM_ModelView = "u_ModelView";
const char* const ShaderConstants::UNIFORM_Model = "u_Model";
const char* const ShaderConstants::UNIFORM_ModelInverse = "u_ModelInv";
const char* const ShaderConstants::UNIFORM_View = "u_View";
const char* const ShaderConstants::UNIFORM_ViewInverse = "u_ViewInv";
const char* const ShaderConstants::UNIFORM_Proj = "u_Proj";

const char* const ShaderConstants::UNIFORM_NormalMatrix = "u_NormalMatrix";
const char* const ShaderConstants::UNIFORM_NumTextures = "u_NumTextures";

const char* const ShaderConstants::UNIFORM_MaterialAmbient = "u_Material.ambient";
const char* const ShaderConstants::UNIFORM_MaterialDiffuse = "u_Material.diffuse";
const char* const ShaderConstants::UNIFORM_MaterialSpecular = "u_Material.specular";
const char* const ShaderConstants::UNIFORM_MaterialShininess = "u_Material.shininess";
const char* const ShaderConstants::UNIFORM_MaterialOpacity = "u_Material.opacity";
const char* const ShaderConstants::UNIFORM_MaterialParallaxScaleBias = "u_Material.parallaxScaleBias";

const char* const ShaderConstants::UNIFORM_ObjectPlanes = "u_ObjectPlanes";
const char* const ShaderConstants::UNIFORM_EyePlanes = "u_EyePlanes";
const char* const ShaderConstants::UNIFORM_TextureMatrices = "u_TexMatrices";

const char* const ShaderConstants::UNIFORM_Samplers[MAX_TEXTURES_STACK] = { "u_Sampler0", "u_Sampler1", "u_Sampler2",
																			"u_Sampler3", "u_Sampler4", "u_Sampler5",
																			"u_Sampler6", "u_Sampler7" };

const char* const ShaderConstants::UNIFORM_TexEnvColors[MAX_TEXTURES_STACK] = { "u_TexEnvColors[0]",
																				"u_TexEnvColors[1]",
																				"u_TexEnvColors[2]",
																				"u_TexEnvColors[3]",
																				"u_TexEnvColors[4]",
																				"u_TexEnvColors[5]",
																				"u_TexEnvColors[6]",
																				"u_TexEnvColors[7]" };

const char* const ShaderConstants::UNIFORM_NumLights = "u_NumLights";

const char* const ShaderConstants::UNIFORM_LightsType[MAX_LIGHTS] = { 	"u_Lights[0].type",
																		"u_Lights[1].type",
																		"u_Lights[2].type",
																		"u_Lights[3].type",
																		"u_Lights[4].type",
																		"u_Lights[5].type",
																		"u_Lights[6].type",
																		"u_Lights[7].type" };

const char* const ShaderConstants::UNIFORM_LightsPos[MAX_LIGHTS] = { 	"u_Lights[0].pos",
																		"u_Lights[1].pos",
																		"u_Lights[2].pos",
																		"u_Lights[3].pos",
																		"u_Lights[4].pos",
																		"u_Lights[5].pos",
																		"u_Lights[6].pos",
																		"u_Lights[7].pos" };

const char* const ShaderConstants::UNIFORM_LightsDirection[MAX_LIGHTS] = { 	"u_Lights[0].dir",
																		"u_Lights[1].dir",
																		"u_Lights[2].dir",
																		"u_Lights[3].dir",
																		"u_Lights[4].dir",
																		"u_Lights[5].dir",
																		"u_Lights[6].dir",
																		"u_Lights[7].dir" };

const char* const ShaderConstants::UNIFORM_LightsWorldPos[MAX_LIGHTS] = { 	"u_Lights[0].world_pos",
																		"u_Lights[1].world_pos",
																		"u_Lights[2].world_pos",
																		"u_Lights[3].world_pos",
																		"u_Lights[4].world_pos",
																		"u_Lights[5].world_pos",
																		"u_Lights[6].world_pos",
																		"u_Lights[7].world_pos" };

const char* const ShaderConstants::UNIFORM_LightsWorldDirection[MAX_LIGHTS] = { 	"u_Lights[0].world_dir",
																		"u_Lights[1].world_dir",
																		"u_Lights[2].world_dir",
																		"u_Lights[3].world_dir",
																		"u_Lights[4].world_dir",
																		"u_Lights[5].world_dir",
																		"u_Lights[6].world_dir",
																		"u_Lights[7].world_dir" };


const char* const ShaderConstants::UNIFORM_LightsAmbient[MAX_LIGHTS] = { 	"u_Lights[0].ambient",
																		"u_Lights[1].ambient",
																		"u_Lights[2].ambient",
																		"u_Lights[3].ambient",
																		"u_Lights[4].ambient",
																		"u_Lights[5].ambient",
																		"u_Lights[6].ambient",
																		"u_Lights[7].ambient" };

const char* const ShaderConstants::UNIFORM_LightsDiffuse[MAX_LIGHTS] = { 	"u_Lights[0].diffuse",
																		"u_Lights[1].diffuse",
																		"u_Lights[2].diffuse",
																		"u_Lights[3].diffuse",
																		"u_Lights[4].diffuse",
																		"u_Lights[5].diffuse",
																		"u_Lights[6].diffuse",
																		"u_Lights[7].diffuse" };

const char* const ShaderConstants::UNIFORM_LightsSpecular[MAX_LIGHTS] = { 	"u_Lights[0].specular",
																		"u_Lights[1].specular",
																		"u_Lights[2].specular",
																		"u_Lights[3].specular",
																		"u_Lights[4].specular",
																		"u_Lights[5].specular",
																		"u_Lights[6].specular",
																		"u_Lights[7].specular" };

const char* const ShaderConstants::UNIFORM_LightsCosSpotCutOff[MAX_LIGHTS] = { 	"u_Lights[0].cosSpotCutOff",
																		"u_Lights[1].cosSpotCutOff",
																		"u_Lights[2].cosSpotCutOff",
																		"u_Lights[3].cosSpotCutOff",
																		"u_Lights[4].cosSpotCutOff",
																		"u_Lights[5].cosSpotCutOff",
																		"u_Lights[6].cosSpotCutOff",
																		"u_Lights[7].cosSpotCutOff" };

const char* const ShaderConstants::UNIFORM_LightsCosOuterConeSpotCutOff[MAX_LIGHTS] = { 	"u_Lights[0].cosOuterConeSpotCutOff",
																		"u_Lights[1].cosOuterConeSpotCutOff",
																		"u_Lights[2].cosOuterConeSpotCutOff",
																		"u_Lights[3].cosOuterConeSpotCutOff",
																		"u_Lights[4].cosOuterConeSpotCutOff",
																		"u_Lights[5].cosOuterConeSpotCutOff",
																		"u_Lights[6].cosOuterConeSpotCutOff",
																		"u_Lights[7].cosOuterConeSpotCutOff" };

const char* const ShaderConstants::UNIFORM_LightsConstAttenuation[MAX_LIGHTS] = { 	"u_Lights[0].constAttenuation",
																		"u_Lights[1].constAttenuation",
																		"u_Lights[2].constAttenuation",
																		"u_Lights[3].constAttenuation",
																		"u_Lights[4].constAttenuation",
																		"u_Lights[5].constAttenuation",
																		"u_Lights[6].constAttenuation",
																		"u_Lights[7].constAttenuation" };

const char* const ShaderConstants::UNIFORM_LightsLinearAttenuation[MAX_LIGHTS] = { 	"u_Lights[0].linearAttenuation",
																		"u_Lights[1].linearAttenuation",
																		"u_Lights[2].linearAttenuation",
																		"u_Lights[3].linearAttenuation",
																		"u_Lights[4].linearAttenuation",
																		"u_Lights[5].linearAttenuation",
																		"u_Lights[6].linearAttenuation",
																		"u_Lights[7].linearAttenuation" };

const char* const ShaderConstants::UNIFORM_LightsQuadraticAttenuation[MAX_LIGHTS] = { 	"u_Lights[0].quadraticAttenuation",
																		"u_Lights[1].quadraticAttenuation",
																		"u_Lights[2].quadraticAttenuation",
																		"u_Lights[3].quadraticAttenuation",
																		"u_Lights[4].quadraticAttenuation",
																		"u_Lights[5].quadraticAttenuation",
																		"u_Lights[6].quadraticAttenuation",
																		"u_Lights[7].quadraticAttenuation" };
