#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

//uniform vec3 offset;
//uniform vec2 size;
uniform mat4 rotationMatrix;

out vec3 ourColor;

void main(){
    gl_Position = rotationMatrix * vec4(aPos, 1.0f);
    ourColor=aColor;
}