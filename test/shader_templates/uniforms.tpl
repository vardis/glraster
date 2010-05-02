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

row_major uniform mat4 u_Model;
row_major uniform mat4 u_View;
row_major uniform mat4 u_Proj;
row_major uniform mat4 u_ModelView;
row_major uniform mat4 u_ModelViewProjection;

struct Material_t {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
	float opacity;
};
uniform Material_t u_Material;

{{#HAS_NORMALS}}
row_major uniform mat3 u_NormalMatrix;
{{/HAS_NORMALS}}

{{#SINGLE_SAMPLER_DECL}}
uniform {{SAMPLER_SPEC}} u_Sampler{{TEX_INDEX}};
{{/SINGLE_SAMPLER_DECL}}

{{#HAS_TEXTURES}}
uniform vec4 u_TexEnvColors[{{NUM_TEXTURES}}];
{{/HAS_TEXTURES}}

uniform int u_NumTextures;

{{#USE_LIGHTING}}

// Important: these values must match the respective constants in Lights.h
#define LT_Directional 1
#define LT_Lamp 2
#define LT_Spot 3

// all vectors are specified in camera space and directional vectors are normalized
struct Light_t {
	int type;
	vec3 pos;	
	vec3 dir;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float cosSpotCutOff;	// cosine of the spot cut-off angle
	float constAttenuation;
	float linearAttenuation;
	float quadraticAttenuation;
};

uniform int u_NumLights;
uniform Light_t u_Lights[8]; 

{{/USE_LIGHTING}}

{{>MATERIAL_DECL}}

