#version  430  core

uniform sampler2D bitmap_texture_id;
uniform vec4 tex_color;

in vec2 vs_tex_coord;

layout (location = 0) out vec4 color;


void main(){
	color = texture(bitmap_texture_id, vs_tex_coord)[0]*tex_color;
}
