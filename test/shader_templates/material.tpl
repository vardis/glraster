
struct Material_t {
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
	float opacity;
};

in Material_t u_Material;
