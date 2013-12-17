#version 430 core

uniform vec4 u_outline_color;

layout (location = 0) out vec4 color;

void main(){
	color  = u_outline_color;
}
