#version 330 core
out vec4 FragColor;
in vec4 ourColor;
in vec2 TexCoord;
uniform sampler2D texture0;
uniform bool isTexture;
uniform bool isLighting;



uniform int numLights;
uniform vec2 lightsPosition[32];
uniform vec3 lightsColor[32];
uniform vec2 lightsIntensityAndRadius[32];

uniform mat4 projMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform vec2 SCREEN_SIZE;

void main(){
	
	vec3 light = vec3(0.0);

	vec4 texColor = vec4(1.0,1.0,1.0,1.0);

	vec4 lightColor = vec4(1.0,1.0,1.0,1.0); // 环境光

	// 将窗口坐标转换为裁剪空间坐标
	vec4 clipCoords = vec4((2.0 * gl_FragCoord.xy) / SCREEN_SIZE - 1.0, 0.0, 1.0);

	// 将裁剪空间坐标转换为视点坐标
	vec4 eyeCoords = inverse(projMatrix) * clipCoords;
	eyeCoords = vec4(eyeCoords.xyz / eyeCoords.w, 1.0);

	// 将视点坐标转换为世界坐标
	vec4 worldCoords = inverse(viewMatrix) * eyeCoords;

	light += lightColor.rgb * 0.1; // 环境光

	// 计算每个光源的光照
    for (int i = 0; i < numLights; i++) {
		
		
		
		vec2 lightDir = vec2(lightsPosition[i].x,lightsPosition[i].y) - worldCoords.xy;
        float distance = length(lightDir);
        //float attenuation = 1.0 / (1.0 + 0.1 * distance + 0.01 * distance * distance);
		//float attenuation = 1.0 / (1.0 + 0.1 * distance + 0.01 * distance * distance / (lightsIntensityAndRadius[i].y));

		float attenuation = 1.0 - distance / lightsIntensityAndRadius[i].y;
		if(attenuation < 0.0)attenuation=0.0;
        float diffuseFactor = max(0.0, dot(vec2(0.0, -1.0), lightDir));
		vec3 diffColor = vec3(lightsColor[i].x,lightsColor[i].y,lightsColor[i].z);
        light += (diffColor  * attenuation * lightsIntensityAndRadius[i].x);
    }


	if(isTexture)
	{
		texColor = texture(texture0, TexCoord);   
	}
	
	if(isLighting)
	{
		texColor = texColor * vec4(light,1.0);
	}
    
	FragColor = texColor*ourColor ;
   
}

