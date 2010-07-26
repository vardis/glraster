	vec2 nmapUVs = fs_TexCoords[{{UV_SET_INDEX}}].{{TEX_COORDS}};
{{#PARALLAX_OFFSET_MAPPING}}

	float pax_scale = u_Material.parallaxScaleBias.x;
	float pax_bias = u_Material.parallaxScaleBias.y;

	// fetch a value from the parallax heightmap
	float pax_height = texture(u_Sampler{{TEX_INDEX}}, fs_TexCoords[{{UV_SET_INDEX}}].{{TEX_COORDS}}).r;
	float pax_offset = pax_height * pax_scale + pax_bias;

	// -tbn_vertexPos.xy is the projection of the fragment-to-eye vector on the triangle's surface
	vec2 nmapUVsOffset = -tbn_vertexPos.xy * pax_offset;

	nmapUVs += nmapUVsOffset;

{{/PARALLAX_OFFSET_MAPPING}}
	vec3 nmap = texture(u_Sampler{{TEX_INDEX}}, nmapUVs).rgb;
	N = normalize(2.0*nmap.rgb - 1.0);
