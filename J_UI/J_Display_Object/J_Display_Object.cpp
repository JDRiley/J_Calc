#include "J_Display_Object.h"
#include <J_Error.h>
#include <J_Shader_Program.h>
#include "../J_View.h"
#include "../../J_OpenGl/J_OpenGL.h"
#include <J_Utile.h>
#include <J_UI/J_UI_Model.h>
#include "../J_UI_Object.h"
#include <j_physical_constants.h>
//Algorithms
#include <algorithm>
#include <functional>
//
#include <iostream>
//
#include <sstream>
using std::all_of; using std::bind; using std::equal_to;using std::mem_fn;
using std::cerr;
//Libraries
#include <GL\glew.h>

using namespace std::placeholders;
//Utilities
#include <cassert>

static int g_object_ids = 0;

namespace jomike{

static Instance_Pointer<Contexts_Handler> s_contexts;
static Instance_Pointer<J_UI_Model> s_model;

//J_Display_Object Functions *********************************************************************
J_Display_Object::J_Display_Object(j_uint i_obj_id):M_ID(g_object_ids++)
	, M_UI_Obj_ID(i_obj_id){
	
}

J_Display_Object::J_Display_Object(j_uint i_object_id, j_uint i_id)
:M_ID(i_id), M_UI_Obj_ID(i_object_id){}


/*void set_clickable_status(bool)*/
void J_Display_Object::set_clickable_status(bool i_status){M_clickable_flag = i_status;}

/*bool clickable_status()const*/
bool J_Display_Object::clickable_status()const{return M_clickable_flag;}


void J_Display_Object::mouse_button_press(J_View_Shared_t, int i_key, int i_modifiers
										  , Pen_Pos_FL_t i_cursor_pos){
	s_model->get_ui_object(get_object_ID())
		->mouse_button_press(i_key, i_modifiers, i_cursor_pos);
}
void J_Display_Object::mouse_button_release(J_View_Shared_t, int i_key, int i_modifiers
										  , Pen_Pos_FL_t i_cursor_pos){
	s_model->get_ui_object(get_object_ID())
		->mouse_button_release(i_key, i_modifiers, i_cursor_pos);
}

/*J_Display_Box(j_float, j_float, j_float, j_float, j_window_t)*/
J_Display_Box::J_Display_Box(j_uint i_obj_id):J_Display_Object(i_obj_id){
	initialization();


}

J_Display_Box::J_Display_Box(j_uint i_object_id, j_uint i_id)
	:J_Display_Object(i_object_id, i_id){
	initialization();
}



//Destruction---------------------------------------------------------------------------------------

J_Display_Object::~J_Display_Object(){
}

void J_Display_Object::alert_cursor_pos(Pen_Pos_FL_t){

}

void J_Display_Box::set_box(j_float i_x1, j_float i_x2, j_float i_y1, j_float i_y2){
	J_Rectangle::set_box(i_x1, i_y2, i_x2 - i_x1, i_y1 - i_y2);
	gen_vaos();
}

void J_Display_Box::set_rectangle(const J_Rectangle& i_rectangle){
	J_Rectangle::set_rectangle(i_rectangle);
	gen_vaos();
}

void J_Display_Box::set_width(j_float i_width){
	J_Rectangle::set_width(i_width);
	gen_vaos();
}

void J_Display_Box::set_height(j_float i_height){
	J_Rectangle::set_height(i_height);
	gen_vaos();
}

j_uint J_Display_Box::get_box_vao()const{return M_box_vao_id;}

void J_Display_Box::draw()const{
	glBindVertexArray(M_box_vao_id);
	if(fill_visibility_status()){
		assert(glIsProgram(M_shader->box_program_id()));
		glUseProgram(M_shader->box_program_id());
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	}

	if(outline_visibility_status()){
		assert(glIsProgram(M_shader->outline_program_id()));
		glUseProgram(M_shader->outline_program_id());
		glLineWidth(outline_thickness());
		glDrawArrays(GL_LINE_LOOP, 0 , 4);
		
	}
	glBindVertexArray(0);
	glUseProgram(0);


}

void J_Display_Box::gen_vaos(){
	glDeleteVertexArrays(1, &M_box_vao_id);
	glDeleteBuffers(1, &M_vao_buffer_id);
	//assert(!glIsVertexArray(M_box_vao_id));
	//assert(!glIsBuffer(M_vao_buffer_id));
	glUseProgram(M_shader->box_program_id());
	std::array<j_float,24> vertex_array_data = {
		x1(), y1(), 0.0f, 1.0f
		, x2(), y1(), 0.0f, 1.0f
		, x2(), y2(), 0.0f, 1.0f
		, x1(), y2(), 0.0f, 1.0f

		, 0.0f, 0.0f
		, 1.0f, 0.0f
		, 1.0f, 1.0f
		, 0.0f, 1.0f
	};


	glGenVertexArrays(1, &M_box_vao_id);
	glBindVertexArray(M_box_vao_id);
	glGenBuffers(1, &M_vao_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, M_vao_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_array_data)
		, vertex_array_data.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, nullptr);

	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0, reinterpret_cast<void*>(sizeof(j_float)*16));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

/*void J_Display_Box::set_box_color(J_Color_RGBA<j_float>)*/
void J_Display_Box::set_fill_color(const J_UI_Color& i_color){
	M_shader->set_fill_color(i_color);
}

/*void J_Display_Box::set_outline_color(J_Color_RBGA<j_float>)*/
void J_Display_Box::set_outline_color(const J_UI_Color& i_color){
	M_shader->set_outline_color(i_color);
}


void J_Display_Box::set_x(j_float i_x){
	J_Rectangle::set_x(i_x);
	gen_vaos();
}

void J_Display_Box::set_y(j_float i_y){
	J_Rectangle::set_y(i_y);
	gen_vaos();
}


J_Display_Box::~J_Display_Box(){
	delete M_shader; 
	glDeleteBuffers(1, &M_vao_buffer_id);
	glDeleteVertexArrays(1, &M_box_vao_id);
}

void J_Display_Box::initialization(){
	M_shader = new J_GL_Box_Shader;
	M_box_vao_id = M_vao_buffer_id = 0;
	gen_vaos();
}



//J_Display_Circle---------------------------------------------------------------------------------------



J_Display_Circle::J_Display_Circle(j_uint i_obj_id) :J_Display_Object(i_obj_id){
#ifndef VS_2013
	default_initialization();
#endif

	M_shader = new J_GL_Circle_Shader;
	gen_vaos();

}

#ifndef VS_2013

void J_Display_Circle::default_initialization(){
	M_circle_vao_id = M_vao_buffer_id = 0u;
	M_num_points = 50;
}

#endif


void J_Display_Circle::set_circle(j_float i_x_pos, j_float i_y_pos, j_float i_radius){
	J_Circle::set_circle(i_x_pos, i_y_pos, i_radius);
	recalc_vertex_data();
}

void J_Display_Circle::set_x(j_float i_x_pos){
	J_Circle::set_x(i_x_pos);
	recalc_vertex_data();
}

void J_Display_Circle::set_y(j_float i_y_pos){
	J_Circle::set_y(i_y_pos);
	recalc_vertex_data();
}

void J_Display_Circle::set_center(j_float i_x_pos, j_float i_y_pos){
	J_Circle::set_center(i_x_pos, i_y_pos);
	recalc_vertex_data();
}

void J_Display_Circle::set_radius(j_float i_radius){


	J_Circle::set_radius(i_radius);
	recalc_vertex_data();
}

J_Display_Circle::~J_Display_Circle(){
	delete M_shader;
	glDeleteBuffers(1, &M_vao_buffer_id);
	glDeleteVertexArrays(1, &M_circle_vao_id);
}


void J_Display_Circle::gen_vaos(){
	glGenVertexArrays(1, &M_circle_vao_id);
	glGenBuffers(1, &M_vao_buffer_id);
	glBindVertexArray(M_circle_vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, M_vao_buffer_id);

	ptrdiff_t data_size = sizeof(j_float)*M_num_points * 4;

	glBufferData(GL_ARRAY_BUFFER, data_size, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void J_Display_Circle::recalc_vertex_data(){
	ex_array<j_float> vertex_array_data;
	auto context = s_contexts->get_active_context();

	j_float ratio = s_contexts->ratio();

	for (int i = 0; i < M_num_points; i++){
		j_float angle = 2.0f*i*static_cast<j_float>(PI) / M_num_points;
		vertex_array_data.push_back(x1() + radius()*cos(angle));
		vertex_array_data.push_back(y1() + radius()*sin(angle)*ratio);
		vertex_array_data.push_back(0.0f);
		vertex_array_data.push_back(1.0f);
	}
	ptrdiff_t data_size = sizeof(j_float)*vertex_array_data.size();
	glBindVertexArray(M_circle_vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, M_vao_buffer_id);
	glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(0), data_size
		, vertex_array_data.data());
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void J_Display_Circle::draw()const{
	glBindVertexArray(M_circle_vao_id);
	if (fill_visibility_status()){
		assert(glIsProgram(M_shader->fill_program_id()));
		glUseProgram(M_shader->fill_program_id());
		glDrawArrays(GL_TRIANGLE_FAN, 0, M_num_points);
	}

	if (outline_visibility_status()){
		assert(glIsProgram(M_shader->outline_program_id()));
		glUseProgram(M_shader->outline_program_id());
		glLineWidth(outline_thickness());
		glDrawArrays(GL_LINE_LOOP, 0, M_num_points);

	}
	glBindVertexArray(0);
	glUseProgram(0);


}

void J_Display_Circle::set_outline_color(const J_UI_Color& i_color){
	M_shader->set_outline_color(i_color);
}

void J_Display_Circle::set_fill_color(const J_UI_Color& i_color){
	M_shader->set_fill_color(i_color);
}

}