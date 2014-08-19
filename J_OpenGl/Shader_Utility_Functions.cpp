#include "Shader_Utility_Functions.h"
//Libraries
#include <GL\glew.h>

//
#include <iostream>

using std::cerr; using std::cout; using std::endl;

//
#include <string>
using std::string;

//
#include "J_Open_GL.h"

//#define PRINT_SHADER_SOURCE
//#define J_GL_DEBUG 1
namespace jomike{

static J_Open_GL s_open_gl;

/*j_uint load_vertex_shader(const string&)*/
j_uint load_vertex_shader(const std::string& irk_vert_shader_src){


#ifdef PRINT_SHADER_SOURCE
	cerr << '\n' << irk_filename << "\nVertex Shader Src:\n\n" << irk_vert_shader_src << endl;
#endif
	j_uint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

	const char* shader_cstr = irk_vert_shader_src.data();
	glShaderSource(vertex_shader_id, 1, &shader_cstr, '\0');
	glCompileShader(vertex_shader_id);

#if J_GL_DEBUG > 0
	enforce_shader_status(vertex_shader_id, GL_COMPILE_STATUS);
#endif
	return vertex_shader_id;
}

/*j_uint load_fragment_Shader(const string&)*/
j_uint load_fragment_shader(const std::string& irk_frag_shader_src){


#ifdef PRINT_SHADER_SOURCE	
	cerr << '\n' << irk_filename << " Fragment Shader Src:\n\n" << irk_frag_shader_src << endl;
#endif
	j_uint frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	const char* shader_cstr = irk_frag_shader_src.data();
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
}



