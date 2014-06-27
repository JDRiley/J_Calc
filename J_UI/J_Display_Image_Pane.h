#ifndef J_DISPLAY_IMAGE_PANE_H
#define J_DISPLAY_IMAGE_PANE_H

#include <J_Fwd_Decl.h>
//
#include "J_Display_Object/J_Display_Object.h"
//
#include <J_Image.h>

#error deprecated
namespace jomike{


class J_Display_Image_Pane : public J_Display_Box{
public:
	J_Display_Image_Pane(j_uint i_obj_id);
	~J_Display_Image_Pane();
	void set_buffer(const j_ubyte*);
	void set_buffer_mono(const j_ubyte*);
	void clear_image();
	void draw()const override;
	void set_image_width(int);
	void set_image_height(int);
	void set_image_format(Image_Format);
	int image_height()const;
	int image_width()const;
	void set_color_format(Image_Format);
	int num_channels()const;
	void set_middle_line_color(J_UI_Color);
private:
	Image_Shader_Program* M_image_program;
	j_uint M_texture_id;
	Image_Format M_format;
	j_uint get_format_val()const;
	int M_height;
	int M_width;
};




}

#endif