#vertex
#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 surfaceNormal;
out vec3 toLightVector[4];
out vec3 toCameraVector;
out vec4 worldPosition;
flat out int numberOfLights;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;
uniform vec3 lightpos[4];
uniform int noOfLights;

void main()
{
	vec4 worldPosition = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	gl_Position = proj * view * worldPosition;
	TexCoord = aTexCoord;

	surfaceNormal = (model * vec4(aNormal, 0.0)).xyz;
	for(int i=0;i<noOfLights;i++)
		toLightVector[i] = lightpos[i] - worldPosition.xyz;	
	toCameraVector = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
	numberOfLights = noOfLights;
}

#fragment
#version 450 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 surfaceNormal;
in vec3 toLightVector[4];
in vec3 toCameraVector;
in vec4 worldPosition;
flat in int numberOfLights;

uniform sampler2D ourTexture;

struct Light{
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	float ambient;
	vec3 diffuse;
	float specular;
};

uniform Light light[4];

void main()
{
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 result = vec3(0.0);
	for(int i=0; i< numberOfLights; i++)
	{
		vec3 unitLightVector = normalize(toLightVector[i]);

		float nDot = dot(unitNormal, unitLightVector);
		float brightness = max(nDot, light[i].ambient);
		vec3 diffuse = brightness * light[i].diffuse;

		vec3 unitVectorToCamera = normalize(toCameraVector);
		vec3 lightDirection = -unitLightVector;
		vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

		float specular = dot(reflectedLightDirection, unitVectorToCamera);
		specular = max(specular, 0.0);
		float dampedFactor = pow(specular, light[i].specular);

		vec3 specularLight = dampedFactor * light[i].diffuse;

		float distance = length(toLightVector[i]);

		float attenuation = 1.0 / (light[i].constant + light[i].linear * distance + light[i].quadratic * (distance * distance));

		diffuse *= attenuation;
		specularLight *= attenuation;

		result += diffuse*vec3(texture(ourTexture, TexCoord)) + specularLight;
	}

	//FragColor = vec4(diffuse, 1.0) * texture(ourTexture, TexCoord) + vec4(specularLight, 1.0);
	FragColor = vec4(result, 1.0);
}