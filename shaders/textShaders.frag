#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCord;
uniform sampler2D textureWall;


void main(){

    vec4 texColor = texture(textureWall, TexCord);
    FragColor = vec4(texColor.rgb, texColor.a);

}


