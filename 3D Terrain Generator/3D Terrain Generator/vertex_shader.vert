
// Vertex shader
 
 #version 400

in vec3 in_Position;
//in vec3 in_Color;

out vec4 gl_Position; 
out vec4 ex_Color;
uniform mat4 myMatrix;
uniform mat4 view;
uniform mat4 projection;

void main(void)
{
	gl_Position = projection * view * myMatrix * vec4(in_Position, 1.0f);
	//ex_Color = vec4(in_Color, 1.0f);
} 
 