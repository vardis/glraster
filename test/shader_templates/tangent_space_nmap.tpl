	vec3 nmap = texture(u_Sampler{{TEX_INDEX}}, fs_TexCoords[{{UV_SET_INDEX}}].{{TEX_COORDS}}).rgb;
	N = normalize(2.0*nmap.rgb - 1.0);
{{!	N = normalize(nmap); }}
