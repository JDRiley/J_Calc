#ifndef J_SHAPE_H
#define J_SHAPE_H

#include <J_Fwd_Decl.h>


namespace jomike{


class J_Shape{
public:
	void set_center(Pen_Pos_FL_t);
	virtual void set_center(j_float i_x_pos, j_float i_y_pos) = 0;
	virtual J_Shape* get_copy()const =0;
	virtual j_float area()const= 0;
	virtual j_float perimeter()const= 0;
	virtual bool is_inside(j_float, j_float)const = 0;
	bool is_inside(Pen_Pos_FL_t)const;
private:
};

class J_Outline_Fill_Management{
public:
#ifndef VS_2013
	J_Outline_Fill_Management();
#endif // !VS_2013

	

	bool outline_visibility_status()const;
	bool fill_visibility_status()const;
	j_float outline_thickness()const;

	virtual void set_outline_visibility_status(bool);
	virtual void set_fill_visibility_status(bool);
	virtual void set_outline_thickness(j_float);
	void set_outline_and_fill_visibility_status(bool);
private:
#if VS_2013
	j_float M_outline_thickness = 1.0f;
	bool M_outline_visibility_status = false;
	bool M_fill_visibility_status = false;
#else
	j_float M_outline_thickness;
	bool M_outline_visibility_status;
	bool M_fill_visibility_status;
	void default_initialization();
#endif
};

class J_Rectangle : public virtual J_Shape{
public:
	using J_Shape::is_inside;
	J_Rectangle* get_copy()const;
	J_Rectangle();
	J_Rectangle(j_float i_x1, j_float i_x2, j_float i_width, j_float i_height);
	j_float x1()const;
	j_float x2()const;
	j_float y1()const;
	j_float y2()const;

	j_float width()const;
	j_float height()const;

	virtual void set_x(j_float);
	virtual void set_y(j_float);
	virtual void set_width(j_float);
	virtual void set_height(j_float);

	
	void set_center(j_float, j_float)override;
	
	bool is_inside(j_float, j_float)const override;
	j_float area()const override;
	j_float perimeter()const override;
	
	virtual void set_box(j_float x1, j_float x2, j_float y1, j_float y2);
	virtual void set_box(const J_Rectangle&);
protected:
	virtual void set_rectangle(const J_Rectangle&);
private:
	j_float M_x_pos, M_y_pos, M_width, M_height;
};

class J_Circle : public virtual J_Shape{
public:
	using J_Shape::is_inside;
	J_Circle* get_copy()const;
	J_Circle();
	J_Circle(j_float i_x1, j_float i_y1, j_float i_radius);
	j_float x1()const;
	j_float y1()const;
	j_float radius()const;

	virtual void set_x(j_float);
	virtual void set_y(j_float);
	virtual void set_radius(j_float);
	virtual void set_center(j_float, j_float);
	
	bool is_inside(j_float, j_float)const;
	j_float area()const override;
	j_float perimeter()const override;
	void set_circle(const J_Circle&);
	virtual void set_circle(j_float i_x_pos, j_float i_y_pos, j_float i_radius);
	
protected:
	
private:
#ifdef VS_2013
	j_float M_x_pos = 0.0f;
	j_float M_y_pos = 0.0f;
	j_float M_radius = 0.0f;
#else
	j_float M_x_pos;
	j_float M_y_pos;
	j_float M_radius;
	void default_initialization();
#endif
};


}

#endif