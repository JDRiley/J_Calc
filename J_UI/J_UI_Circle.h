#ifndef J_UI_CIRCLE_H
#define J_UI_CIRCLE_H

#include "J_UI_Object.h"
//
#include <J_Circle.h>
//
#include <J_GL_Objects.h>
namespace jomike{


class J_UI_Circle : public J_UI_Object, public J_Circle{
public:
	J_UI_Circle(const J_Circle&);
	void draw()const override;

	using J_Circle::set_circle;
	using J_Circle::set_center;

	void set_x(j_float)override;
	void set_y(j_float)override;
	void set_radius(j_float)override;
	void set_center(j_float, j_float)override;

	void set_circle(j_float i_x_pos, j_float i_y_pos, j_float i_radius)override;
protected:
	const J_GL_Vertex_Array& get_circle_vao()const;
private:
	void recalc_vertex_data();

	J_GL_Vertex_Array M_circle_vao;
	J_GL_Buffer M_vao_buffer;
	int M_num_points = 50;

	J_GL_Circle_Shader_Unique_t M_shader;

	void derived_set_fill_color()override;
	void derived_set_outline_color()override;

	void init_program();
	void gen_vaos();

};
}

#endif //J_UI_CIRCLE_H


