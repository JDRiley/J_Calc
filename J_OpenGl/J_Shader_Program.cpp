#include "J_Shader_Program.h"
//
#include <J_String.h>

#include "J_Open_GL.h"
//
#include "J_OpenGL_Functions.h"
//
#include "Shader_Program_Sources.h"
//
//
#include "Shader_Utility_Functions.h"
//Utilities
#include <cassert>

#include <iostream>
using std::cerr; using std::endl; using std::cout;
using std::string;



namespace jomike{
//J_GL_Shader_Program Functions *************************************************************





static J_Open_GL s_open_gl;

typedef unsigned int GLenum;

void enforce_shader_status(j_uint i_shader_id, GLenum i_status);


J_GL_Shader_Program::~J_GL_Shader_Program(){
	for(auto& f_program : M_program_ids){
		s_open_gl.delete_program(f_program);
		
	}
	for(auto f_shader : M_shader_ids){
		s_open_gl.delete_shader(f_shader);
	}
}


void J_GL_Shader_Program::attach_shader_to_program(j_uint i_program_id, j_uint i_shader_id){
	s_open_gl.attach_shader(i_program_id, i_shader_id);
}

void J_GL_Shader_Program::link_program(j_uint i_program_id){
	s_open_gl.link_program(i_program_id);
}


void enforce_program_status(j_uint i_program_id, GL_Statuses i_status);

void J_GL_Shader_Program::enforce_program_status(j_uint i_program_id, GL_Statuses i_status)const{
	::jomike::enforce_program_status(i_program_id, i_status);
}

int J_GL_Shader_Program::get_uniform_location(j_uint i_program_id, const char* const ik_uniform_name){
	return s_open_gl.get_uniform_location(i_program_id, ik_uniform_name);
}

void J_GL_Shader_Program
	::program_uniform_4v(j_uint i_program_id, int i_uniform_loc, int i_count, const j_float* i_data){
		s_open_gl.program_uniform_4v(i_program_id, i_uniform_loc, i_count, i_data);
		
}

j_uint J_GL_Shader_Program::create_program(){
	j_uint program_id = s_open_gl.create_program();
	M_program_ids.push_back(program_id);
	return program_id;
}

j_uint J_GL_Shader_Program::load_fragment_shader(const char* const ik_shader_src){
	j_uint fragment_shader = ::jomike::load_fragment_shader(ik_shader_src);
	M_shader_ids.push_back(fragment_shader);
	return fragment_shader;
}

j_uint J_GL_Shader_Program::load_vertex_shader(const char* const ik_shader_src){
	j_uint vertex_shader = ::jomike::load_vertex_shader(ik_shader_src);
	M_shader_ids.push_back(vertex_shader);
	return vertex_shader;
}


J_GL_Box_Shader::J_GL_Box_Shader(j_uint i_box_program_id, j_uint i_outline_program_id){
	// Optimization to prevent recompilation of identical programs. This shader does not
	// manage the lifetimes of the program.
	M_box_prog_id = i_box_program_id;
	M_outline_prog_id = i_outline_program_id;
}

}