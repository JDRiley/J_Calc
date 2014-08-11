#ifndef J_RECTANGLE_H
#define J_RECTANGLE_H

#include "J_Shape.h"


namespace jomike{

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

	void set_pos(Pen_Pos_FL_t i_pos);
	void set_center(j_float, j_float)override;

	bool is_inside(j_float, j_float)const override;
	bool is_y_inside(j_float i_y)const;
	bool is_x_inside(j_float i_x)const;

	j_float area()const override;
	j_float perimeter()const override;
	void set_x2(j_float end_x_pos);
	virtual void set_box(j_float x1, j_float x2, j_float y1, j_float y2);
	virtual void set_box(const J_Rectangle&);
protected:
	virtual void set_rectangle(const J_Rectangle&);

private:
	j_float M_x_pos, M_y_pos, M_width, M_height;
};

}

#endif //J_RECTANGLE_H


