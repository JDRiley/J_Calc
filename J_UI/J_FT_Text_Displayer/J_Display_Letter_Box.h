#ifndef J_DISPLAY_LETTER_BOX_H
#define J_DISPLAY_LETTER_BOX_H

#include "../J_UI_Fwd_Decl.h"
//
#include "../J_Font_Manager.h"
//
#include "../J_Display_Object/J_Display_Object.h"

namespace jomike{

class J_Display_Letter_Box : public J_Display_Box{
public:
	J_Display_Letter_Box(j_uint i_obj_id = 0);

	void set_image_box(const Pen_Pos_FL_t &i_pos, const Bitmap_Metrics &i_metrics);

	void draw()const override;
	void set_buffer(const Pen_Pos_FL_t& i_pos, const Bitmap_Metrics& i_metrics
					, const J_UI_Color& i_color, const j_ubyte* i_buffer);
	void set_buffer_data(const Bitmap_Metrics& i_metrics, const J_UI_Color& i_color
						 , const j_ubyte* i_data);
	
	void set_buffer_data(int i_width, int i_height, const J_UI_Color& i_color, const j_ubyte* i_bitmap);
private:
	j_uint M_texture_id = 0;
};

}

#endif //J_DISPLAY_LETTER_BOX_H

