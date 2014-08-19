#include "J_Shader_Program.h"
//
#include "Shader_Program_Sources.h"

//this file created to seperate responsibility of loading and managing lifetime of shaders
// and program to only J_GL_Shader_Program
namespace jomike{
class Quad_Vert_Shader_Class : public J_GL_Shader_Program{
public:
	static Quad_Vert_Shader_Class& get_instance(){
		static Quad_Vert_Shader_Class vert_shader;
		return vert_shader;
	}

	int id()const{
		return M_vert_shader_id;
	}
private:
	int M_vert_shader_id;

	Quad_Vert_Shader_Class(){
		M_vert_shader_id = load_vertex_shader(gk_quad_vert_source);
	}
};

static Instance_Pointer<Quad_Vert_Shader_Class> s_vert_shader;


J_GL_Box_Shader::J_GL_Box_Shader(){
	j_uint vert_shader_id = s_vert_shader->id();
	j_uint box_frag_id = load_fragment_shader(gk_box_frag_source);
	j_uint outline_frag_id = load_fragment_shader(gk_box_outline_frag_source);


	//do not add vert_shader_id as that id's life time is not managed by the shader
	//add_shader_id(vert_shader_id);



	M_box_prog_id = create_program();

	attach_shader_to_program(M_box_prog_id, vert_shader_id);
	attach_shader_to_program(M_box_prog_id, box_frag_id);
	link_program(M_box_prog_id);
	enforce_program_status(M_box_prog_id, GL_Statuses::LINK_STATUS);



	M_outline_prog_id = create_program();

	attach_shader_to_program(M_outline_prog_id, vert_shader_id);
	attach_shader_to_program(M_outline_prog_id, outline_frag_id);
	link_program(M_outline_prog_id);
	enforce_program_status(M_outline_prog_id, GL_Statuses::LINK_STATUS);
}

void J_GL_Box_Shader::set_fill_color(J_Color_RGBA<j_float> i_color){
	int uniform_loc = get_uniform_location(M_box_prog_id, "u_box_color");
	program_uniform_4v(M_box_prog_id, uniform_loc, 1, i_color.M_data.data());
}

void J_GL_Box_Shader::set_outline_color(J_Color_RGBA<j_float> i_color){
	int uniform_loc = get_uniform_location(M_outline_prog_id, "u_outline_color");
	program_uniform_4v(M_outline_prog_id, uniform_loc, 1, i_color.M_data.data());
}


Image_Shader_Program::Image_Shader_Program(Image_Format):M_image_program_id(0){
	M_vert_shader_id = s_vert_shader->id();
	M_image_frag_id = load_fragment_shader(gk_quad_image_frag_source);




	M_image_program_id = create_program();



	attach_shader_to_program(M_image_program_id, M_vert_shader_id);
	attach_shader_to_program(M_image_program_id, M_image_frag_id);
	link_program(M_image_program_id);
	enforce_program_status(M_image_program_id, GL_Statuses::LINK_STATUS);
}

j_uint Image_Shader_Program::program_id()const{
	return M_image_program_id;
}



J_GL_Circle_Shader::J_GL_Circle_Shader(){
	//glPatchParameteri(GL_PATCHES, 1);
	j_uint vert_shader_id = load_vertex_shader(gk_pass_through_vert_source);

	j_uint outline_frag_id = load_fragment_shader(gk_box_outline_frag_source);


	j_uint fill_frag_id = load_fragment_shader(gk_box_frag_source);




	M_circle_fill_prog_id = create_program();



	attach_shader_to_program(M_circle_fill_prog_id, vert_shader_id);
	attach_shader_to_program(M_circle_fill_prog_id, fill_frag_id);
	link_program(M_circle_fill_prog_id);
	enforce_program_status(M_circle_fill_prog_id, GL_Statuses::LINK_STATUS);

	M_circle_outine_prog_id = create_program();



	attach_shader_to_program(M_circle_outine_prog_id, vert_shader_id);
	attach_shader_to_program(M_circle_outine_prog_id, outline_frag_id);
	link_program(M_circle_outine_prog_id);
	enforce_program_status(M_circle_outine_prog_id, GL_Statuses::LINK_STATUS);
}


void J_GL_Circle_Shader::set_fill_color(const J_UI_Color& i_color){
	int uniform_loc = get_uniform_location(M_circle_fill_prog_id, "u_box_color");
	program_uniform_4v(M_circle_fill_prog_id, uniform_loc, 1, i_color.M_data.data());
}

void J_GL_Circle_Shader::set_outline_color(const J_UI_Color& i_color){
	int uniform_loc = get_uniform_location(M_circle_outine_prog_id, "u_outline_color");
	program_uniform_4v(M_circle_outine_prog_id, uniform_loc, 1, i_color.M_data.data());
}

j_uint J_GL_Circle_Shader::fill_program_id()const{ return M_circle_fill_prog_id; }

j_uint J_GL_Circle_Shader::outline_program_id()const{ return M_circle_outine_prog_id; }


void J_GL_Circle_Shader::set_radius(j_float){
	assert(!"Method makes no sense to have based on how circle is created.");

}



J_GL_Line_Shader::J_GL_Line_Shader(){
	M_program_id = create_program();
	j_uint outline_shader_id = s_vert_shader->id();
	j_uint outline_frag_shader_id = load_fragment_shader(gk_box_outline_frag_source);




	attach_shader_to_program(M_program_id, outline_shader_id);
	attach_shader_to_program(M_program_id, outline_frag_shader_id);
	link_program(M_program_id);
	enforce_program_status(M_program_id, GL_Statuses::LINK_STATUS);
	//cerr << "\n Shader Program ID: " << M_program_id << "*********************************************************";
}

int J_GL_Line_Shader::program_id(){
	return M_program_id;
}

void J_GL_Line_Shader::set_fill_color(const J_UI_Color& i_color){
	int uniform_loc = get_uniform_location(program_id(), "u_outline_color");
	program_uniform_4v(program_id(), uniform_loc, 1, i_color.M_data.data());

}



}



