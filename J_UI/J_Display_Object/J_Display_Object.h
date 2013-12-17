#ifndef J_DISPLAY_OBJECT_H
#define J_DISPLAY_OBJECT_H

#include "../J_UI_Fwd_Decl.h"
#include "../J_Image/J_Image.h"
#include "../J_Shape.h"


namespace jomike{

class J_Display_Object : public J_Outline_Fill_Management, public virtual J_Shape{
public:


	virtual void draw()const = 0;
	virtual void alert_resize(int,int) = 0;

	
	virtual ~J_Display_Object();
	virtual void set_fill_color(const J_UI_Color&)=0;
	virtual void set_outline_color(const J_UI_Color&)=0;

	virtual void mouse_button_press(J_View_Shared_t, int i_button, int modifiers, Pen_Pos_FL_t);
	virtual void mouse_button_release(J_View_Shared_t, int i_button, int modifiers, Pen_Pos_FL_t);

	bool clickable_status()const;
	void set_clickable_status(bool);
	
	j_uint get_ID()const{ return M_ID; }
	j_uint get_object_ID()const{return M_UI_Obj_ID;}

#ifdef VS_2013
	J_Display_Object(const J_Display_Object&) = delete;
	J_Display_Object& operator=(const J_Display_Object&) = delete;
#endif // VS_2013
protected:
	J_Display_Object(j_uint);
	J_Display_Object(j_uint i_object_id, j_uint i_id);
private:
	const j_uint M_ID;
	j_uint M_UI_Obj_ID;

	bool M_clickable_flag;
	
#ifndef VS_2013
	J_Display_Object(const J_Display_Object&);
	J_Display_Object& operator=(const J_Display_Object&);
#endif //VS_2013
};


/*class J_Display_Boxs*/
class J_Display_Box : public J_Display_Object , public J_Rectangle{
public:
	J_Display_Box(j_uint);
	
	void draw()const;

	void set_box(j_float , j_float, j_float , j_float )override;
	void set_fill_color(const J_UI_Color& i_color)override;
	void set_outline_color(const J_UI_Color& i_color)override;
	void alert_resize(int,int)override{}

	void set_box(const J_Rectangle&)override;

	void set_width(j_float)override;
	void set_height(j_float)override;
	~J_Display_Box();
protected:
	j_uint get_box_vao()const;
	J_Display_Box(j_uint i_object_id, j_uint i_id);

	

private:
	//Graphics MAnagement
	j_uint M_box_vao_id;
	j_uint M_vao_buffer_id;

	J_GL_Box_Shader* M_shader;

	void initialization();
	void gen_vaos();
	void init_program();
	
};



/*class J_Display_Circle*/
class J_Display_Circle : public J_Display_Object, public J_Circle{
public:
	using J_Circle::set_circle;
	using J_Circle::set_center;

	J_Display_Circle(j_uint);
	void draw()const override;

	
	void set_fill_color(const J_UI_Color&)override;
	void set_outline_color(const J_UI_Color&)override;
	void alert_resize(int, int)override{}



	void set_circle(j_float, j_float, j_float i_radius)override;
	void set_x(j_float)override;
	void set_y(j_float)override;
	void set_radius(j_float)override;
	void set_center(j_float, j_float)override;

	~J_Display_Circle();
protected:
	j_uint get_box_vao()const;
private:
	J_GL_Circle_Shader* M_shader;

	//Graphics MAnagement
#ifdef VS_2013
	j_uint M_circle_vao_id = 0u;
	j_uint M_vao_buffer_id = 0u;
	int M_num_points = 50;
#else
	j_uint M_circle_vao_id;
	j_uint M_vao_buffer_id;
	int M_num_points;
	void default_initialization();
#endif
	


	void gen_vaos();
	void recalc_vertex_data();
	void init_program();

};

}


#endif