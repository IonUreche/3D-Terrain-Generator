
// Fragment shader  
 
 #version 400

in vec3 ex_Color;
uniform int codCol;
 
out vec4 out_Color;

void main(void)
{
	out_Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);//vec4(ex_Color, 1.0f);
}
 