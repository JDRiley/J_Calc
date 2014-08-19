#ifndef SHADER_UTILITY_FUNCTIONS_H
#define SHADER_UTILITY_FUNCTIONS_H

#include "J_OpenGL_Fwd_Decl.h"
//
#include "OpenGL_Enums.h"
//
#include <J_Image.h>
//
#include <string>


namespace jomike{
/*j_uint load_vertex_shader*/
/*
	Loads a vertex shader and returns It's id.
	Warning j_uint is locally typedef'd to unsigned
*/
j_uint load_vertex_shader(const std::string& irk_source);

/*j_uint load_fragment_shader*/
/*
	Loads a fragment shader and returns It's id.
	Warning j_uint is locally typedef'd to unsigned
*/
j_uint load_fragment_shader(const std::string& irk_source);


///j_uint load_tessellation_control_shader(const std::string&)
/*																			*/
/*		Loads a tessellation control shader using string as source file		*/
/*																			*/
j_uint load_tessellation_control_shader(const std::string&);

///j_uint load_tessellation_evaluation_shader(const std::string&)
/*																			*/
/*		loads a tessellation evaluation shader using string as source file	*/
/*																			*/
j_uint load_tessellation_evaluation_shader(const std::string&);

/*void enforce_shader_status(GLenum)*/
/*
Checks the given status of the given shader. Throws exception and prints on failure
*/
void enforce_shader_status(j_uint shader_id, GL_Statuses i_status);

/*void enforce_program_status(GLenum)*/
/*
Checks the given status of the given shader. Throws exception and prints on failure
*/
void enforce_program_status(j_uint program_id, GL_Statuses i_status);
}

#endif //SHADER_UTILITY_FUNCTIONS_H


