{{! to be applied multiple times with a different dictionary on each invocation }}
{{! 
Markers index

TEX_BLENDOP_MULTIPLY: marks the section that implements the multiply (GL_MODULATE) texture application
TEX_INDEX: the index of the texture
TEX_COORDS: the swizzle of the texture coordinates, e.g. xy, x, st, etc.
}}
{{#TEX_MAP_TO_SPECULAR}}
specular *= texture(u_Sampler{{TEX_INDEX}}, ogl_texCoords[{{UV_SET_INDEX}}].{{TEX_COORDS}});
{{/TEX_MAP_TO_SPECULAR}}

{{#TEX_MAP_TO_SHININESS}}
shininess *= texture(u_Sampler{{TEX_INDEX}}, ogl_texCoords[{{UV_SET_INDEX}}].{{TEX_COORDS}});
{{/TEX_MAP_TO_SHININESS}}

{{#TEX_MAP_TO_DIFFUSE}}

{{#TEX_BLENDOP_MULTIPLY}}
texDiffuse *= texture(u_Sampler{{TEX_INDEX}}, u_TexCoord[{{UV_SET_INDEX}}].{{TEX_COORDS}});
{{/TEX_BLENDOP_MULTIPLY}}

{{#TEX_BLENDOP_DECAL}}
vec4 texColor{{TEX_INDEX}}= texture(u_Sampler{{TEX_INDEX}}, u_TexCoord[{{UV_SET_INDEX}}].{{TEX_COORDS}});
vec3 c{{TEX_INDEX}} = mix(texColor.rgb, texColor{{TEX_INDEX}}.rgb, texColor{{TEX_INDEX}}.a);
texDiffuse = vec4(c{{TEX_INDEX}}, texDiffuse.a);
{{/TEX_BLENDOP_DECAL}}

{{#TEX_BLENDOP_BLEND}}
vec4 texColor{{TEX_INDEX}}= texture(u_Sampler{{TEX_INDEX}}, u_TexCoord[{{UV_SET_INDEX}}].{{TEX_COORDS}});
vec3 c{{TEX_INDEX}} = mix(texColor{{TEX_INDEX}}.rgb, fs_texEnvColors[{{TEX_INDEX}}].rgb, texColor{{TEX_INDEX}}.a);
texDiffuse = vec4(c{{TEX_INDEX}}, texDiffuse.a * texColor{{TEX_INDEX}}.a);
{{/TEX_BLENDOP_BLEND}}

{{#TEX_BLENDOP_ADD}}
vec4 texColor{{TEX_INDEX}}= texture(u_Sampler{{TEX_INDEX}}, u_TexCoord[{{UV_SET_INDEX}}].{{TEX_COORDS}});
texDiffuse.rgb += texColor{{TEX_INDEX}}.rgb;
texDiffuse.a  *= texColor{{TEX_INDEX}}.a;
{{/TEX_BLENDOP_ADD}}

{{/TEX_MAP_TO_DIFFUSE}}

{{#TEX_MAP_TO_NORMAL}}
{{/TEX_MAP_TO_NORMAL}}
