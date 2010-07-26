  	{{#NORMAL_MAPPING}}
	// For point lights pre-calculate the camera position in tangent space
	//vec3 tbn_camPos = vec3(u_View * vec4(0.0, 0.0, 0.0, 1.0)) * fs_TangentBasis;
	vec3 tbn_camPos = (u_ModelInv * u_camPos) * fs_TangentBasis;
    {{/NORMAL_MAPPING}}

	for (int i = 0; i < u_NumLights; i++) {
		if (u_Lights[i].type == LT_Directional) {

			{{#VIEW_SPACE_LIGHTING}}
			Directional_radiance(i, N, normalize(-fs_FragCoordsView), u_Lights[i].dir, ambient, diffuse, specular);
			{{/VIEW_SPACE_LIGHTING}}

		  	{{#NORMAL_MAPPING}}
			// transform the light's direction from world to model space and then to tangent space
			vec3 tbn_lightDir = normalize((u_ModelInv*u_Lights[i].dir) * fs_TangentBasis);
			Directional_radiance(i, N, -normalize(tbn_vertexPos), tbn_lightDir, ambient, diffuse, specular);
		    {{/NORMAL_MAPPING}}			

		} else if (u_Lights[i].type == LT_Lamp) {

			{{#VIEW_SPACE_LIGHTING}}
			Lamp_radiance(i, N, vec3(0.0), fs_FragCoordsView, vec3(u_Lights[i].pos), ambient, diffuse, specular);
			{{/VIEW_SPACE_LIGHTING}}

		  	{{#NORMAL_MAPPING}}
			// light's position in tangent space
			vec3 tbn_lightPos = (u_ModelInv * vec3(u_Lights[i].world_pos)) * fs_TangentBasis;
			Lamp_radiance(i, N, tbn_camPos, tbn_vertexPos, tbn_lightPos, ambient, diffuse, specular);
		  	{{/NORMAL_MAPPING}}			

		} else if (u_Lights[i].type == LT_Spot) {

			{{#VIEW_SPACE_LIGHTING}}
			Spot_radiance(i, N, vec3(0), fs_FragCoordsView, u_Lights[i].dir, u_Lights[i].pos.xyz, ambient, diffuse, specular);
			{{/VIEW_SPACE_LIGHTING}}

		  	{{#NORMAL_MAPPING}}
			// light's position in tangent space
			vec3 tbn_lightPos = (u_ModelInv * vec3(u_Lights[i].world_pos)) * fs_TangentBasis;
			vec3 tbn_lightDir = normalize((u_ModelInv*u_Lights[i].world_dir) * fs_TangentBasis);
			Spot_radiance(i, N, tbn_camPos, tbn_vertexPos, tbn_lightDir, tbn_lightPos, ambient, diffuse, specular);
		  	{{/NORMAL_MAPPING}}
		}
	}
