#version 330

in vec2 fragTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 outputColor;

void main()
{
    vec4 color1 = texture(texture1, fragTexCoord);
    vec4 color2 = texture(texture2, fragTexCoord);

    outputColor = mix(color1, color2, color2.a);
    //outputColor = vec4(1.f,1.f,1.f,1.f);
}
