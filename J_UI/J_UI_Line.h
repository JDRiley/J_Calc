#ifndef J_UI_LINE_H
#define J_UI_LINE_H

#include "J_UI_Object.h"
//
#include <J_OpenGL_Fwd_Decl.h>
//
#include <J_GL_Objects.h>
namespace jomike{
class J_UI_Line : public J_UI_Object, public J_Line{
public:
	J_UI_Line();
	void draw()const override;
	J_UI_Line(j_float i_x1, j_float i_y1, j_float i_x2, j_float i_y2);
	void set_line(const J_Line& i_line)override;

private:
	J_GL_Vertex_Array M_vao;
	J_GL_Buffer M_vao_buffer;

	J_GL_Line_Shader_Unique_t M_shader;
	void derived_set_fill_color()override;
	void derived_set_outline_color()override;

	void init_vao();
	void recalculate_vao();
};

}

#endif //J_UI_LINE_H

