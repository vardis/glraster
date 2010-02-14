
{{! to be applied multiple times with a different dictionary on each invocation }}
{{#SINGLE_TEXCOORDS_ASSIGN}}

	{{#UV_MAPPING}}
	fs_TexCoords[{{UV_SET_INDEX}}] = vs_TexCoords[{{UV_SET_INDEX}}];
	{{/UV_MAPPING}}

	{{#SPHERE_TEXGEN}}
	fs_TexCoords[{{UV_SET_INDEX}}] = vec4(SphereMap(vs_Vertex, vs_Normal), 0.0, 1.0);
	{{/SPHERE_TEXGEN}}

	{{#REFLECTION_TEXGEN}}
	fs_TexCoords[{{UV_SET_INDEX}}] = vec4(ReflectionMap(vs_Vertex, vs_Normal), 1.0);
	{{/REFLECTION_TEXGEN}}

	{{#OBJECT_TEXGEN}}
	fs_TexCoords[{{UV_SET_INDEX}}] = vec4(
		dot(vs_Vertex, u_ObjectPlanes.s),
		dot(vs_Vertex, u_ObjectPlanes.t),
		dot(vs_Vertex, u_ObjectPlanes.r),
		dot(vs_Vertex, u_ObjectPlanes.q)
	);
	{{/OBJECT_TEXGEN}}

	{{#EYE_TEXGEN}}
	fs_TexCoords[{{UV_SET_INDEX}}] = vec4(
		dot(vs_Vertex, u_EyePlanes.s),
		dot(vs_Vertex, u_EyePlanes.t),
		dot(vs_Vertex, u_EyePlanes.r),
		dot(vs_Vertex, u_EyePlanes.q)
	);
	{{/EYE_TEXGEN}}

	{{#NORMAL_TEXGEN}}
	fs_TexCoords[{{UV_SET_INDEX}}] = vec4(vs_Normal, 1.0);
	{{/NORMAL_TEXGEN}}

{{/SINGLE_TEXCOORDS_ASSIGN}}
