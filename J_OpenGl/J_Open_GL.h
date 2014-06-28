#ifndef J_OPEN_GL_H
#define J_OPEN_GL_H

#include "J_OpenGL_Fwd_Decl.h"
//
#include "OpenGL_Enums.h"

namespace jomike{




class J_Open_GL{
public:
	J_Open_GL();
	//
	void bind_texture(Texture_Target i_target, const J_GL_Texture& irk_texture_id);
	//
	void bind_texture_2D(const J_GL_Texture& irk_texture_id);
	//
	void debind_texture(Texture_Target i_target);
	//
	void tex_storage_2D(
		Texture_Target i_texture_type, int i_num_levels
		, GL_Sized_Internal_Formats i_internal_format, int i_width, int i_height);
	//
	void tex_sub_image_2D_ubyte(
		Texture_Target i_texture_type, int i_level, int i_x_offset
		, int i_y_offset, int i_width, int i_height, GL_Pixel_Formats i_format
		, const j_ubyte* data);
	//
	void bind_vertex_array(const J_GL_Vertex_Array& i_vao_id);

	void check_frame_buffer_status()const;

	//
	void debind_vertex_array();
	void debind_buffer(GL_Buffer_Targets i_buffer_target);
	//
	void use_program(j_uint i_program_id);
	//
	void debind_program();
	//
	int create_program();
	//
	void attach_shader(int i_program_id, int i_shader_id);
	//
	void link_program(int i_program_id);
	//
	bool get_program_info(int i_prgram_id, GL_Statuses i_status);
	//
	void bind_read_framebuffer(const J_GL_Framebuffer& irk_framebuffer_id);
	//
	void bind_draw_framebuffer(const J_GL_Framebuffer& irk_framebuffer_id);
	//
	void bind_framebuffer(const J_GL_Framebuffer& irk_framebuffer_id);
	//
	void draw_arrays(Array_Draw_Mode i_draw_mode, int i_first, int i_count);
	//
	void bind_buffer(GL_Buffer_Targets i_target, const J_GL_Buffer&  i_buffer_id);
	//
	void viewport(int i_x, int i_y, int i_width, int i_height);
	//
	void set_clear_color(j_float i_red, j_float i_green, j_float i_blue, j_float i_alpha);
	//
	void attach_read_framebuffer_texture_2D(
		GL_Attachment_Points i_attatchment_point, Texture_Target i_tex_target
		, j_uint i_texture_id, int i_level);
	//
	void attach_draw_framebuffer_texture_2D(
		GL_Attachment_Points i_attatchment_point, Texture_Target i_tex_target
		,  const J_GL_Texture_Render_Buffer& irk_frame_renderbuffer, int i_level);
	//
	void buffer_data(
		GL_Buffer_Targets i_buffer, std::size_t i_size, const j_float* i_data
		, GL_Draw_Types i_draw_type);
	//
	void buffer_sub_data(
		GL_Buffer_Targets i_target, int i_offset, int i_size, const j_float* i_data);

	void vertix_attribute_pointer(
		int i_index, int i_size, GL_Types i_data_type, bool i_normalized, int i_stride, int i_offset);

	void enable_vertex_attribute_array(int i_index);
	//
	void line_width(j_float i_thickness);
	//
	void texture_wrap_s(Texture_Target i_target, Texture_Wrap_Type i_texture_wrap_type);
	void texture_wrap_t(Texture_Target i_target, Texture_Wrap_Type i_texture_wrap_type);
	void texture_wrap_r(Texture_Target i_target, Texture_Wrap_Type i_texture_wrap_type);
private:

};

}

#endif //J_OPEN_GL_H


