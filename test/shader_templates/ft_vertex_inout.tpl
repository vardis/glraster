in vec4 vs_VertexPos; // vertex position

{{#HAS_NORMALS}}
in vec3 vs_Normal;
{{/HAS_NORMALS}}

{{#HAS_TANGENTS}}
in vec3 vs_Tangent;
{{/HAS_TANGENTS}}

{{#HAS_BINORMALS}}
in vec3 vs_Binormal;
{{/HAS_BINORMALS}}

{{#HAS_COLORS}}
in vec4 vs_Color;
{{/HAS_COLORS}}

{{#HAS_TEXTURES}}
in vec2 vs_TexCoords[{{NUM_UV_SETS}}];
{{/HAS_TEXTURES}}


