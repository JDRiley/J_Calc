#ifndef J_CIRCLE_H
#define J_CIRCLE_H

#include "J_Shape.h"


namespace jomike{
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

#endif //J_CIRCLE_H


