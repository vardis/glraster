{{! 
Markers index

Vertex Shader Inputs
--------------------
vec3 vs_VertexPos: vertex position
vec3 vs_Normal: vertex normal
vec4 vs_Color: vertex color 
vec4 vs_TexCoords[NUM_UV_SETS] : array of texture coordinates
}}

#version 150

{{>VERTEX_INPUTS}}
{{>FRAGMENT_INPUTS}}
{{>UNIFORMS}}

{{>TEX_COORDS_GEN_DECL}}


{{! vertex shader }}
void vs_main() {
	gl_Position = u_ModelViewProjection * vs_VertexPos;
	vec3 viewPos = u_ModelView * vs_VertexPos;

{{#HAS_NORMALS}}
	fs_normal = u_NormalMatrix * vs_Normal;

{{#NORMALIZE_NORMALS}} 
	fs_Normal = normalize(fs_Normal);
{{/NORMALIZE_NORMALS}}

{{/HAS_NORMALS}}

{{#HAS_COLORS}}
	fs_Color = vs_Color;
{{/HAS_COLORS}}

{{>TEX_COORDS_GEN}}

}

{{! fragment shader }}
void fs_main() {
}

