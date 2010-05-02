{{! 
Markers index

TEX_BLENDOP_MULTIPLY: marks the section that implements the multiply (GL_MODULATE) texture application
TEX_INDEX: the index of the texture
TEX_COORDS: the swizzle of the texture coordinates, e.g. xy, x, st, etc.
NUM_UV_SETS : number of UV coordinate sets
SHADE_MODE : specifies the interpolation qualifier for fs_color (flat, smooth, noperspective)

Vertex Shader Outputs / Fragment Shader Inputs
----------------------------------------------
vec3 fs_Normal: vertex normal
vec4 fs_Color: vertex color
vec2 fs_TexCoords[] : uv coords for UV sets
}}

// position of the shading point in eye/camera space
out vec3 fs_eyePos;

{{#HAS_NORMALS}}
{{#IN_DIRECTION}}in{{/IN_DIRECTION}} {{#OUT_DIRECTION}}out{{/OUT_DIRECTION}} vec3 fs_Normal;
{{/HAS_NORMALS}}

{{#HAS_COLORS}}
{{SHADE_MODE}} {{#IN_DIRECTION}}in{{/IN_DIRECTION}} {{#OUT_DIRECTION}}out{{/OUT_DIRECTION}} vec4 fs_Color;
{{/HAS_COLORS}}

{{#HAS_TEXTURES}}
{{#IN_DIRECTION}}in{{/IN_DIRECTION}} {{#OUT_DIRECTION}}out{{/OUT_DIRECTION}} vec2 fs_TexCoords[{{NUM_UV_SETS}}];
{{/HAS_TEXTURES}}
