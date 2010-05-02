
const vec3 Xunitvec = vec3(1.0, 0.0, 0.0);
const vec3 Yunitvec = vec3(0.0, 1.0, 0.0);


vec2 SphereMap(in vec3 ecPosition3, in vec3 normal)
{
   float m;
   vec3 r, u;
   u = normalize(ecPosition3);
   r = reflect(u, normal);
   //m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0));
  // return vec2(r.x / m + 0.5, r.y / m + 0.5);

	// Compute altitude and azimuth angles
	vec2 index;
	index.t = dot(normalize(r), Yunitvec);
	r.y = 0.0;
	index.s = dot(normalize(r), Xunitvec) * 0.5;

	// Translate index values into proper range
	if (r.z >= 0.0)
		index = (index + 1.0) * 0.5;
	else
	{
	  index.t = (index.t + 1.0) * 0.5;
	  index.s = (-index.s) * 0.5 + 1.0;
	}
	return index;
}

vec3 ReflectionMap(in vec3 ecPosition3, in vec3 normal)
{
   float NdotU, m;
   vec3 u;
   u = normalize(ecPosition3);
   return (reflect(u, normal));
}
