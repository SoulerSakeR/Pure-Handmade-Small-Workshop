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


/*
	环境光：影响系数，  颜色

	漫反射：影响系数，  颜色，  通过距离计算效果， 光源强度，光源位置
	struct LightSourceData {
		vec4 position; // 光源位置
		vec3 color;    // 光源颜色
		int type;      // 光源类型
		float intensity; // 光源强度（0-1）

	};

	uniform int numLights;
	uniform LightSourceData lights[MAX_NUM_LIGHTS];

*/





void main(){
	
	vec3 light = lightsColor[0];

	

	vec4 texColor = vec4(1.0,1.0,1.0,1.0);

	vec4 lightColor = vec4(0.8,0.5,0.5,1.0);
	
	//vec4 lightColor = vec4(light,1.0); // 


	if(isTexture)
	{
		texColor = texture(texture0, TexCoord);   
	}
	
	if(isLighting)
	{
		texColor = texColor * lightColor;
	}
    
	 FragColor = texColor*ourColor;
	//FragColor = vec4(light,1.0);
   
}

