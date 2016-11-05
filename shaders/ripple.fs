#version 330

in vec2 TexCoords;

uniform float Time;
uniform vec2 Resolution;
uniform sampler2D bottomTexture;

layout (location = 0) out vec4 FragColor;


void main()
{
    vec2 cPos = -1.0 + 2.0 * gl_FragCoord.xy / Resolution.xy;
    float cLength = length(cPos);

    vec2 uv = gl_FragCoord.xy / Resolution.xy +
                (cPos/cLength) * cos(cLength * 12.0 - Time * 4.0)
                * 0.03;
    vec3 col = texture2D(bottomTexture, uv).xyz;
    //FragColor = texture(bottomTexture, TexCoords);
    FragColor = vec4(col, 1.0);
}
