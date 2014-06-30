#ifndef J_UI_BOX_H
#define J_UI_BOX_H

#include "J_UI_Object.h"
//
#include "J_Shape.h"
//
#include "J_GL_Objects.h"

namespace jomike{

class J_UI_Box : public J_UI_Object, public J_Rectangle{
public:
	J_UI_Box(const J_Rectangle&);
	//void broadcast_current_state()const;
	

	void draw()const override;
	void set_y(j_float)override;
	void set_x(j_float)override;
	void set_width(j_float)override;
	void set_height(j_float)override;
	void set_box(j_float , j_float, j_float , j_float )override;
	
	void set_rectangle(const J_Rectangle&)override;
	void alert_resize(int, int)override;
	~J_UI_Box();
protected:
	//J_UI_Box(const J_Rectangle&, j_uint i_obj_id);
	const J_GL_Vertex_Array& get_box_vao()const;
	

private:
	J_GL_Vertex_Array M_box_vao;
	J_GL_Buffer M_vao_buffer;

	J_GL_Box_Shader_Unique_t M_shader;

	void initialization();
	void recalculate_vaos();
	void init_vaos();
	void derived_set_fill_color()override;
	void derived_set_outline_color()override;
};






}

#endif //J_UI_BOX_H


