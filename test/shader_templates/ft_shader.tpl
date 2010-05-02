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
	vec4 hPos = vec4(vs_VertexPos, 1.0);
	gl_Position = u_ModelViewProjection * hPos;
	fs_eyePos = vec3(u_ModelView * hPos);

{{#HAS_NORMALS}}
	fs_Normal = /*u_NormalMatrix * */vs_Normal;

{{#NORMALIZE_NORMALS}} 
	fs_Normal = normalize(fs_Normal);
{{/NORMALIZE_NORMALS}}

{{/HAS_NORMALS}}

{{#HAS_COLORS}}
	fs_Color = vs_Color;
{{/HAS_COLORS}}

{{>TEX_COORDS_GEN}}
}



