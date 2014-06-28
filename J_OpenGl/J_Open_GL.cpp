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

static int get_gl_enum(GL_Buffer_Targets i_target){
	return buffer_targets_array[static_cast<int>(i_target)];
}

static int get_gl_enum(Texture_Target i_target){
	return texture_map_array[static_cast<int>(i_target)];
}
static int get_gl_enum(Texture_Wrap_Type i_wrap_type){
	return texture_wrap_type_array[static_cast<int>(i_wrap_type)];
}

static int get_gl_enum(GL_Pixel_Formats i_format){
	return gl_pixel_formats_array[static_cast<int>(i_format)];
}


void J_Open_GL::bind_texture(Texture_Target i_target, const J_GL_Texture& irk_texture_id){
	glBindTexture(texture_map_array[static_cast<int>(i_target)], irk_texture_id.get_ID());
	assert(!open_gl_error());
}

void J_Open_GL::bind_texture_2D(const J_GL_Texture& irk_texture_id){
	glBindTexture(GL_TEXTURE_2D, irk_texture_id.get_ID());
	assert(!open_gl_error());
}

void J_Open_GL::bind_vertex_array(const J_GL_Vertex_Array& irk_vao){
	glBindVertexArray(irk_vao.get_ID());
	assert(glIsVertexArray(irk_vao.get_ID()));



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

void J_Open_GL::bind_read_framebuffer(const J_GL_Framebuffer& irk_framebuffer){
	glBindFramebuffer(GL_READ_FRAMEBUFFER, irk_framebuffer.get_ID());
	assert(!open_gl_error());
}

void J_Open_GL::bind_framebuffer(const J_GL_Framebuffer& irk_framebuffer){
	
	glBindFramebuffer(GL_FRAMEBUFFER, irk_framebuffer.get_ID());
	assert(!open_gl_error());
}

void J_Open_GL::draw_arrays(Array_Draw_Mode i_draw_mode, int i_first, int i_count){

	glDrawArrays(array_draw_mode_array[static_cast<int>(i_draw_mode)], i_first, i_count);

	assert(!open_gl_error());
}

void J_Open_GL::bind_buffer(GL_Buffer_Targets i_target, const J_GL_Buffer& irk_buffer_id){
	glBindBuffer(buffer_targets_array[static_cast<int>(i_target)], irk_buffer_id.get_ID());
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

	assert(glIsTexture(irk_frame_renderbuffer.get_ID()));

	glFramebufferTexture2D(
		GL_DRAW_FRAMEBUFFER, gl_attachment_points_arrary[static_cast<int>(i_attatchment_point)]
		, texture_map_array[static_cast<int>(i_tex_target)], irk_frame_renderbuffer.get_ID(), i_level);

		assert(!open_gl_error());
}

void J_Open_GL::buffer_data(
	GL_Buffer_Targets i_buffer_target, std::size_t i_size, const j_float* i_data, GL_Draw_Types i_draw_type){
	
	glBufferData(
		get_gl_enum(i_buffer_target)
		, sizeof(j_float)*i_size, i_data, draw_types_array[static_cast<int>(i_draw_type)]);
	assert(!open_gl_error());
}

void J_Open_GL::vertix_attribute_pointer(
	int i_index, int i_size, GL_Types i_data_type, bool i_normalized, int i_stride, int i_offset){

	glVertexAttribPointer(i_index, i_size, gl_types_array[ static_cast<int>(i_data_type)]
						  , i_normalized, i_stride, reinterpret_cast<void*>(i_offset));

	assert(!open_gl_error());
}

void J_Open_GL::enable_vertex_attribute_array(int i_index){
	glEnableVertexAttribArray(i_index);
	assert(!open_gl_error());
}

void J_Open_GL::debind_program(){
	glUseProgram(0);
	assert(!open_gl_error());
}

void J_Open_GL::line_width(j_float i_thickness){
	glLineWidth(i_thickness);
	assert(!open_gl_error());
}

void J_Open_GL::debind_buffer(GL_Buffer_Targets i_buffer_target){
	glBindBuffer(get_gl_enum(i_buffer_target), 0);
	assert(!open_gl_error());
}

void J_Open_GL::debind_vertex_array(){
	glBindVertexArray(0);
	assert(!open_gl_error());
}

void J_Open_GL::buffer_sub_data(GL_Buffer_Targets i_target, int i_offset, int i_size, const j_float* i_data){
	glBufferSubData(get_gl_enum(i_target), static_cast<GLintptr>(i_offset), sizeof(j_float)*i_size, i_data);
	assert(!open_gl_error());
}

void J_Open_GL::debind_texture(Texture_Target i_target){
	glBindTexture(get_gl_enum(i_target), 0);
	assert(!open_gl_error());
}

void J_Open_GL::texture_wrap_s(Texture_Target i_target, Texture_Wrap_Type i_texture_wrap_type){
	glTexParameteri(get_gl_enum(i_target), GL_TEXTURE_WRAP_S, get_gl_enum(i_texture_wrap_type));
	assert(!open_gl_error());
}

void J_Open_GL::texture_wrap_r(Texture_Target i_target, Texture_Wrap_Type i_texture_wrap_type){
	glTexParameteri(get_gl_enum(i_target), GL_TEXTURE_WRAP_R, get_gl_enum(i_texture_wrap_type));
	assert(!open_gl_error());
}

void J_Open_GL::texture_wrap_t(Texture_Target i_target, Texture_Wrap_Type i_texture_wrap_type){
	glTexParameteri(get_gl_enum(i_target), GL_TEXTURE_WRAP_T, get_gl_enum(i_texture_wrap_type));
	assert(!open_gl_error());
}

J_Open_GL::J_Open_GL(){

}

void J_Open_GL::tex_sub_image_2D_ubyte(
	Texture_Target i_texture_type, int i_level, int i_x_offset, int i_y_offset, int i_width
	, int i_height, GL_Pixel_Formats i_format, const j_ubyte* i_data){

	glTexSubImage2D(
		get_gl_enum(i_texture_type), i_level, i_x_offset, i_y_offset, i_width, i_height, get_gl_enum(i_format)
		, GL_UNSIGNED_BYTE, i_data);

	assert(!open_gl_error());
}

void J_Open_GL::check_frame_buffer_status()const{
#ifndef NDEBUG
	int draw_frame_buffer_status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	int read_framebuffer_status = glCheckFramebufferStatus(GL_READ_FRAMEBUFFER);
	assert(draw_frame_buffer_status == GL_FRAMEBUFFER_COMPLETE);
	assert(read_framebuffer_status == GL_FRAMEBUFFER_COMPLETE);
#endif
}


}



