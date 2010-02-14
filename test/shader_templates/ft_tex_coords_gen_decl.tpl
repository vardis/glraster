vec2 SphereMap(in vec3 ecPosition3, in vec3 normal)
{
   float m;
   vec3 r, u;
   u = normalize(ecPosition3);
   r = reflect(u, normal);
   m = 2.0 * sqrt(r.x * r.x + r.y * r.y + (r.z + 1.0) * (r.z + 1.0));
   return vec2(r.x / m + 0.5, r.y / m + 0.5);
}

vec3 ReflectionMap(in vec3 ecPosition3, in vec3 normal)
{
   float NdotU, m;
   vec3 u;
   u = normalize(ecPosition3);
   return (reflect(u, normal));
}
