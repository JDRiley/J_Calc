#ifndef BITMAP_METRICS_H
#define BITMAP_METRICS_H

#include "J_UI_Fwd_Decl.h"


namespace jomike{
	
extern const int EXTRA_CHAR_DPI_FACTOR;

struct Bitmap_Metrics{
	int width;
	int height;
	int buffer_width;
	int buffer_height;
	int left;
	int underreach;
	int advance_x;
	int advance_y;
	int new_line_size;
	Bitmap_Metrics(int width, int height, int left, int underreach
					, int advance_x, int advance_y);
};


}

#endif //BITMAP_METRICS_H


