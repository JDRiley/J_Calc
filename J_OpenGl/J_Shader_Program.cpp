#include "J_Shader_Program.h"
//
#include <J_String.h>
//Libraries
#include <GL\glew.h>
//
#include "J_Open_GL.h"
//
#include "J_OpenGL_Functions.h"
//Utilities
#include <cassert>

#include <iostream>
using std::cerr; using std::endl; using std::cout;
using std::string;
namespace jomike{
//J_GL_Shader_Program Functions *************************************************************


static J_Open_GL s_open_gl;
J_GL_Box_Shader::J_GL_Box_Shader(){
	j_uint vert_shader_id = load_vertex_shader(SHADER_BASE_PATH + "quad.vert");
	j_uint box_frag_id  = load_fragment_shader(SHADER_BASE_PATH + "box.frag");
	j_uint outline_frag_id  = load_fragment_shader(SHADER_BASE_PATH + "box_outline.frag");
	
	add_shader_id(vert_shader_id);
	add_shader_id(box_frag_id);
	add_shader_id(outline_frag_id);

	M_box_prog_id = glCreateProgram();
	add_program_id(M_box_prog_id);
	glAttachShader(M_box_prog_id, vert_shader_id);
	glAttachShader(M_box_prog_id, box_frag_id);
	glLinkProgram(M_box_prog_id);
	enforce_program_status(M_box_prog_id, GL_Statuses::LINK_STATUS);



	M_outline_prog_id = glCreateProgram();
	add_program_id(M_outline_prog_id);
	glAttachShader(M_outline_prog_id, vert_shader_id);
	glAttachShader(M_outline_prog_id, outline_frag_id);
	glLinkProgram(M_outline_prog_id);
	enforce_program_status(M_outline_prog_id, GL_Statuses::LINK_STATUS);
}

void J_GL_Box_Shader::set_fill_color(J_Color_RGBA<j_float> i_color){
	int uniform_loc = glGetUniformLocation(M_box_prog_id, "u_box_color");
	assert(-1 != uniform_loc);
	glProgramUniform4fv(M_box_prog_id, uniform_loc, 1, i_color.M_data.data());
}

void J_GL_Box_Shader::set_outline_color(J_Color_RGBA<j_float> i_color){
	int uniform_loc = glGetUniformLocation(M_outline_prog_id, "u_outline_color");
	assert(-1 != uniform_loc);
	glProgramUniform4fv(M_outline_prog_id, uniform_loc, 1, i_color.M_data.data());
}


static string get_image_shader_name(Image_Format i_format){
	switch(j_num_channels(i_format)){
	case 3:
		return "quad_image.frag";
	case 4:
		return "quad_image.frag";//"quad_image_four_channel.frag";
	default:
		assert(!"No fragment program for this number of channels");
		return "";
	}
}

Image_Shader_Program::Image_Shader_Program(Image_Format ):M_image_program_id(0){
	M_vert_shader_id = load_vertex_shader(SHADER_BASE_PATH + "quad.vert");
	M_image_frag_id = load_fragment_shader(SHADER_BASE_PATH + "quad_image.frag");

	M_image_program_id = glCreateProgram();
	glAttachShader(M_image_program_id, M_vert_shader_id);
	glAttachShader(M_image_program_id, M_image_frag_id);
	glLinkProgram(M_image_program_id);
	enforce_program_status(M_image_program_id, GL_Statuses::LINK_STATUS);
}

j_uint Image_Shader_Program::program_id()const{
	return M_image_program_id;
}

void Image_Shader_Program::set_format(Image_Format i_format){
	assert(!"Haven't figured out why i'd need to change the fragment shader");
	assert(i_format != M_format);
	M_format = i_format;
	glDetachShader(M_image_program_id, M_image_frag_id);
	glDeleteShader(M_image_frag_id);

	M_image_frag_id = load_fragment_shader(SHADER_BASE_PATH + get_image_shader_name(M_format));
	glAttachShader(M_image_program_id, M_image_frag_id);
	glLinkProgram(M_image_program_id);
	enforce_program_status(M_image_program_id, GL_Statuses::LINK_STATUS);
}

void Image_Shader_Program::set_middle_line_color(J_UI_Color i_color){
	int uniform_loc = glGetUniformLocation(M_image_program_id, "u_middle_line_color");
	assert(-1 != uniform_loc);
	glProgramUniform4fv(M_image_program_id, uniform_loc, 1, i_color.M_data.data());
}




J_GL_Circle_Shader::J_GL_Circle_Shader(){
	glPatchParameteri(GL_PATCHES, 1);
	j_uint vert_shader_id = load_vertex_shader(SHADER_BASE_PATH + "pass_through.vert");

	j_uint outline_frag_id = load_fragment_shader(SHADER_BASE_PATH + "box_outline.frag");


	j_uint fill_frag_id = load_fragment_shader(SHADER_BASE_PATH + "box.frag");


	M_circle_fill_prog_id = glCreateProgram();
	glAttachShader(M_circle_fill_prog_id, vert_shader_id);
	glAttachShader(M_circle_fill_prog_id, fill_frag_id);
	glLinkProgram(M_circle_fill_prog_id);
	enforce_program_status(M_circle_fill_prog_id, GL_Statuses::LINK_STATUS);

	M_circle_outine_prog_id = glCreateProgram();
	glAttachShader(M_circle_outine_prog_id, vert_shader_id);
	glAttachShader(M_circle_outine_prog_id, outline_frag_id);
	glLinkProgram(M_circle_outine_prog_id);
	enforce_program_status(M_circle_outine_prog_id, GL_Statuses::LINK_STATUS);
}


void J_GL_Circle_Shader::set_fill_color(const J_UI_Color& i_color){
	int uniform_loc = glGetUniformLocation(M_circle_fill_prog_id, "u_box_color");
	assert(-1 != uniform_loc);
	glProgramUniform4fv(M_circle_fill_prog_id, uniform_loc, 1, i_color.M_data.data());
}

void J_GL_Circle_Shader::set_outline_color(const J_UI_Color& i_color){
	int uniform_loc = glGetUniformLocation(M_circle_outine_prog_id, "u_outline_color");
	assert(-1 != uniform_loc);
	glProgramUniform4fv(M_circle_outine_prog_id, uniform_loc, 1, i_color.M_data.data());
}

j_uint J_GL_Circle_Shader::fill_program_id()const{ return M_circle_fill_prog_id; }

j_uint J_GL_Circle_Shader::outline_program_id()const{return M_circle_outine_prog_id;}


void J_GL_Circle_Shader::set_radius(j_float){
	assert(!"Method makes no sense to have based on how circle is created.");
	
}

/*j_uint load_vertex_shader(const string&)*/
j_uint load_vertex_shader(const std::string& irk_filename){
	string vert_shader_str(file_to_str(irk_filename));

#ifdef PRINT_SHADER_SOURCE
	cerr << '\n' << irk_filename << "\nVertex Shader Src:\n\n" << vert_shader_str << endl;
#endif
	j_uint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

	const char* shader_cstr = vert_shader_str.data();
	glShaderSource(vertex_shader_id, 1, &shader_cstr, '\0');
	glCompileShader(vertex_shader_id);

#if J_GL_DEBUG > 0
	enforce_shader_status(vertex_shader_id, GL_COMPILE_STATUS);
#endif
	return vertex_shader_id;
}

/*j_uint load_fragment_Shader(const string&)*/
j_uint load_fragment_shader(const std::string& irk_filename){
	string frag_shader_str(file_to_str(irk_filename));

#ifdef PRINT_SHADER_SOURCE	
	cerr << '\n' << irk_filename << " Fragment Shader Src:\n\n" << frag_shader_str << endl;
#endif
	j_uint frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	const char* shader_cstr = frag_shader_str.data();
	glShaderSource(frag_shader_id, 1, &shader_cstr, '\0');
	glCompileShader(frag_shader_id);

#if J_GL_DEBUG > 0
	enforce_shader_status(frag_shader_id, GL_COMPILE_STATUS);
#endif
	return frag_shader_id;
}

/*j_uint load_tessellation_control_shader(const string&)*/
j_uint load_tessellation_control_shader(const std::string& irk_src){

#ifdef PRINT_SHADER_SOURCE	
	cerr << "\nTCS Shader Src:\n\n" << irk_src << endl;
#endif
	j_uint tess_control_shader_id = glCreateShader(GL_TESS_CONTROL_SHADER);

	const char* shader_cstr = irk_src.data();
	glShaderSource(tess_control_shader_id, 1, &shader_cstr, '\0');
	glCompileShader(tess_control_shader_id);

#if J_GL_DEBUG > 0
	enforce_shader_status(tess_control_shader_id, GL_COMPILE_STATUS);
#endif
	return tess_control_shader_id;
}

/*j_uint load_tessellation_evaluation_shader(const string&)*/
j_uint load_tessellation_evaluation_shader(const std::string& irk_src){

#ifdef PRINT_SHADER_SOURCE	
	cerr << "\nTES Shader Src:\n\n" << irk_src << endl;
#endif
	j_uint tess_eval_shader_id = glCreateShader(GL_TESS_EVALUATION_SHADER);

	const char* shader_cstr = irk_src.data();
	glShaderSource(tess_eval_shader_id, 1, &shader_cstr, '\0');
	glCompileShader(tess_eval_shader_id);

#if J_GL_DEBUG > 0
	enforce_shader_status(tess_eval_shader_id, GL_COMPILE_STATUS);
#endif
	return tess_eval_shader_id;
}



/*void enforce_chader_status(GLenum)*/
void enforce_shader_status(j_uint i_shader_id, GLenum i_status){
	GLint shader_status;
	glGetShaderiv(i_shader_id, i_status, &shader_status);

	if(shader_status){
		return;
	}

	GLsizei log_len;
	glGetShaderiv(i_shader_id, GL_INFO_LOG_LENGTH, &log_len);
	string log;
	log.resize(log_len + 1);
	glGetShaderInfoLog(i_shader_id, log_len, &log_len, &log[0]);
	std::cerr << "\nShader Status Check Fail:\n" << log << std::endl;

	assert(!"Shader Status Check Fail!");

}

/*void enforce_program_status(j_uint, GLenum)*/
void enforce_program_status(j_uint i_program_id, GL_Statuses i_status){
	GLint program_status = s_open_gl.get_program_info(i_program_id, i_status);


	

	if(program_status){
		return;
	}

	GLsizei log_len;
	glGetProgramiv(i_program_id, GL_INFO_LOG_LENGTH, &log_len);
	string log;
	log.resize(log_len + 1);
	glGetProgramInfoLog(i_program_id, log_len, &log_len, &log[0]);
	std::cerr << "\nProgram Link Fail:\n" << log << std::endl;
	assert(!"Could Not Link Program!");

}

J_GL_Line_Shader::J_GL_Line_Shader(){
	M_program_id = glCreateProgram();
	j_uint outline_shader_id = load_vertex_shader(SHADER_BASE_PATH + "quad.vert");
	j_uint outline_frag_shader_id = load_fragment_shader(SHADER_BASE_PATH + "box_outline.frag");

	glAttachShader(M_program_id, outline_shader_id);
	glAttachShader(M_program_id, outline_frag_shader_id);
	glLinkProgram(M_program_id);
	enforce_program_status(M_program_id, GL_Statuses::LINK_STATUS);
	//cerr << "\n Shader Program ID: " << M_program_id << "*********************************************************";
}

int J_GL_Line_Shader::program_id(){
	return M_program_id;
}

void J_GL_Line_Shader::set_fill_color(const J_UI_Color& i_color){
	int uniform_loc = glGetUniformLocation(program_id(), "u_outline_color");
	assert(-1 != uniform_loc);
	glProgramUniform4fv(program_id(), uniform_loc, 1, i_color.M_data.data());
	assert(!open_gl_error());
}



J_GL_Shader_Program::~J_GL_Shader_Program(){
	for(auto& f_program : M_program_ids){
		glDeleteProgram(f_program);
		
	}
	for(auto f_shader : M_shader_ids){
		glDeleteShader(f_shader);
	}
}

void J_GL_Shader_Program::add_program_id(j_uint i_program_id){
	M_program_ids.push_back(i_program_id);
}

void J_GL_Shader_Program::add_shader_id(j_uint i_shader_id){
	M_shader_ids.push_back(i_shader_id);
}

}