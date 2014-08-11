#ifndef J_LINE_H
#define J_LINE_H

#include "J_Shape.h"


namespace jomike{
class J_Line : public virtual J_Shape{
public:
	J_Line();
	J_Line(j_float i_x1, j_float i_y1, j_float i_x2, j_float i_y2);
	virtual void set_line(const J_Line& i_line);
	void set_points(j_float i_x1, j_float i_y1, j_float i_x2, j_float i_y2);
	J_Line* get_copy()const override;

	j_float area()const override;

	j_float perimeter()const override;

	bool is_inside(j_float, j_float)const override;

	void set_center(j_float i_x_pos, j_float i_y_pos)override;

	j_float x1()const;
	j_float y1()const;
	j_float x2()const;
	j_float y2()const;

private:
	j_float M_x1 = 0.0f;
	j_float M_y1 = 0.0f;
	j_float M_x2 = 0.0f;
	j_float M_y2 = 0.0f;
};


}

#endif //J_LINE_H


