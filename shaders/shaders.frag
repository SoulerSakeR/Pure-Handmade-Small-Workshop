#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCod;
uniform sampler2D texture0;
void main(){
    FragColor = texture(texture0,TexCod);
    
}
