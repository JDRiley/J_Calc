#include "J_Display_Line.h"

//
#include <GL/glew.h>
//
#include <J_OpenGl_Functions.h>
//
#include <J_Shader_Program.h>
namespace jomike{


J_Display_Line::J_Display_Line(j_uint i_obj_id):J_Display_Object(i_obj_id){
	init_vao();
}

void J_Display_Line::init_vao(){
	glDeleteVertexArrays(1, &M_vao_id);
	glDeleteBuffers(1, &M_buffer_id);

	glGenVertexArrays(1, &M_vao_id);
	glBindVertexArray(M_vao_id);

	glGenBuffers(1, &M_buffer_id);
	glBindBuffer(GL_ARRAY_BUFFER, M_buffer_id);

	std::array<j_float, 4> cursor_data;


	glBufferData(GL_ARRAY_BUFFER, sizeof(cursor_data), cursor_data.data(), GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, nullptr);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void J_Display_Line::recalculate_vao(){
	std::array<j_float, 4> cursor_data = {
		x1(), y1()
		, x2(), y2()
	};
	glBindBuffer(GL_ARRAY_BUFFER, M_buffer_id);
	glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(0)
					, sizeof(cursor_data), cursor_data.data());


	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void J_Display_Line::set_line(const J_Line& i_line){
	J_Line::set_line(i_line);
	recalculate_vao();
}

void J_Display_Line::draw()const{
	if(!fill_visibility_status()){
		return;
	}
	glUseProgram(s_line_shader->program_id());
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(M_vao_id);
	glLineWidth(1.0f);

	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
	glUseProgram(0);
}

void J_Display_Line::alert_resize(int, int){}

void J_Display_Line::set_fill_color(const J_UI_Color& i_color){
	int uniform_loc = glGetUniformLocation(s_line_shader->program_id(), "u_outline_color");
	assert(-1 != uniform_loc);
	glProgramUniform4fv(s_line_shader->program_id(), uniform_loc, 1, i_color.M_data.data());
}

void J_Display_Line::set_outline_color(const J_UI_Color& ){

}

}