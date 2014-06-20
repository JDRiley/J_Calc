#include "J_Open_GL.h"
//
#include "J_OpenGL.h"
//
#include <gl\glew.h>
//
#include <j_map.h>
//
#include "GL_Enum_Conversion_Maps.h"
//
#include "J_GL_Objects.h"
namespace jomike{


static J_Open_GL s_open_gl;



void J_Open_GL::bind_texture(Texture_Target i_target, const J_GL_Texture& irk_texture_id){
	glBindTexture(texture_map_array[static_cast<int>(i_target)], irk_texture_id.get_ID());
	assert(!open_gl_error());
}

void J_Open_GL::bind_texture_2D(const J_GL_Texture& irk_texture_id){
	glBindTexture(texture_map_array[static_cast<int>(Texture_Target::TEXTURE_2D)], irk_texture_id.get_ID());
	assert(!open_gl_error());
}

void J_Open_GL::bind_vertex_array(const J_GL_Vertex_Array& irk_vao){
	glBindVertexArray(irk_vao.get_ID());
	assert(!open_gl_error());
}

void J_Open_GL::use_program(j_uint i_program_id){
	assert(glIsProgram(i_program_id));
	glUseProgram(i_program_id);
	assert(!open_gl_error());
}

int J_Open_GL::create_program(){
	int program_id = glCreateProgram();
	assert(!open_gl_error());
	return program_id;
}

void J_Open_GL::attach_shader(int i_program_id, int i_shader_id){
	glAttachShader(i_program_id, i_shader_id);
	assert(!open_gl_error());
}

void J_Open_GL::link_program(int i_program_id){
	glLinkProgram(i_program_id);
	assert(!open_gl_error());
}

bool J_Open_GL::get_program_info(int i_program_id, GL_Statuses i_status){
	int program_status;

	glGetProgramiv(i_program_id, gl_status_array[static_cast<int>(i_status)], &program_status);
	assert(!open_gl_error());
	return program_status;
}

void J_Open_GL::bind_draw_framebuffer(const J_GL_Framebuffer& irk_framebuffer){
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, irk_framebuffer.get_ID());
	assert(!open_gl_error());
}

void J_Open_GL::bind_read_framebuffer(int i_framebuffer_id){
	glBindFramebuffer(GL_READ_FRAMEBUFFER, i_framebuffer_id);
	assert(!open_gl_error());
}

void J_Open_GL::bind_framebuffer(const J_GL_Framebuffer& irk_frambuffer){
	glBindFramebuffer(GL_FRAMEBUFFER, irk_frambuffer.get_ID());
	assert(!open_gl_error());
}

void J_Open_GL::draw_arrays(Array_Draw_Mode i_draw_mode, int i_first, int i_count){
	glDrawArrays(array_draw_mode_array[static_cast<int>(i_draw_mode)], i_first, i_count);

	assert(!open_gl_error());
}

void J_Open_GL::bind_buffer(GL_Buffer_Targets i_target, j_uint i_buffer_id){
	glBindBuffer(buffer_targets_array[static_cast<int>(i_target)], i_buffer_id);
	assert(!open_gl_error());
}

void J_Open_GL::viewport(int i_x, int i_y, int i_width, int i_height){
	glViewport(i_x, i_y, i_width, i_height);
	assert(!open_gl_error());
}

void J_Open_GL::set_clear_color(j_float i_red, j_float i_green, j_float i_blue, j_float i_alpha){
	glClearColor(i_red, i_green, i_blue, i_alpha);
	assert(!open_gl_error());
}





void J_Open_GL::tex_storage_2D(
	Texture_Target i_texture_type, int i_num_levels
	, GL_Sized_Internal_Formats i_internal_format, int i_width, int i_height){


	glTexStorage2D(
		texture_map_array[static_cast<int>(i_texture_type)], i_num_levels
		, sized_internal_formats[static_cast<int>(i_internal_format)], i_width, i_height);

	assert(!open_gl_error());
}

void J_Open_GL::attach_read_framebuffer_texture_2D(
	GL_Attachment_Points i_attatchment_point, Texture_Target i_tex_target
	, j_uint i_texture_id, int i_level){

	glFramebufferTexture2D(
		GL_READ_FRAMEBUFFER, gl_attachment_points_arrary[static_cast<int>(i_attatchment_point)]
		, texture_map_array[static_cast<int>(i_tex_target)], i_texture_id, i_level);

	assert(!open_gl_error());
}


void J_Open_GL::attach_draw_framebuffer_texture_2D(
	GL_Attachment_Points i_attatchment_point, Texture_Target i_tex_target
	, const J_GL_Texture_Render_Buffer& irk_frame_renderbuffer, int i_level){

	glFramebufferTexture2D(
		GL_DRAW_FRAMEBUFFER, gl_attachment_points_arrary[static_cast<int>(i_attatchment_point)]
		, texture_map_array[static_cast<int>(i_tex_target)], irk_frame_renderbuffer.get_ID(), i_level);

		assert(!open_gl_error());
}




}



