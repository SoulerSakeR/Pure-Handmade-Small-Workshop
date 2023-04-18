#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCord;
layout(location = 2) in vec3 aPosBox;
out vec4 ourColor;
out vec2 TexCord;

uniform mat4 MVPMatrix;
uniform vec4 color;

void main(){
    gl_Position = MVPMatrix * vec4(aPos, 1.0f);
    ourColor=color;
    TexCord=aTexCord;
}
