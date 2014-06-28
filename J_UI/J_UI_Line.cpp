#include "J_UI_Line.h"
//
#include <J_Shader_Program.h>
//
#include <J_Open_GL.h>
namespace jomike{
static Instance_Pointer<J_UI_Model> s_model;
static J_Open_GL s_open_gl;


void J_UI_Line::set_line(const J_Line& i_line){
	J_Line::set_line(i_line);
	recalculate_vao();
}

J_UI_Line::J_UI_Line(j_float i_x1, j_float i_y1, j_float i_x2, j_float i_y2):J_Line(i_x1, i_y1, i_x2, i_y2){
	M_shader = J_GL_Line_Shader_Unique_t(new J_GL_Line_Shader);
	init_vao();
}

void J_UI_Line::derived_set_fill_color(){
	M_shader->set_fill_color(fill_color());
}

void J_UI_Line::derived_set_outline_color(){

}


void J_UI_Line::init_vao(){
	s_open_gl.bind_vertex_array(M_vao);


	s_open_gl.bind_buffer(GL_Buffer_Targets::ARRAY_BUFFER, M_vao_buffer);

	std::array<j_float, 4> cursor_data;

	s_open_gl.buffer_data(GL_Buffer_Targets::ARRAY_BUFFER, 4, cursor_data.data(), GL_Draw_Types::DYNAMIC);
	s_open_gl.vertix_attribute_pointer(0, 2, GL_Types::FLOAT, false, 0, 0);
	s_open_gl.enable_vertex_attribute_array(0);
	s_open_gl.debind_vertex_array();
	s_open_gl.debind_buffer(GL_Buffer_Targets::ARRAY_BUFFER);

}

void J_UI_Line::recalculate_vao(){
	std::array<j_float, 4> cursor_data = {
		x1(), y1()
		, x2(), y2()
	};
	s_open_gl.bind_buffer(GL_Buffer_Targets::ARRAY_BUFFER, M_vao_buffer);
	s_open_gl.buffer_sub_data(GL_Buffer_Targets::ARRAY_BUFFER, 0
					, 4, cursor_data.data());


	s_open_gl.debind_buffer(GL_Buffer_Targets::ARRAY_BUFFER);
}

void J_UI_Line::draw()const{
	if(!fill_visibility_status()){
		return;
	}
	s_open_gl.use_program(M_shader->program_id());
	//s_open_gl.enable(GL_BLEND);
	//s_open_gl.BlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	s_open_gl.bind_vertex_array(M_vao);
	s_open_gl.line_width(1.0f);

	s_open_gl.draw_arrays(Array_Draw_Mode::LINES, 0, 2);
	s_open_gl.debind_vertex_array();
	s_open_gl.debind_program();
}


}



