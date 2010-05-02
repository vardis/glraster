
{{! to be applied multiple times with a different dictionary on each invocation }}
{{#SINGLE_TEXCOORDS_ASSIGN}}

	{{#UV_MAPPING}}
	fs_TexCoords[{{UV_SET_INDEX}}] = vs_TexCoords[{{UV_SET_INDEX}}];
	{{/UV_MAPPING}}

	{{#SPHERE_TEXGEN}}
	fs_TexCoords[{{UV_SET_INDEX}}] = SphereMap(vs_VertexPos, vs_Normal);
	{{/SPHERE_TEXGEN}}

	{{#REFLECTION_TEXGEN}}
	fs_TexCoords[{{UV_SET_INDEX}}] = vec4(ReflectionMap(vs_VertexPos, vs_Normal), 1.0);
	{{/REFLECTION_TEXGEN}}

	{{#OBJECT_TEXGEN}}
	fs_TexCoords[{{UV_SET_INDEX}}] = vec4(
		dot(vs_VertexPos, u_ObjectPlanes.s),
		dot(vs_VertexPos, u_ObjectPlanes.t),
		dot(vs_VertexPos, u_ObjectPlanes.r),
		dot(vs_VertexPos, u_ObjectPlanes.q)
	);
	{{/OBJECT_TEXGEN}}

	{{#EYE_TEXGEN}}
	fs_TexCoords[{{UV_SET_INDEX}}] = vec4(
		dot(vs_VertexPos, u_EyePlanes.s),
		dot(vs_VertexPos, u_EyePlanes.t),
		dot(vs_VertexPos, u_EyePlanes.r),
		dot(vs_VertexPos, u_EyePlanes.q)
	);
	{{/EYE_TEXGEN}}

	{{#NORMAL_TEXGEN}}
	fs_TexCoords[{{UV_SET_INDEX}}] = vec4(vs_Normal, 1.0);
	{{/NORMAL_TEXGEN}}

{{/SINGLE_TEXCOORDS_ASSIGN}}
