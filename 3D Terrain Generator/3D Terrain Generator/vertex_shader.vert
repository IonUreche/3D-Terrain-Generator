
// Vertex shader
 
 #version 400

in vec3 in_Position;
in vec3 in_Color;

out vec4 gl_Position; 
out vec4 ex_Color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
	gl_Position = projection * view * model * vec4(in_Position, 1.0f);
	ex_Color = vec4(in_Color, 1.0f);
	//ex_Color = vec4(in_Position.x / 10.0f, in_Position.y / 10.0f, in_Position.z / 10.0f, 1.0f);
} 
 