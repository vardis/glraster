
{{#FOG_LINEAR}}
float fogFactor = (u_Fog.fogEnd - abs(fs_FragCoordsView.z)) / (u_Fog.fogEnd - u_Fog.fogStart);
{{/FOG_LINEAR}}

{{#FOG_EXP}}
/*
fogFactor = exp(-(density * z)^2)

The exponential function can be written by a power of 2 :
	exp(x) = 2^(x/log(2)) 
	1/log(2) = 1.442695 
	exp(x) = 2^(1.442695 * x)
*/
float fogFactor = exp2(1.442695 * (- u_Fog.fogDensity * abs(fs_FragCoordsView.z)));
{{/FOG_EXP}}


{{#FOG_EXP2}}
float fogFactor = exp2(1.442695 * (- u_Fog.fogDensity * u_Fog.fogDensity * fs_FragCoordsView.z * fs_FragCoordsView.z));
{{/FOG_EXP2}}

{{#FOG_USER}}
// enter your formula here...
{{/FOG_USER}}

fogFactor = clamp(fogFactor, 0.0, 1.0);
fs_FragColor = mix(u_Fog.fogColor, fs_FragColor, fogFactor);
