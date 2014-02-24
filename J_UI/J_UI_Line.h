#ifndef J_UI_LINE_H
#define J_UI_LINE_H

#include "J_UI_Object.h"


namespace jomike{
class J_UI_Line : public J_UI_Object, public J_Line{
public:
	J_UI_Line();
	J_UI_Line(j_float i_x1, j_float i_y1, j_float i_x2, j_float i_y2);
	void set_line(const J_Line& i_line)override;
	void broadcast_current_state()const override;
private:


};

}

#endif //J_UI_LINE_H

