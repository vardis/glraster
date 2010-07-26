{{! 
Markers index

Vertex Shader Inputs
--------------------
vec3 vs_VertexPos: vertex position
vec3 vs_Normal: vertex normal
vec4 vs_Color: vertex color 
vec2 vs_TexCoords[NUM_UV_SETS] : array of texture coordinates
}}

#version 150 core

{{>VERTEX_INPUTS}}
{{>FRAGMENT_INPUTS}}
{{>UNIFORMS}}

{{>TEX_COORDS_GEN_DECL}}


{{! vertex shader }}
void main() {
	gl_Position = u_ModelViewProjection * vs_VertexPos;
	fs_FragCoordsView = vec3(u_ModelView * vs_VertexPos);

{{#HAS_NORMALS}}
	fs_Normal = u_NormalMatrix * vs_Normal;
//	fs_Normal = (u_ModelView * vec4(vs_Normal, 0.0)).xyz;

{{#NORMALIZE_NORMALS}} 
	fs_Normal = normalize(fs_Normal);
{{/NORMALIZE_NORMALS}}

{{/HAS_NORMALS}}

{{#NORMAL_MAPPING}}
//fs_FragCoordsObj = vs_VertexPos;
vec3 t = u_NormalMatrix * vs_Tangent;
vec3 b = cross(t, fs_Normal);
fs_TangentBasis = mat3(t, b, fs_Normal);
tbn_vertexPos = vs_VertexPos.xyz * fs_TangentBasis;
{{/NORMAL_MAPPING}}

{{#HAS_COLORS}}
	fs_Color = vs_Color;
{{/HAS_COLORS}}

{{>TEX_COORDS_GEN}}
}



