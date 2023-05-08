#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;
layout(location = 2) in vec3 aPosBox;
out vec4 ourColor;
out vec2 TexCoord;

uniform bool isWorldPos;
uniform mat4 MVPMatrix;
uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec4 color;

void main(){
if(isWorldPos)
{
    gl_Position = projMatrix * viewMatrix * vec4(aPos, 1.0f);
}
else
{
    gl_Position = MVPMatrix * vec4(aPos, 1.0f);
}
    ourColor=color;
    TexCoord=aTexCoord;
}
