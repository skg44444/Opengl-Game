#vertex
#version 450 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 toCameraVector;

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;
uniform vec3 lightPos;

void main()
{
	vec4 worldPosition = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
	gl_Position = proj * view * worldPosition;
	TexCoord = aTexCoord;

	surfaceNormal = (model * vec4(aNormal, 0.0)).xyz;
	toLightVector = lightPos - worldPosition.xyz;
	toCameraVector = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
}

#fragment
#version 450 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec3 toCameraVector;

uniform sampler2D ourTexture;
uniform vec3 lightColor;

void main()
{
	vec3 unitNormal = normalize(surfaceNormal);
	vec3 unitLightVector = normalize(toLightVector);

	float nDot = dot(unitNormal, unitLightVector);
	float brightness = max(nDot, 0.2);
	vec3 diffuse = brightness * lightColor;

	vec3 unitVectorToCamera = normalize(toCameraVector);
	vec3 lightDirection = -unitLightVector;
	vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);

	float specular = dot(reflectedLightDirection, unitVectorToCamera);
	specular = max(specular, 0.0);
	float dampedFactor = pow(specular, 10);

	vec3 specularLight = dampedFactor * lightColor;

	FragColor = vec4(diffuse, 1.0) * texture(ourTexture, TexCoord) + vec4(specularLight, 1.0);
}