	for (int i = 0; i < u_NumLights; i++) {
		if (u_Lights[i].type == LT_Directional) {
		  	{{#NORMAL_MAPPING}}
			// transform the light's direction from world to model space and then to tangent space
			vec3 nm_lightDir = normalize((u_ModelInv*u_Lights[i].world_dir) * fs_TangentBasis);
			Directional_radiance(i, N, tbn_vertexPos, nm_lightDir, ambient, diffuse, specular);
		    {{/NORMAL_MAPPING}}

			{{#VIEW_SPACE_LIGHTING}}
			Directional_radiance(i, N, fs_FragCoordsView, u_Lights[i].dir, ambient, diffuse, specular);
			{{/VIEW_SPACE_LIGHTING}}
		} /*else if (u_Lights[i].type == LT_Lamp) {
			Lamp_radiance(i, fs_Normal, vec3(u_View[3]), vec3(gl_FragCoord), ambient, diffuse, specular);
		} else if (u_Lights[i].type == LT_Spot) {
			Spot_radiance(i, fs_Normal, vec3(u_View[3]), vec3(gl_FragCoord), ambient, diffuse, specular);
		}*/
	}
