
// Fragment shader  
 
 #version 400

uniform mat4 model;
uniform vec3 lightPosition;
uniform vec3 lightIntensities;
uniform vec3 viewPos;
uniform int texturing_enabled;

in vec3 fragNormal;
in vec3 fragVert;
in vec4 ex_Color;

out vec4 out_Color;

in vec2 texCoord;

uniform sampler2D gSampler;

void main(void)
{
	mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 normal = normalize(normalMatrix * fragNormal);
	vec3 fragPosition = vec3(model * vec4(fragVert, 1));
	/*
	// Ambient
	float ambientStrength = 0.2f;
	vec3 ambient = ambientStrength * lightIntensities;

	// Diffuse
	vec3 surfaceToLight = lightPosition - fragPosition;
	float diff = max(dot(normal, surfaceToLight), 0.0f);
	vec3 diffuse = diff * lightIntensities;

	// Specular
	float specularStrength = 0.1f;
	vec3 viewDir = normalize(viewPos - fragPosition);
	vec3 reflectDir = reflect(-surfaceToLight, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightIntensities;

	vec3 result = (ambient + diffuse + specular) * ex_Color;
	out_Color = vec4(result, 1.0f);
	*/
	
	if(texturing_enabled == 0)
	{
		out_Color = ex_Color;//vec4(1.0f); //ex_Color;
	}
	//else
	//{
	//	out_Color = texture2D(gSampler, texCoord);
	//}
}
 