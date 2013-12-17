#version  430  core

layout (location = 0) in vec4 in_position;

out vec2 vs_tex_coord;


void main(){
	gl_Position = in_position;
}
