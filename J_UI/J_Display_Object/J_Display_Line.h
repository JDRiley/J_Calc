#ifndef J_DISPLAY_LINE_H
#define J_DISPLAY_LINE_H

#include "J_Display_Object.h"


namespace jomike{
class J_Display_Line : public J_Display_Object, public J_Line{
public:
	J_Display_Line(j_uint i_obj_id);
	void set_line(const J_Line& i_line)override;
	void draw()const override;

	void alert_resize(int, int)override;

	void set_fill_color(const J_UI_Color&)override;
	void set_outline_color(const J_UI_Color&)override;
private:
	j_uint M_vao_id = 0;
	j_uint M_buffer_id = 0;
	void init_vao();
	void recalculate_vao();

};

}

#endif //J_DISPLAY_LINE_H


