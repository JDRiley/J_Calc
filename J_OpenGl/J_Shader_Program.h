#ifndef J_SHADER_PROGRAM_H
#define J_SHADER_PROGRAM_H

#include "J_OpenGL_Fwd_Decl.h"
//
#include "OpenGL_Enums.h"
//
#include <J_Image.h>
//
#include <string>
namespace jomike{
//const std::string SHADER_BASE_PATH = "Shaders/";


class J_GL_Shader_Program{
public:
	~J_GL_Shader_Program();
protected:

	j_uint load_fragment_shader(const char* const ik_shader_src);
	j_uint load_vertex_shader(const char* const ik_shader_src);

	j_uint create_program();

	int get_uniform_location(j_uint i_program_id, const char* const ik_uniform_name);
	void attach_shader_to_program(j_uint i_program_id, j_uint i_shader_id);

	void link_program(j_uint i_program_id);

	void enforce_program_status(j_uint i_program_id, GL_Statuses i_status)const;

	void program_uniform_4v(j_uint i_program_id, int i_uniform_loc, int i_count, const j_float* i_data);

private:
	ex_array<j_uint> M_program_ids;
	ex_array<j_uint> M_shader_ids;
};



class J_GL_Box_Shader : public J_GL_Shader_Program{
public:
	J_GL_Box_Shader();
	/*void attach_box_program();
	void attach_outline_program();*/
	void set_fill_color(J_Color_RGBA<j_float>);
	void set_outline_color(J_Color_RGBA<j_float>);

	j_uint box_program_id()const{return M_box_prog_id;}
	j_uint outline_program_id()const{return M_outline_prog_id;}

	
	//This should be protected but function make_new() in J_UI_Letter_Box.cpp does not 
	// compile for some reason when declared as protected
	J_GL_Box_Shader(j_uint i_box_program_id, j_uint i_outline_program_id);
protected:
//	J_GL_Box_Shader(j_uint i_box_program_id, j_uint i_outline_program_id);
	
private:
	j_uint M_box_prog_id;
	j_uint M_outline_prog_id;
};


class Image_Shader_Program : public J_GL_Shader_Program{
public:
	Image_Shader_Program(Image_Format i_format);
	j_uint program_id()const;


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

class J_GL_Line_Shader : public J_GL_Shader_Program{
public:
	J_GL_Line_Shader();
	int program_id();
	void set_fill_color(const J_UI_Color& i_color);
private:
	
	int M_program_id;
};



}

#endif