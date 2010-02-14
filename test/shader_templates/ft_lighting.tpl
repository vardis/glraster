
#define MAX_LIGHTS 8


/**
 * Calculates the incoming radiance from a directional light.
 * @param lightIdx the index of the light inside the uniform ogl_Lights array
 * @param normal the normal at the current shading point
 * @param ambient the accumulated ambient radiance
 * @param diffuse the accumulated diffuse radiance
 * @param specular the accumulated specular radiance
 */
void Directional_radiance(in int lightIdx, in vec3 normal, inout vec4 ambient, inout vec4 diffuse, inout vec4 specular) 
{
	float fd = max(0.0, dot(normal, ogl_Lights[lightIdx].pos));
	if (fd > 0.0) 
	{
		float fs = max(0.0, dot(normal, ogl_Lights[lightIdx].halfVec));
		diffuse += fd*ogl_Lights[lightIdx].diffuse;
		specular += pow(fs, ogl_Material.shininess) * ogl_Lights[lightIdx].specular;
	}
	ambient += ogl_Lights[lightIdx].ambient;
}

/**
 * Calculates the incoming radiance from a positional light.
 * @param lightIdx the index of the light inside the uniform ogl_Lights array
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
	vec3 vl = normalize(ogl_Lights[lightIdx].pos - eyePos);

	// light's half vector
	vec3 hv = normalize(ve + vl);

	float fs = max(0.0, dot(normal, ve));
	if (fs > 0.0)
	{
		{{#WITH_LIGHT_ATTENUATION}}
		float attenuation = 1.0 / (ogl_Lights[lightIdx].constAttenuation + dist * ogl_Lights[lightIdx].linearAttenuation + dist * dist * ogl_Lights[lightIdx].quadraticAttenuation);
		{{/WITH_LIGHT_ATTENUATION}}

		{{#WITHOUT_LIGHT_ATTENUATION}}
		float attenuation = 1.0 / ogl_Lights[lightIdx].constAttenuation;
		{{/WITHOUT_LIGHT_ATTENUATION}}
		diffuse += attenuation * fs * ogl_Lights[lightIdx].diffuse;
		
		float fs = pow(dot(n, hv), ogl_Material.shininess);
		specular += attenuation * fs * ogl_Lights[lightIdx].specular;
	}
	ambient += ogl_Lights[lightIdx].ambient;
}

/**
 * Calculates the incoming radiance from a spotlight.
 * @param lightIdx the index of the light inside the uniform ogl_Lights array
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
	vec3 vl = normalize(ogl_Lights[lightIdx].pos - eyePos);

	// light's half vector
	vec3 hv = normalize(ve + vl);

	float spotAttenuation = dot(-vl, ogl_Lights[lightIdx].dir);
	if (spotAttenuation >= ogl_Lights[lightIdx].cosSpotCutOff)
	{
		float fs = max(0.0, dot(normal, ve));
		if (fs > 0.0)
		{
			{{#WITH_LIGHT_ATTENUATION}}
			float attenuation = 1.0 / (ogl_Lights[lightIdx].constAttenuation + dist * ogl_Lights[lightIdx].linearAttenuation + dist * dist * ogl_Lights[lightIdx].quadraticAttenuation);
			{{/WITH_LIGHT_ATTENUATION}}

			{{#WITHOUT_LIGHT_ATTENUATION}}
			float attenuation = 1.0 / ogl_Lights[lightIdx].constAttenuation;
			{{/WITHOUT_LIGHT_ATTENUATION}}

			attenuation *= spotAttenuation;
			diffuse += attenuation * fs * ogl_Lights[lightIdx].diffuse;
		
			float fs = pow(dot(n, hv), ogl_Material.shininess);
			specular += attenuation * fs * ogl_Lights[lightIdx].specular;
		}
	}
	ambient += ogl_Lights[lightIdx].ambient;	
}
