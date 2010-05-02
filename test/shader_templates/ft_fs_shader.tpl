{{! 
Markers index

TEX_BLENDOP_MULTIPLY: marks the section that implements the multiply (GL_MODULATE) texture application
TEX_INDEX: the index of the texture
TEX_COORDS: the swizzle of the texture coordinates, e.g. xy, x, st, etc.
}}

#version 150 core

{{>FRAGMENT_INPUTS}}
{{UNIFORMS}}

{{#PHONG_LIGHTING}}
{{>LIGHTING}}
{{/PHONG_LIGHTING}}

out vec4 fs_FragColor;

{{! fragment shader }}
void main() {
	// receives the final color from the application of the diffuse maps
	vec4 texDiffuse = uvec4(1.0);

	// receives the total ambient radiance from all light sources
	vec4 ambient = vec4(0.0);

	// receives the total diffuse radiance from all light sources
	// if there are vertex colors then don't use u_Material.diffuse
	vec4 diffuse = vec4(0.0);

	// receives the total specular radiance from all light sources
	vec4 specular = vec4(0.0);

	float shininess = u_Material.shininess;

	{{#PHONG_LIGHTING}}
	vec3 N = normalize(fs_Normal);
	for (int i = 0; i < u_NumLights; i++) {
		//if (u_Lights[i].type == LT_Directional) {
			Directional_radiance(i, N, fs_eyePos, ambient, diffuse, specular);
	/*	} else if (u_Lights[i].type == LT_Lamp) {
			Lamp_radiance(i, fs_Normal, vec3(u_View[3]), vec3(gl_FragCoord), ambient, diffuse, specular);
		} else if (u_Lights[i].type == LT_Spot) {
			Spot_radiance(i, fs_Normal, vec3(u_View[3]), vec3(gl_FragCoord), ambient, diffuse, specular);
		}*/
	}
	{{/PHONG_LIGHTING}}

	{{! 
		TEXTURE_STACK_APPLICATION will be equal to the string result of applying template tex_stack.tpl
		as many times as there are textures that affect the final color and concatenating those strings together.
	}}

	{{>SINGLE_TEXTURE_STACK_ENTRY}}	


	{{#PHONG_LIGHTING}}
	texDiffuse *= vec4(diffuse.rgb, 1.0);
	{{/PHONG_LIGHTING}}

	fs_FragColor = /*vec4(1.0) +*/ ambient*u_Material.ambient + texDiffuse*u_Material.diffuse + specular*u_Material.specular;
//	fs_FragColor = specular*u_Material.specular;
//	if (fs_FragColor.a > 0.9) fs_FragColor.rgb = vec3(1.0, 0, 0);
	gl_FragDepth = gl_FragCoord.z;
}

