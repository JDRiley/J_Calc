#ifndef J_SHAPE_H
#define J_SHAPE_H

#include "J_Geometry_Fwd_Decl.h"


namespace jomike{

//Virtual Class
class J_Shape{
public:
	void set_center(Pen_Pos_FL_t);
	virtual void set_center(j_float i_x_pos, j_float i_y_pos) = 0;
	virtual J_Shape* get_copy()const = 0;
	virtual j_float area()const = 0;
	virtual j_float perimeter()const = 0;
	virtual bool is_inside(j_float, j_float)const = 0;
	bool is_inside(Pen_Pos_FL_t)const;
private:
};
}

#endif //J_SHAPE_H


