#include "J_Font_Face.h"
//
#include <ft2build.h>
#include FT_FREETYPE_H
//
#include <J_Image.h>
//
#include <iostream>

using std::cerr;
namespace jomike{
const int FT_POINT_TO_PARAMETER_CONVERSION = 64;//64;
const int DEFAULT_DOTS_PER_INCH = 96*EXTRA_CHAR_DPI_FACTOR;//96;
const int DEFAULT_CHAR_POINT_SIZE = 24;
const int DEFAULT_NUMBER_OF_FT_LIBS = 1;
const int DEFAULT_NUMBER_OF_FT_FACES = 1;

static FT_GlyphSlot get_rendered_glyph(FT_Face i_face, j_ulint i_charcode, FT_Int32 i_load_flag);
J_Font_Face__::J_Font_Face__(J_Context_Shared_t i_context, FT_Face i_face
							 , int i_font_size, int i_render_mode):M_tab_distance(4)
	, M_new_line_size(static_cast<int>(i_face->height/FT_POINT_TO_PARAMETER_CONVERSION)){
	

	if(FT_Set_Char_Size(i_face, i_font_size*FT_POINT_TO_PARAMETER_CONVERSION,
		i_font_size*FT_POINT_TO_PARAMETER_CONVERSION, DEFAULT_DOTS_PER_INCH, DEFAULT_DOTS_PER_INCH)){
		throw J_Init_Error("Could Not Set Character Size Properly!");
	}
	
	int num_glyphs = i_face->num_glyphs;

	M_bitmap_metrics.reserve(num_glyphs);

	M_bitmap_datas.resize(num_glyphs);
	//used to prevent excessive allocations
	j_bitmap<j_ubyte, 1> expanded_bitmap;
	
	for(int i=0; i < i_face->num_glyphs; i++){
		FT_GlyphSlot glyph;
		if('\t' == i){
			glyph = get_rendered_glyph(i_face, ' ', i_render_mode);
			text_bitmap_formatter(&expanded_bitmap, glyph->bitmap.width, glyph->bitmap.rows
			, glyph->bitmap.buffer);
			expanded_bitmap.resize(expanded_bitmap.width()*M_tab_distance,expanded_bitmap.height());
			FT_Bitmap& bitmap = glyph->bitmap;
			M_bitmap_metrics.push_back(Bitmap_Metrics(expanded_bitmap.width(), expanded_bitmap.height()
				, glyph->bitmap_left, bitmap.rows - glyph->bitmap_top, glyph->advance.x*M_tab_distance/FT_POINT_TO_PARAMETER_CONVERSION, glyph->advance.y/FT_POINT_TO_PARAMETER_CONVERSION));

		} else if('\n' == i){
			glyph = get_rendered_glyph(i_face, ' ', i_render_mode);
			text_bitmap_formatter(&expanded_bitmap, glyph->bitmap.width, 0
								  , glyph->bitmap.buffer);
			expanded_bitmap.resize(expanded_bitmap.width()*M_tab_distance, expanded_bitmap.height());
			FT_Bitmap& bitmap = glyph->bitmap;
			M_bitmap_metrics.push_back(Bitmap_Metrics(expanded_bitmap.width(), 0
				, glyph->bitmap_left, bitmap.rows - glyph->bitmap_top, 0, glyph->advance.y / FT_POINT_TO_PARAMETER_CONVERSION));

		} else{
			glyph = get_rendered_glyph(i_face, i, i_render_mode);
			text_bitmap_formatter(&expanded_bitmap, glyph->bitmap.width, glyph->bitmap.rows
			, glyph->bitmap.buffer);
			FT_Bitmap& bitmap = glyph->bitmap;
			M_bitmap_metrics.push_back(Bitmap_Metrics(expanded_bitmap.width(), expanded_bitmap.height()
				, glyph->bitmap_left, bitmap.rows - glyph->bitmap_top, glyph->advance.x/FT_POINT_TO_PARAMETER_CONVERSION
				, glyph->advance.y/FT_POINT_TO_PARAMETER_CONVERSION));

		}
		M_bitmap_datas[i] = expanded_bitmap.get_array();
		
		if(!(i%500)){
			cerr << "\nGlyph: " << i << " created!";
		}

	}
}

FT_GlyphSlot get_rendered_glyph(FT_Face i_face, j_ulint i_charcode, FT_Int32 i_load_flag){

	if(FT_Load_Char(i_face, i_charcode, i_load_flag)){
		throw J_FT_Char_Load_Error("Could Not Render Character", i_charcode, i_load_flag);
	}

	return i_face->glyph;

}

Bitmap_Metrics& J_Font_Face__::bitmap_metric(j_size_t i_index){
	return M_bitmap_metrics[i_index];
}

const Bitmap_Metrics& J_Font_Face__::bitmap_metric(j_size_t i_index)const{
	return M_bitmap_metrics[i_index];
}

const j_ubyte* J_Font_Face__::get_data(j_size_t i_index){
	return M_bitmap_datas[i_index].data();
}

j_size_t J_Font_Face__::num_glyphs()const{
	return M_bitmap_datas.size();
}

int J_Font_Face__::new_line_size()const{
	return M_new_line_size;
}

}



