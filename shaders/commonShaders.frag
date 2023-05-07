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
	�����⣺Ӱ��ϵ����  ��ɫ

	�����䣺Ӱ��ϵ����  ��ɫ��  ͨ���������Ч���� ��Դǿ�ȣ���Դλ��
	struct LightSourceData {
		vec4 position; // ��Դλ��
		vec3 color;    // ��Դ��ɫ
		int type;      // ��Դ����
		float intensity; // ��Դǿ�ȣ�0-1��

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

