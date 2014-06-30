#include "J_UI_Circle.h"
//
#include <J_Shader_Program.h>
//
#include <J_Open_GL.h>
//
#include <J_OpenGL.h>
//
#include <j_physical_constants.h>
namespace jomike{

static J_Open_GL s_open_gl;
static Instance_Pointer<Contexts_Handler> s_contexts;
J_UI_Circle::J_UI_Circle(const J_Circle& irk_circle) :J_Circle(irk_circle){
	
	M_shader = J_GL_Circle_Shader_Unique_t(new J_GL_Circle_Shader);
	gen_vaos();
}


void J_UI_Circle::set_circle(j_float i_x_pos, j_float i_y_pos, j_float i_radius){
	J_Circle::set_circle(i_x_pos, i_y_pos, i_radius);
	recalc_vertex_data();
}

void J_UI_Circle::set_x(j_float i_x_pos){
	J_Circle::set_x(i_x_pos);
	recalc_vertex_data();
}

void J_UI_Circle::set_y(j_float i_y_pos){
	J_Circle::set_y(i_y_pos);
	recalc_vertex_data();
	
}

void J_UI_Circle::set_center(j_float i_x_pos, j_float i_y_pos){
	J_Circle::set_center(i_x_pos, i_y_pos);
	recalc_vertex_data();
}

void J_UI_Circle::set_radius(j_float i_radius){
	J_Circle::set_radius(i_radius);
	recalc_vertex_data();
}

void J_UI_Circle::gen_vaos(){

	s_open_gl.bind_vertex_array(M_circle_vao);
	s_open_gl.bind_buffer(GL_Buffer_Targets::ARRAY_BUFFER, M_vao_buffer);

	ptrdiff_t data_size = sizeof(j_float)*M_num_points * 4;

	s_open_gl.buffer_data(
		GL_Buffer_Targets::ARRAY_BUFFER, data_size, nullptr, GL_Draw_Types::DYNAMIC);
	s_open_gl.vertix_attribute_pointer(
		0, 4, GL_Types::FLOAT, false, 0, 0);

	s_open_gl.enable_vertex_attribute_array(0);
	s_open_gl.debind_vertex_array();
	s_open_gl.debind_buffer(GL_Buffer_Targets::ARRAY_BUFFER);
}

void J_UI_Circle::recalc_vertex_data(){
	ex_array<j_float> vertex_array_data;
	auto context = s_contexts->get_active_context();

	j_float ratio = s_contexts->ratio();

	for(int i = 0; i < M_num_points; i++){
		j_float angle = 2.0f*i*static_cast<j_float>(PI) / M_num_points;
		vertex_array_data.push_back(x1() + radius()*cos(angle));
		vertex_array_data.push_back(y1() + radius()*sin(angle)*ratio);
		vertex_array_data.push_back(0.0f);
		vertex_array_data.push_back(1.0f);
	}
	int data_size = safe_int_cast(vertex_array_data.size());

	s_open_gl.bind_vertex_array(M_circle_vao);
	s_open_gl.bind_buffer(GL_Buffer_Targets::ARRAY_BUFFER, M_vao_buffer);
	s_open_gl.buffer_sub_data(
		GL_Buffer_Targets::ARRAY_BUFFER, 0, data_size, vertex_array_data.data());


	s_open_gl.debind_vertex_array();
	s_open_gl.debind_buffer(GL_Buffer_Targets::ARRAY_BUFFER);
}

void J_UI_Circle::draw()const{
	s_open_gl.bind_vertex_array(M_circle_vao);
	if(fill_visibility_status()){
		s_open_gl.use_program(M_shader->fill_program_id());
		s_open_gl.draw_arrays(Array_Draw_Mode::TRIANGLE_FAN, 0, M_num_points);
	}

	if(outline_visibility_status()){
		s_open_gl.use_program(M_shader->outline_program_id());
		s_open_gl.line_width(outline_thickness());
		s_open_gl.draw_arrays(Array_Draw_Mode::LINE_LOOP, 0, M_num_points);

	}
	s_open_gl.debind_vertex_array();
	s_open_gl.debind_program();


}

void J_UI_Circle::derived_set_fill_color(){
	M_shader->set_fill_color(fill_color());
}

void J_UI_Circle::derived_set_outline_color(){
	M_shader->set_outline_color(outline_color());
}



}



