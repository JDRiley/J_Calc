#version  430  core

uniform sampler2D image_texture_id;


in vec2 vs_tex_coord;
out vec4 color;

void main()
{
int i;
float width = 1.0/1280.0;
float height = 1.0/720.0;
vec2 offset[9] = {
	{width, height}, {0.0, height}, {-width, height}, {width, 0.0}
	, {0.0, 0.0}, {-width, 0.0}, {width, -height}, {0.0, -height}
	, {-width, -height}
};
int kernel[9] = {0, -1, 0,

-1, 5, -1,

0, -1, 0
};

if(vs_tex_coord[0] > 0.0){
	for (i = 0; i < 9; i++) {
		color +=  kernel[i]*texture2D(image_texture_id, vs_tex_coord +offset[i]);

	}
}else{
	color = texture2D(image_texture_id, vs_tex_coord);
}

}

