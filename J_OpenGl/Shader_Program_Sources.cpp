#include "Shader_Program_Sources.h"


namespace jomike{

extern const char * const gk_quad_vert_source 
="#version  430  core\n\
layout(location = 0) in vec4 in_position;\
layout(location = 1) in vec2 in_tex_coord;\
\
out vec2 vs_tex_coord;\
\
\
void main(){\
	gl_Position = in_position;\
	vs_tex_coord = in_tex_coord;\
\
}\
";


extern const char * const gk_box_frag_source
=
"#version 430 core\n\
\
uniform vec4 u_box_color;\
\
layout(location = 0) out vec4 color;\
\
void main(){\
	color = u_box_color;\
}\
";


extern const char * const gk_box_outline_frag_source
=
"#version 430 core\n\
\
uniform vec4 u_outline_color;\
\
layout(location = 0) out vec4 color;\
\
void main(){\
	color = u_outline_color;\
}\
";

extern const char* const gk_quad_image_frag_source
=
"#version  430  core\n\
\
uniform sampler2D image_texture_id;\
\
\
in vec2 vs_tex_coord;\
out vec4 color;\
\
void main()\
{\
\
color = texture2D(image_texture_id, vs_tex_coord);\
}\
";

extern const char* const gk_pass_through_vert_source
=
"#version  430  core\n\
\
layout(location = 0) in vec4 in_position;\
\
out vec2 vs_tex_coord;\
\
\
void main(){\
	gl_Position = in_position;\
}\
";


}



