#include "J_Open_GL.h"
//
#include "J_OpenGL.h"
//
#include <gl\glew.h>
//
#include <j_map.h>

namespace jomike{

static const j_map<Texture_Target, int> sk_texture_enum_map
= {
	{Texture_Target::TEXTURE_1D, GL_TEXTURE_1D}
	, {Texture_Target::TEXTURE_2D, GL_TEXTURE_2D}
	, {Texture_Target::TEXTURE_3D, GL_TEXTURE_3D}
	, {Texture_Target::TEXTURE_CUBE_MAP, GL_TEXTURE_CUBE_MAP}
};


void J_Open_GL::bind_texture(Texture_Target i_target, j_uint i_texture_id){
	glBindTexture(*sk_texture_enum_map.find(i_target), i_texture_id);
	assert(!open_gl_error());
}

void J_Open_GL::bind_texture_2D(j_uint i_texture_id){
	glBindTexture(*sk_texture_enum_map.find(Texture_Target::TEXTURE_2D), i_texture_id);
	assert(!open_gl_error());
}

void J_Open_GL::bind_vertex_array(j_uint i_vao_id){
	glBindVertexArray(i_vao_id);
	assert(!open_gl_error());
}

void J_Open_GL::use_program(j_uint i_program_id){
	assert(glIsProgram(i_program_id));
	glUseProgram(i_program_id);
}





}



