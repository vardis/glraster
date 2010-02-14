{{! 
Markers index

TEX_BLENDOP_MULTIPLY: marks the section that implements the multiply (GL_MODULATE) texture application
TEX_INDEX: the index of the texture
TEX_COORDS: the swizzle of the texture coordinates, e.g. xy, x, st, etc.
}}

#version 150 core

{{FRAGMENT_INPUTS}}
{{UNIFORMS}}

{{#PHONG_LIGHTING}}
{{>LIGHTING}}
{{/PHONG_LIGHTING}}

{{! fragment shader }}
void main() {
	vec4 texDiffuse = vec4(1.0);

	vec4 ambient = u_Material.ambient;
	vec4 diffuse = u_Material.diffuse;
	vec4 specular = u_Material.specular;
	float shininess = u_Material.shininess;

	{{! 
		TEXTURE_STACK_APPLICATION will be equal to the string result of applying template tex_stack.tpl
		as many times as there are textures that affect the final color and concatenating those strings together.
	}}

	{{>SINGLE_TEXTURE_STACK_ENTRY}}	

	{{#PHONG_LIGHTING}}
	for (int i = 0; i < u_NumLights; i++) {
		if (u_Lights[i].type == LT_Directional) {
			Directional_radiance(i, fs_normal, ambient, diffuse, specular);
		} else if (u_Lights[i].type == LT_Lamp) {
			Lamp_radiance(i, fs_normal, u_View.w, gl_FragCoord, ambient, diffuse, specular);
		} else if (u_Lights[i].type == LT_Spot) {
			Spot_radiance(i, fs_normal, u_View.w, gl_FragCoord, ambient, diffuse, specular);
		}
	}
	{{/PHONG_LIGHTING}}

	gl_FragData[0] = ambient + texDiffuse*diffuse + specular;
	gl_FragDepth = gl_FragCoord.z;
}

