#ifndef J_SHADER_PROGRAM_H
#define J_SHADER_PROGRAM_H

#include "J_OpenGL_Fwd_Decl.h"
//
#include <J_Image.h>
//
#include <string>
namespace jomike{
const std::string SHADER_BASE_PATH = "J:/JoMike Library/J_OpenGl/Shaders/";
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
void enforce_shader_status(j_uint shader_id, j_uint i_status);

/*void enforce_program_status(GLenum)*/
/*
Checks the given status of the given shader. Throws exception and prints on failure
*/
void enforce_program_status(j_uint program_id, j_uint i_status);




class J_GL_Shader_Program{
public:

private:

};



class J_GL_Box_Shader : public J_GL_Shader_Program{
public:
	J_GL_Box_Shader();
	void attach_box_program();
	void attach_outline_program();
	void set_fill_color(J_Color_RGBA<j_float>);
	void set_outline_color(J_Color_RGBA<j_float>);

	j_uint box_program_id()const{return M_box_prog_id;}
	j_uint outline_program_id()const{return M_outline_prog_id;}
private:
	j_uint M_box_prog_id;
	j_uint M_outline_prog_id;
};


class Image_Shader_Program : public J_GL_Shader_Program{
public:
	Image_Shader_Program(Image_Format i_format);
	j_uint image_program_id()const;
	void set_format(Image_Format);
	void set_middle_line_color(J_UI_Color);
private:
	j_uint M_image_program_id, M_vert_shader_id, M_image_frag_id;
	Image_Format M_format;
};



class J_GL_Circle_Shader : public J_GL_Shader_Program{
public:
	J_GL_Circle_Shader();
	j_uint fill_program_id()const;
	j_uint outline_program_id()const;
	void set_fill_color(const J_UI_Color&);
	void set_outline_color(const J_UI_Color&);
	void set_radius(j_float);
private:
	j_uint M_circle_fill_prog_id;
	j_uint M_circle_outine_prog_id;
};


}

#endif