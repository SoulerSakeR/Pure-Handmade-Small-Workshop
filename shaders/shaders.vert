#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCord;
out vec3 ourColor;
out vec2 TexCord;

uniform vec3 offset;
uniform vec2 size;

void main(){
    gl_Position = vec4(aPos.xy*size+offset.xy, aPos.z+offset.z, 1.0f);
    ourColor=aColor;
    TexCord=aTexCord;
}
