
#define MAX_LIGHTS 8


/**
 * Calculates the incoming radiance from a directional light.
 * @param lightIdx the index of the light inside the uniform u_Lights array
 * @param normal the normal at the current shading point
 * @param shadePoint the position of the point being shaded
 * @param ambient the accumulated ambient radiance
 * @param diffuse the accumulated diffuse radiance
 * @param specular the accumulated specular radiance
 */
void Directional_radiance(in int lightIdx, in vec3 normal, in vec3 shadePoint, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular) 
{
	float fd = max(0.0, dot(normal, -u_Lights[lightIdx].dir));
	if (fd > 0.0) 
	{
		// light's half vector
		vec3 hv = normalize(-shadePoint - u_Lights[lightIdx].dir);
		float fs = max(0.0, dot(normal, hv));
		diffuse += fd*u_Lights[lightIdx].diffuse;
		if (fs > 0.0)
			specular += pow(fs, u_Material.shininess) * u_Lights[lightIdx].specular;
	} 
	ambient += u_Lights[lightIdx].ambient;
}

/**
 * Calculates the incoming radiance from a positional light.
 * @param lightIdx the index of the light inside the uniform u_Lights array
 * @param normal the normal at the current shading point
 * @param eyePos the camera's position
 * @param shadePoint the position of the point being shaded
 * @param ambient the accumulated ambient radiance
 * @param diffuse the accumulated diffuse radiance
 * @param specular the accumulated specular radiance
 */
void Lamp_radiance(in int lightIdx, in vec3 normal, in vec3 eyePos, in vec3 shadePoint, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular) 
{
	// normal vector from shade point to eye
	vec3 ve = normalize(eyePos - shadePoint);

	// normal vector from shade point to light
	vec3 vl = normalize(u_Lights[lightIdx].pos - eyePos);

	// light's half vector
	vec3 hv = normalize(ve + vl);

	float fd = max(0.0, dot(normal, ve));
	if (fd > 0.0)
	{
		{{#WITH_LIGHT_ATTENUATION}}
		float dist = distance(shadePoint, eyePos);
		float attenuation = 1.0 / (u_Lights[lightIdx].constAttenuation + dist * u_Lights[lightIdx].linearAttenuation + dist * dist * u_Lights[lightIdx].quadraticAttenuation);
		{{/WITH_LIGHT_ATTENUATION}}

		{{#WITHOUT_LIGHT_ATTENUATION}}
		float attenuation = 1.0 / u_Lights[lightIdx].constAttenuation;
		{{/WITHOUT_LIGHT_ATTENUATION}}
		diffuse += attenuation * fd * u_Lights[lightIdx].diffuse;
		
		float fs = pow(dot(normal, hv), u_Material.shininess);
		specular += attenuation * fs * u_Lights[lightIdx].specular;
	}
	ambient += u_Lights[lightIdx].ambient;
}

/**
 * Calculates the incoming radiance from a spotlight.
 * @param lightIdx the index of the light inside the uniform u_Lights array
 * @param normal the normal at the current shading point
 * @param eyePos the camera position
 * @param shadePoint the position of the point being shaded
 * @param ambient the accumulated ambient radiance
 * @param diffuse the accumulated diffuse radiance
 * @param specular the accumulated specular radiance
 */
void Spot_radiance(in int lightIdx, in vec3 normal, in vec3 eyePos, in vec3 shadePoint, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular) 
{
	// normal vector from shade point to eye
	vec3 ve = normalize(eyePos - shadePoint);

	// normal vector from shade point to light
	vec3 vl = normalize(u_Lights[lightIdx].pos - eyePos);

	// light's half vector
	vec3 hv = normalize(ve + vl);

	float spotAttenuation = dot(-vl, u_Lights[lightIdx].dir);
	if (spotAttenuation >= u_Lights[lightIdx].cosSpotCutOff)
	{
		float fd = max(0.0, dot(normal, ve));
		if (fd > 0.0)
		{
			{{#WITH_LIGHT_ATTENUATION}}
			float dist = distance(shadePoint, eyePos);
			float attenuation = 1.0 / (u_Lights[lightIdx].constAttenuation + dist * u_Lights[lightIdx].linearAttenuation + dist * dist * u_Lights[lightIdx].quadraticAttenuation);
			{{/WITH_LIGHT_ATTENUATION}}

			{{#WITHOUT_LIGHT_ATTENUATION}}
			float attenuation = 1.0 / u_Lights[lightIdx].constAttenuation;
			{{/WITHOUT_LIGHT_ATTENUATION}}

			attenuation *= spotAttenuation;
			diffuse += attenuation * fd * u_Lights[lightIdx].diffuse;
		
			float fs = pow(dot(normal, hv), u_Material.shininess);
			specular += attenuation * fs * u_Lights[lightIdx].specular;
		}
	}
	ambient += u_Lights[lightIdx].ambient;	
}
