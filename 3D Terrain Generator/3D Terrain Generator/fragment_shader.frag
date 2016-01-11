
// Fragment shader  
 
 #version 400

uniform mat4 model;
uniform vec3 lightPosition;
uniform vec3 lightIntensities;

in vec3 fragNormal;
in vec3 fragVert;
//in vec4 gl_Position; 
in vec4 ex_Color;

out vec4 out_Color;

void main(void)
{

	mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 normal = normalize(normalMatrix * fragNormal);
	vec3 fragPosition = vec3(model * vec4(fragVert, 1));

	vec3 surfaceToLight = lightPosition - fragPosition;

	float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
    brightness = clamp(brightness, 0, 1);

    // calculate final color of the pixel, based on:
    // 1. The angle of incidence: brightness
    // 2. The color/intensities of the light: light.intensities
    // 3. The texture and texture coord: texture(tex, fragTexCoord)
    //  vec4 surfaceColor = texture(tex, fragTexCoord);
    //out_Color = vec4(brightness * lightIntensities * vec3(ex_Color), ex_Color.a);
	
	out_Color = vec4(brightness * vec3(1.0f) * vec3(ex_Color), 1.0f);

	//out_Color = ex_Color;//vec4(1.0f); //ex_Color;
}
 