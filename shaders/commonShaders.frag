#version 330 core
out vec4 FragColor;
in vec4 ourColor;
in vec2 TexCoord;
uniform sampler2D texture0;
uniform bool isTexture;
uniform bool isLighting;



void main(){
	vec4 texColor = vec4(1.0,1.0,1.0,1.0);
	vec4 lightColor = vec4(0.8,0.5,0.5,1.0);
	if(isTexture)
	{
		texColor = texture(texture0, TexCoord);   
	}
	
	if(isLighting)
	{
		texColor = texColor * lightColor;
	}
    
	FragColor = texColor*ourColor;
   
}

