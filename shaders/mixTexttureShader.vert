#version 330

in vec3 aPos;
in vec2 aTexCoord;

out vec2 fragTexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    fragTexCoord = aTexCoord;
}

