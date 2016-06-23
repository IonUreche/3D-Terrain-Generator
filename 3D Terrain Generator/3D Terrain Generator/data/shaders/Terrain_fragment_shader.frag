
// Fragment shader  
 
 #version 330

uniform mat4 model;
uniform vec3 lightPosition;
uniform vec3 lightIntensities;
uniform vec3 viewPos;
uniform int texturing_enabled;

in vec3 fragNormal;
in vec3 fragVert;
in vec4 ex_Color;
in vec2 texCoords;

out vec4 out_Color;

uniform float minHeight;
uniform float maxHeight;

uniform sampler2D tGrass;
uniform sampler2D tRock;
uniform sampler2D tSnow;

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
		float x = (fragVert.y - minHeight) / (maxHeight - minHeight);
		float r;// = x;
		float g;// = 2 * x;
		//if(x > 0.5f) g = 2 - g;
		float b;// = (1 - x);

		if (x < 0.1) b = 0.5 + 5 * x; else
		if (x < 0.35) b = 1; else
		if (x < 0.6) b = 1 - 4 * (x - 0.35); 
		else b = 0;

		if (x < 0.1) g = 0; else
		if (x < 0.35) g = (x - 0.1) * 4; else
		if (x < 0.6) g = 1; else
		if (x < 0.85) g = 1 - (x - 0.6) * 4; 
		else g = 0;

		if (x < 0.35) r = 0; else
		if (x < 0.6) r = (x - 0.35) * 4; else
		if (x < 0.85) r = 1;
		else r = 1 - (x - 0.85) * 3.333;

		out_Color = vec4(r, g, b, 1.0f);

		//out_Color = ex_Color;
	}
	else
	{
		float height_treshhold = 0.85f;
		vec3 up = vec3(0.0, 1.0, 0.0);
		float cos_angle = dot(fragNormal, up) / (length(fragNormal) * length(fragNormal));
		vec2 texCoords2 = texCoords;
		if(texCoords2.x >= 0.5) texCoords2.x - 0.5;
		if(texCoords2.y >= 0.5) texCoords2.y - 0.5;
		texCoords2 = texCoords2 * 2;
		vec4 grass = mix(texture2D(tGrass, texCoords), texture2D(tGrass, texCoords2), 0.5);
		vec4 rock = mix(texture2D(tRock, texCoords), texture2D(tRock, texCoords2), 0.5);
		vec4 snow = mix(texture2D(tSnow, texCoords), texture2D(tSnow, texCoords2), 0.5);
		vec4 temp = vec4(1.0);
		if(fragVert.y < height_treshhold * maxHeight) temp = mix(grass, rock, abs(1 - cos_angle));
			else 
			{
				float ww = fragVert.y / maxHeight;
				float cf = (ww - height_treshhold) / (1 - height_treshhold);
				temp = mix(grass, snow, cf);
			}
		//temp = texture2D(tSnow, texCoords);
		out_Color = temp;
	}
}
 