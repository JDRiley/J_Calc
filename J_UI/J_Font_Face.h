#include "Bitmap_Metrics.h"
#ifndef J_FONT_FACE_H
#define J_FONT_FACE_H

#include "J_UI_Fwd_Decl.h"
//
#include <ex_array.h>
//
#include "Bitmap_Metrics.h"
//
#include "J_Shape.h"
namespace jomike{
class J_Font_Face__{
public:
	J_Font_Face__(J_Context_Shared_t i_context
				  , FT_Face i_face, int i_font_size, int i_render_mode);

	Bitmap_Metrics& bitmap_metric(j_size_t i_index);
	const Bitmap_Metrics& bitmap_metric(j_size_t i_index)const;

	const j_ubyte* get_data(j_size_t i_index);
	j_size_t num_glyphs()const;
	int new_line_size()const;
private:
	int M_tab_distance;
	int M_new_line_size;
	ex_array<j_ubyte_arr_t> M_bitmap_datas;
	ex_array<Bitmap_Metrics> M_bitmap_metrics;
};



class J_FT_Char_Load_Error : public J_Error{
public:
	J_FT_Char_Load_Error(const char* const ik_message, j_ulint i_charcode
						 , int i_loadflag): J_Error(ik_message), M_charcode(i_charcode)
						 , M_load_flag(i_loadflag){}
private:
	j_ulint M_charcode;
	int M_load_flag;

};


}

#endif //J_FONT_FACE_H


