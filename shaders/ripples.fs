#version 330

uniform float Time;
in vec2 TexCoords;

uniform sampler2D bottomTexture;
uniform sampler2D waterTexture;
//uniform sampler2D tex2;

float radius = .5;

void main()
{
	float t = 0.4;//clamp(Time / 6., 0., 1.);

	vec2 dir = TexCoords - vec2(.5);

	float dist = distance(TexCoords, vec2(.5));
	vec2 offset = dir * (sin(dist * 80. - Time*15.) + .5) / 30.;

	vec2 texCoord = TexCoords + offset;
	vec4 diffuse = texture2D(bottomTexture, texCoord);

	vec4 mixin = texture2D(waterTexture, texCoord);

 	gl_FragColor = mixin * t + diffuse * (1. - t);
}
