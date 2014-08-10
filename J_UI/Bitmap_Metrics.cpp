#include "Bitmap_Metrics.h"


namespace jomike{

extern const int EXTRA_CHAR_DPI_FACTOR = 1;

Bitmap_Metrics::Bitmap_Metrics(int i_width, int i_height, int i_left
							   , int i_underreach, int i_advance_x, int i_advance_y)
				:width(i_width), height(i_height), buffer_width(width), buffer_height(height)
				, left(i_left), underreach(i_underreach)
				, advance_x(i_advance_x), advance_y(i_advance_y){

	width = width/EXTRA_CHAR_DPI_FACTOR;
	height = height/EXTRA_CHAR_DPI_FACTOR;

	//do not adjust buffer width;

	left = left/EXTRA_CHAR_DPI_FACTOR;
	underreach = underreach/EXTRA_CHAR_DPI_FACTOR;
	advance_x = advance_x/EXTRA_CHAR_DPI_FACTOR;
	advance_y = advance_y/EXTRA_CHAR_DPI_FACTOR;
	new_line_size = new_line_size/EXTRA_CHAR_DPI_FACTOR;
	

}

}



