#version 330 core
out vec4 FragColor;
in vec4 ourColor;
in vec2 TexCord;
uniform sampler2D textureWall;


void main(){
//   FragColor = vec4(texture(textureWall,TexCord).rgb,1);
//    FragColor = vec4(TexCord,0,1);

    vec4 texColor = texture(textureWall, TexCord);
    if(texColor.a<=0.1)
    {
        FragColor = texColor;
        discard;
    }
    vec4 finalColor = mix(texColor,ourColor,ourColor.a);
    FragColor = finalColor;
    //FragColor = vec4(ourColor,1.0);

}

/*
void main(){
    vec4 texColor = texture(textureWall, TexCord);
    FragColor = vec4(texColor.rgb, texColor.a);
}
*/