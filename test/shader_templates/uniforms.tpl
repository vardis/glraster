{{! 
Markers index

Uniforms
--------
mat4 u_TexMatrices[]: array of texture matrices
mat4 u_ModelViewProjection: model view projection matrix
mat4 u_ModelView:  model view matrix
mat4 u_View:  view matrix
mat4 u_Proj: projection matrix
mat4 u_NormalMatrix: vertex normal matrix
int  u_NumTextures: number of texture units used in the material
Material u_Material: the current material
mat4 u_ObjectPlanes: objects planes for texture gen
mat4 u_EyePlanes: eye planes for texture gen
vec4 u_TexEnvColors[] : environment colors for the texture units
Light_t u_Lights: lights that affect this object
NUM_TEXTURES : number of active texture units

}}

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_ModelView;
uniform mat4 u_ModelViewProjection;

{{#HAS_NORMALS}}
uniform mat3 u_NormalMatrix;
{{/HAS_NORMALS}}

{{#SINGLE_SAMPLER_DECL}}
uniform {{SAMPLER_SPEC}} u_Sampler{{TEX_INDEX}};
{{/SINGLE_SAMPLER_DECL}}

uniform vec4 u_TexEnvColors[{{NUM_TEXTURES}}];

uniform int u_NumTextures;

{{#USE_LIGHTING}}

// all vectors are specified in camera space and directional vectors are normalized
struct Light_t {
	vec3 pos;	// for directional lights this is equal to the direction 
	vec3 dir;
	vec4 diffuse;
	vec4 specular;
	float cosSpotCutOff;	// cosine of the spot cut-off angle
	float attenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

uniform Light_t u_Lights[]; 

{{/USE_LIGHTING}}

{{>MATERIAL_DECL}}

