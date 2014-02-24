#include "J_Display_Letter_Box.h"
//
#include <J_OpenGL.h>
//
#include <gl/glew.h>
//
#include <J_Shader_Program.h>
//
#include "../Bitmap_Metrics.h"
namespace jomike{
static Instance_Pointer<Contexts_Handler> s_contexts;



class Static_Image_Shader : public J_GL_Shader_Program{
public:
	static Static_Image_Shader& get_instance();
	j_uint program_id()const;
private:
	Static_Image_Shader();
	j_uint M_program_id, M_vert_shader_id, M_image_frag_id;

};

j_uint Static_Image_Shader::program_id()const{
	return M_program_id;
}

Static_Image_Shader::Static_Image_Shader():M_program_id(0){
	M_vert_shader_id = load_vertex_shader(SHADER_BASE_PATH + "quad.vert");
	M_image_frag_id = load_fragment_shader(SHADER_BASE_PATH + "quad_image.frag");

	M_program_id = glCreateProgram();
	glAttachShader(M_program_id, M_vert_shader_id);
	glAttachShader(M_program_id, M_image_frag_id);
	glLinkProgram(M_program_id);
	enforce_program_status(M_program_id, GL_LINK_STATUS);
}

Static_Image_Shader& Static_Image_Shader::get_instance(){
	static Static_Image_Shader image_shader;
	return image_shader;
}

static Instance_Pointer<Static_Image_Shader> s_image_shader;

J_Display_Letter_Box
	::J_Display_Letter_Box(j_uint i_obj_id):J_Display_Box(i_obj_id){}

void J_Display_Letter_Box::set_image_box(const Pen_Pos_FL_t &i_pos, const Bitmap_Metrics &i_metrics){
	auto window = s_contexts->get_active_window();
	J_Rectangle rectangle;

	rectangle.set_x(i_pos.first + to_x_screen(window, i_metrics.left));
	rectangle.set_y(i_pos.second - to_y_screen(window, i_metrics.underreach));
	rectangle.set_width(to_x_screen(window, i_metrics.width));
	rectangle.set_height(to_y_screen(window, i_metrics.height));
	set_rectangle(rectangle);
}

void J_Display_Letter_Box::set_buffer_data(int i_width, int i_height, const J_UI_Color& i_color, const j_ubyte* i_bitmap){
	ex_array<j_ubyte> colored_bitmap(convert_to_rgba32(i_width, i_height, i_color, i_bitmap));
	
	glUseProgram(s_image_shader->program_id());

	glDeleteTextures(1, &M_texture_id);
	glGenTextures(1, &M_texture_id);
	glBindTexture(GL_TEXTURE_2D, M_texture_id);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, i_width, i_height);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, i_width, i_height, GL_RGBA
					, GL_UNSIGNED_BYTE, colored_bitmap.data());
	glUseProgram(0);



}

void J_Display_Letter_Box::set_buffer_data(const Bitmap_Metrics& i_metrics, const J_UI_Color& i_color, const j_ubyte* i_data){
	set_buffer_data(i_metrics.width, i_metrics.height, i_color, i_data);
}

void J_Display_Letter_Box::draw()const{
	assert(glIsProgram(s_image_shader->program_id()));
	glBindVertexArray(get_box_vao());
	glUseProgram(s_image_shader->program_id());

	glBindTexture(GL_TEXTURE_2D, M_texture_id);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

}



