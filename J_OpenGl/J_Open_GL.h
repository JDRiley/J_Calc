#ifndef J_OPEN_GL_H
#define J_OPEN_GL_H

#include "J_OpenGL_Fwd_Decl.h"
//
#include "OpenGL_Enums.h"

namespace jomike{
class J_Open_GL{
public:
	J_Open_GL();
	//
	void bind_texture(Texture_Target, j_uint i_texture_id);
	//
	void bind_texture_2D(j_uint i_texture_id);
	//
	void bind_vertex_array(j_uint i_vao_id);
	//
	void use_program(j_uint i_program_id);
private:

};

}

#endif //J_OPEN_GL_H


