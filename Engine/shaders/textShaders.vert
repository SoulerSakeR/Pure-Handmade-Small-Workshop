#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCord;
layout(location = 3) in vec3 aPosBox;
out vec3 ourColor;
out vec2 TexCord;

uniform vec3 offset;
uniform vec2 size;
uniform mat4 MVPMatrix;

void main(){
    gl_Position = MVPMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
    ourColor=aColor;
    TexCord=vec2(aTexCord.x, 1 - aTexCord.y);
}