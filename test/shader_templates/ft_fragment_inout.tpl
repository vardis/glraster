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

// position of the shading point in camera space
{{#IN_DIRECTION}}in{{/IN_DIRECTION}} {{#OUT_DIRECTION}}out{{/OUT_DIRECTION}} vec3 fs_FragCoordsView;

{{#HAS_NORMALS}}
{{#IN_DIRECTION}}in{{/IN_DIRECTION}} {{#OUT_DIRECTION}}out{{/OUT_DIRECTION}} vec3 fs_Normal;
{{/HAS_NORMALS}}

{{#HAS_COLORS}}
{{SHADE_MODE}} {{#IN_DIRECTION}}in{{/IN_DIRECTION}} {{#OUT_DIRECTION}}out{{/OUT_DIRECTION}} vec4 fs_Color;
{{/HAS_COLORS}}

{{#HAS_TEXTURES}}
{{#IN_DIRECTION}}in{{/IN_DIRECTION}} {{#OUT_DIRECTION}}out{{/OUT_DIRECTION}} vec4 fs_TexCoords[{{NUM_UV_SETS}}];
{{/HAS_TEXTURES}}

{{#NORMAL_MAPPING}}

// fragment coordinates in object space 
//{{#IN_DIRECTION}}in{{/IN_DIRECTION}} {{#OUT_DIRECTION}}out{{/OUT_DIRECTION}} vec3 fs_FragCoordsObj;

// TBN matrix
// the tangent, binormal and normal occupy the 0,1 and 2 rows , respectively
// therefore this matrix should transform vectors by multiplying from the right side
{{#IN_DIRECTION}}in{{/IN_DIRECTION}} {{#OUT_DIRECTION}}out{{/OUT_DIRECTION}} mat3 fs_TangentBasis;

// vertex coordinates in tangent space
{{#IN_DIRECTION}}in{{/IN_DIRECTION}} {{#OUT_DIRECTION}}out{{/OUT_DIRECTION}} vec3 tbn_vertexPos;


{{/NORMAL_MAPPING}}

