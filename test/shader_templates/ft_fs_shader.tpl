{{! 
Markers index

TEX_BLENDOP_MULTIPLY: marks the section that implements the multiply (GL_MODULATE) texture application
TEX_INDEX: the index of the texture
TEX_COORDS: the swizzle of the texture coordinates, e.g. xy, x, st, etc.
}}

#version 150 core

{{>FRAGMENT_INPUTS}}
{{UNIFORMS}}

{{#USE_LIGHTING}}
{{>LIGHTING}}
{{/USE_LIGHTING}}

out vec4 fs_FragColor;

{{! fragment shader }}
void main() {

	{{#ALPHA_MAP_APPLICATION}}
	// alpha maps are applied early to avoid shading fragments that will eventually only be discarded 
	vec4 alpha = texture(u_Sampler{{TEX_INDEX}}, fs_TexCoords[{{UV_SET_INDEX}}].{{TEX_COORDS}});
	if (alpha.r < 0.1) discard;
	{{/ALPHA_MAP_APPLICATION}}

	// receives the final color from the application of the diffuse maps
	vec4 texDiffuse = vec4(1.0);

	// receives the total ambient radiance from all light sources
	vec4 ambient = vec4(0.0);

	// receives the total diffuse radiance from all light sources
	// if there are vertex colors then don't use u_Material.diffuse
	vec4 diffuse = vec4(0.0);

	// receives the total specular radiance from all light sources
	vec4 specular = vec4(0.0);

	// the normal used in shading calculations
	vec3 N;

//	float shininess = u_Material.shininess;	

	{{>TANGENT_SPACE_NORMAL_MAP}}

	{{#GEOMETRIC_NORMAL}}
	N = normalize(fs_Normal);
	{{/GEOMETRIC_NORMAL}}

	{{>PHONG_LIGHTING}}


	{{! 
		TEXTURE_STACK_APPLICATION will be equal to the string result of applying template tex_stack.tpl
		as many times as there are textures that affect the final color and concatenating those strings together.
	}}

	{{>SINGLE_TEXTURE_STACK_ENTRY}}	


	{{#USE_LIGHTING}}
	texDiffuse *= vec4(diffuse.rgb, 1.0);
	{{/USE_LIGHTING}}

	fs_FragColor = /*vec4(1.0) +*/ ambient*u_Material.ambient + texDiffuse*u_Material.diffuse + specular*u_Material.specular;

//	fs_FragColor = vec4(0.5*(fs_FragCoordsView + 1.0), 1.0);
//	fs_FragColor = vec4(normalize(-fs_FragCoordsView), 1.0);
//	if (fs_FragColor.a > 0.9) fs_FragColor.rgb = vec3(1.0, 0, 0);
	gl_FragDepth = gl_FragCoord.z;
}

