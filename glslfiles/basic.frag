// Fragment Shader – file "minimal.frag"

#version 140

in  vec3 ex_Color;  //colour arriving from the vertex
out vec4 out_Color; //colour for the pixel

void main(void)
{
	out_Color = vec4(0.7,0.2,0.5,1.0);
	//out_Color = vec4(ex_Color,1.0);
}