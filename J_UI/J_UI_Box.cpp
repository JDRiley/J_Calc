#include "J_UI_Box.h"
//
#include <J_Shader_Program.h>
//
#include <J_Open_GL.h>

namespace jomike{
static J_Open_GL s_open_gl;

J_UI_Box::J_UI_Box(const J_Rectangle& irk_pos):J_Rectangle(irk_pos){
	initialization();
}

void J_UI_Box::initialization(){
	M_shader = J_GL_Box_Shader_Unique_t(new J_GL_Box_Shader);
	gen_vaos();
}

void J_UI_Box::gen_vaos(){
	M_box_vao = J_GL_Vertex_Array();
	M_vao_buffer = J_GL_Buffer();


	s_open_gl.use_program(M_shader->box_program_id());
	std::array<j_float, 24> vertex_array_data = {
		x1(), y1(), 0.0f, 1.0f
		, x2(), y1(), 0.0f, 1.0f
		, x2(), y2(), 0.0f, 1.0f
		, x1(), y2(), 0.0f, 1.0f

		, 0.0f, 0.0f
		, 1.0f, 0.0f
		, 1.0f, 1.0f
		, 0.0f, 1.0f
	};


	s_open_gl.bind_vertex_array(M_box_vao);

	s_open_gl.bind_buffer(GL_Buffer_Targets::ARRAY_BUFFER, M_vao_buffer);

	s_open_gl.buffer_data(
		GL_Buffer_Targets::ARRAY_BUFFER, vertex_array_data.size(),
		vertex_array_data.data(), GL_Draw_Types::DYNAMIC);

	
	s_open_gl.vertix_attribute_pointer(0, 4, GL_Types::FLOAT, false, 0, 0);
	s_open_gl.vertix_attribute_pointer(1, 2, GL_Types::FLOAT, false, 0
									   , sizeof(j_float)* 16);
	s_open_gl.enable_vertex_attribute_array(0);
	s_open_gl.enable_vertex_attribute_array(1);


	s_open_gl.bind_vertex_array(J_GL_Vertex_Array::null_object());
	s_open_gl.bind_buffer(GL_Buffer_Targets::ARRAY_BUFFER, J_GL_Buffer::null_object());

}

void J_UI_Box::draw()const{
	s_open_gl.bind_vertex_array(M_box_vao);

	if(fill_visibility_status()){
		s_open_gl.use_program(M_shader->box_program_id());
		s_open_gl.draw_arrays(Array_Draw_Mode::TRIANGLE_FAN, 0, 4);
	}

	if(outline_visibility_status()){

		s_open_gl.use_program(M_shader->outline_program_id());
		s_open_gl.line_width(outline_thickness());
		s_open_gl.draw_arrays(Array_Draw_Mode::LINE_LOOP, 0, 4);

	}
	s_open_gl.bind_vertex_array(J_GL_Vertex_Array::null_object());
	s_open_gl.debind_program();


}


//J_UI_Box::J_UI_Box(const J_Rectangle& irk_pos, j_uint i_obj_id): J_UI_Object(i_obj_id)
//, J_Rectangle(irk_pos){}

void J_UI_Box::set_box(j_float i_x1, j_float i_x2, j_float i_y1, j_float i_y2){
	J_Rectangle::set_box(i_x1, i_y2, i_x2 - i_x1, i_y1 - i_y2);
	gen_vaos();
}

void J_UI_Box::derived_set_fill_color(){
	M_shader->set_fill_color(fill_color());
}

void J_UI_Box::derived_set_outline_color(){
	M_shader->set_outline_color(outline_color());
}

void J_UI_Box::set_width(j_float i_width){
	J_Rectangle::set_width(i_width);
	gen_vaos();
}

void J_UI_Box::set_height(j_float i_height){
	J_Rectangle::set_height(i_height);
	gen_vaos();
}

void J_UI_Box::set_x(j_float i_x){
	J_Rectangle::set_x(i_x);
	gen_vaos();
}

void J_UI_Box::set_y(j_float i_y){
	J_Rectangle::set_y(i_y);
	gen_vaos();
}

void J_UI_Box::set_rectangle(const J_Rectangle& irk_rectangle){
	J_Rectangle::set_rectangle(irk_rectangle);
	gen_vaos();

}


}



