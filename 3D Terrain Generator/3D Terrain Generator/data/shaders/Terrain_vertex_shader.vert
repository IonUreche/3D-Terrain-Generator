
// Vertex shader
 
 #version 330

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float minHeight;
uniform float maxHeight;

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Color;
layout(location=2) in vec3 vertNormal;
layout(location=3) in vec2 inCoords;

out vec3 fragNormal;
out vec4 ex_Color;
out vec3 fragVert;
out vec2 texCoords;

void main(void)
{
	fragNormal = vertNormal;
	gl_Position = projection * view * model * vec4(in_Position, 1.0f);
	fragVert = in_Position;
	/*
	float x = (fragVert.y - minHeight) / (maxHeight - minHeight);
	float r,g,b;
	//float green = 2 * alpha;
	//if(alpha > 0.5f) green = 2 - green;
	
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
	*/
	ex_Color = vec4(in_Color, 1.0f);//vec4(r, g, b, 1.0f);
	texCoords = inCoords; 
} 
 