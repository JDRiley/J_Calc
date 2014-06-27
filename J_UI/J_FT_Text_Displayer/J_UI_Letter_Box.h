#ifndef J_DISPLAY_LETTER_BOX_H
#define J_DISPLAY_LETTER_BOX_H

#include "../J_UI_Fwd_Decl.h"
//
#include "../J_Image_Pane.h"
//
#include "../Bitmap_Metrics.h"
//
#include <J_GL_Objects.h>
namespace jomike{



class J_UI_Letter_Box : public J_Image_Pane{
public:
	J_UI_Letter_Box(const J_Rectangle& irk_rectangle);

	void set_image_box(const Pen_Pos_FL_t &i_pos, const Bitmap_Metrics &i_metrics);

	void draw()const override;
	void draw_to_frame_buffer(j_uint i_frame_buffer)const;
	void set_buffer_data(const Pen_Pos_FL_t& i_pos, const Bitmap_Metrics& i_metrics
					, const J_UI_Color& i_color, const j_ubyte* i_buffer);
	void set_buffer_data(const Bitmap_Metrics& i_metrics, const J_UI_Color& i_color
						 , const j_ubyte* i_data);
	
	void set_buffer_data(int i_width, int i_height, const J_UI_Color& i_color, const j_ubyte* i_bitmap);

	~J_UI_Letter_Box();
private:
	j_ubyte_arr_t M_colored_bitmap;
	Bitmap_Metrics M_metrics;
};

}

#endif //J_DISPLAY_LETTER_BOX_H

