
#define MAX_LIGHTS 8


/**
 * Calculates the incoming radiance from a directional light.
 * @param lightIdx the index of the light inside the uniform u_Lights array
 * @param normal the normal at the current shading point
 * @param eyeDir the unit vector from the fragment to the eye posistion
 * @param lightDir the unit vector indicating the direction of the light source
 * @param ambient the accumulated ambient radiance
 * @param diffuse the accumulated diffuse radiance
 * @param specular the accumulated specular radiance
 */
void Directional_radiance(in int lightIdx, in vec3 normal, in vec3 eyeDir, in vec3 lightDir, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular) 
{
	float fd = clamp(max(0.0, dot(normal, -lightDir)), 0.0, 1.0);
	if (fd > 0.0) 
	{
		// light's half vector
//		vec3 hv = normalize(-shadePoint - lightDir);
		vec3 hv = normalize(reflect(lightDir, normal));
		float fs = clamp(max(0.0, dot(hv, eyeDir)), 0.0, 1.0); //normal, hv));
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
 * @param camPos the camera's position
 * @param shadePoint the position of the point being shaded
 * @param lightPos the light's position
 * @param ambient the accumulated ambient radiance
 * @param diffuse the accumulated diffuse radiance
 * @param specular the accumulated specular radiance
 */
void Lamp_radiance(in int lightIdx, in vec3 normal, in vec3 camPos, in vec3 shadePoint, in vec3 lightPos, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular) 
{
	// normal vector from shade point to light
	vec3 vl = normalize(lightPos - shadePoint);

	float fd = clamp(max(0.0, dot(normal, vl)), 0.0, 1.0);
	if (fd > 0.0)
	{
		// normal vector from shade point to eye
		vec3 ve = normalize(camPos - shadePoint);

		// light's half vector
		vec3 hv = normalize(ve + vl);

		{{#WITH_LIGHT_ATTENUATION}}
		float dist = distance(shadePoint, lightPos);
		float attenuation = 1.0 / (u_Lights[lightIdx].constAttenuation + dist * u_Lights[lightIdx].linearAttenuation + dist * dist * u_Lights[lightIdx].quadraticAttenuation);
		{{/WITH_LIGHT_ATTENUATION}}

		{{#WITHOUT_LIGHT_ATTENUATION}}
		float attenuation = 1.0 / u_Lights[lightIdx].constAttenuation;
		{{/WITHOUT_LIGHT_ATTENUATION}}

		diffuse += attenuation * fd * u_Lights[lightIdx].diffuse;
		
		float fs = clamp(max(0.0, dot(hv, normal)), 0.0, 1.0);
		fs = pow(fs, u_Material.shininess);
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
 * @param lightDir the unit vector indicating the direction of the light source
 * @param lightPos the light's position
 * @param ambient the accumulated ambient radiance
 * @param diffuse the accumulated diffuse radiance
 * @param specular the accumulated specular radiance
 */
void Spot_radiance(in int lightIdx, in vec3 normal, in vec3 eyePos, in vec3 shadePoint, in vec3 lightDir, in vec3 lightPos, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular) 
{
	// normal vector from shade point to light
	vec3 vl = normalize(lightPos - shadePoint);
	float fd = max(0.0, dot(normal, vl));
	if (fd > 0.0)
	{
		float cosSpotAngle  = dot(-vl, normalize(lightDir));

		// normal vector from shade point to eye
		vec3 ve = normalize(eyePos - shadePoint);

		// light's half vector
		vec3 hv = normalize(ve + vl);

		// decrease the light intensity linearly between the inner and outer cones
		float cosInnerMinusOuter = u_Lights[lightIdx].cosSpotCutOff - u_Lights[lightIdx].cosOuterConeSpotCutOff;
		float spotAttenuation = clamp((cosSpotAngle - u_Lights[lightIdx].cosOuterConeSpotCutOff) / cosInnerMinusOuter, 0.0, 1.0);

		{{#WITH_LIGHT_ATTENUATION}}
		float dist = distance(shadePoint, lightPos);
		float attenuation = 1.0 / (u_Lights[lightIdx].constAttenuation + dist * u_Lights[lightIdx].linearAttenuation + dist * dist * u_Lights[lightIdx].quadraticAttenuation);
		{{/WITH_LIGHT_ATTENUATION}}

		{{#WITHOUT_LIGHT_ATTENUATION}}
		float attenuation = 1.0 / u_Lights[lightIdx].constAttenuation;
		{{/WITHOUT_LIGHT_ATTENUATION}}

		diffuse += spotAttenuation * attenuation * fd * u_Lights[lightIdx].diffuse;
	
		float fs = max(0.0, dot(hv, normal));
		fs = pow(fs, u_Material.shininess);			
		specular += spotAttenuation * attenuation * fs * u_Lights[lightIdx].specular;
	}
	ambient += u_Lights[lightIdx].ambient;	
}

