
// Vertex shader
 
 #version 400

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location=0) in vec3 in_Position;
layout(location=1) in vec3 in_Color;
layout(location=2) in vec3 vertNormal;
layout(location=3) in vec2 inCoords;

out vec3 fragNormal;
//out vec4 gl_Position; 
out vec4 ex_Color;
out vec3 fragVert;
out vec2 texCoords;

void main(void)
{
	fragNormal = vertNormal;
	gl_Position = projection * view * model * vec4(in_Position, 1.0f);
	fragVert = in_Position;
	ex_Color = vec4(in_Color, 1.0f);
	texCoords = inCoords;
} 
 