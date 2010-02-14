in vec3 vs_Vertex; // vertex position

{{#HAS_NORMALS}}
in vec3 vs_Normal;
{{/HAS_NORMALS}}

{{#HAS_COLORS}}
in vec4 vs_Color;
{{/HAS_COLORS}}

{{#HAS_TEXTURES}}
in vec4 vs_TexCoords[{{NUM_UV_SETS}}];
{{/HAS_TEXTURES}}

