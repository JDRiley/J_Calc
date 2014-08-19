#ifndef J_IMAGE_PANE_H
#define J_IMAGE_PANE_H

#include <J_Fwd_Decl.h>
//
#include "J_UI_Box.h"
//
#include <J_Image.h>
//
#include <OpenGL_Enums.h>
namespace jomike{


class J_Image_Pane : public J_UI_Box{
public:
	J_Image_Pane(const J_Rectangle&, GL_Pixel_Formats format, int width = 0, int height = 0 );

	J_Image_Pane(const J_Rectangle& irk_rectangle, GL_Pixel_Formats i_format
				 , J_GL_Box_Shader* i_program, int i_width = 0, int i_height = 0);

	J_Image_Pane(const J_Image_Pane&) = delete;
	J_Image_Pane(J_Image_Pane&&) = delete;
	J_Image_Pane& operator=(const J_Image_Pane&) = delete;
	J_Image_Pane& operator=(J_Image_Pane&&) = delete;
	


	void set_buffer(const j_ubyte*);
	void draw()const;
	void set_image_width(int);
	void set_image_height(int);
	int image_width()const;
	int image_height()const;
	void set_middle_line_color(J_UI_Color);
	void set_input_format(GL_Pixel_Formats);
	GL_Pixel_Formats input_format()const;
	void clear_image();
	~J_Image_Pane();
private:
	GL_Pixel_Formats M_format;
	GL_Sized_Internal_Formats M_internal_format = GL_Sized_Internal_Formats::RGBA8;
	int M_image_width;
	int M_image_height;

	J_GL_Texture M_texture;
	
	

	//Requires texture is already bound
	void set_texture_clamp_parameters();
	int num_channels();
};




}

#endif