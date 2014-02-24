#ifndef J_IMAGE_PANE_H
#define J_IMAGE_PANE_H

#include <J_Fwd_Decl.h>
//
#include "J_UI_Object.h"
//
#include <J_Image.h>
namespace jomike{


class J_Image_Pane : public J_UI_Box{
public:
	J_Image_Pane(const J_Rectangle&, Image_Format format = Image_Format::BGR24_UBYTE
		, int width = 0, int height = 0 );
	void set_buffer(const j_ubyte*);
	
	void set_image_width(int);
	void set_image_height(int);
	int image_width()const;
	int image_height()const;
	void set_middle_line_color(J_UI_Color);
	void set_input_format(Image_Format);
	Image_Format input_format()const;
	void broadcast_current_state()const override;
private:
	Image_Format M_format;
	int M_width;
	int M_height;
};




}

#endif