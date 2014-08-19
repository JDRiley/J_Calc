#include "J_UI_Letter_Box.h"
//
#include <J_Frame.h>
//
#include "../Bitmap_Metrics.h"
//
#include <J_Shader_Program.h>


namespace jomike{


class J_UI_Letter_Box_Shader : public J_GL_Box_Shader{
public:
	static J_GL_Box_Shader* make_new();
private:
	J_UI_Letter_Box_Shader() = delete;
};

J_GL_Box_Shader* J_UI_Letter_Box_Shader::make_new(){
	static J_GL_Box_Shader box_shader;
	return new J_GL_Box_Shader(box_shader.box_program_id(), box_shader.outline_program_id());
}


J_UI_Letter_Box
	::J_UI_Letter_Box(const J_Rectangle& irk_rectangle)
	:J_Image_Pane(irk_rectangle, GL_Pixel_Formats::RGBA, J_UI_Letter_Box_Shader::make_new())
	, M_metrics(0,0,0,0,0,0){

}

void J_UI_Letter_Box::set_image_box(const Pen_Pos_FL_t &i_pos, const Bitmap_Metrics& i_metrics
									, const J_Frame& i_frame){
	M_metrics = i_metrics;

	J_Rectangle rectangle;

	rectangle.set_x(i_pos.first + i_frame.to_x_screen(i_metrics.left));
	rectangle.set_y(i_pos.second - i_frame.to_y_screen(i_metrics.underreach));
	rectangle.set_width(i_frame.to_x_screen(i_metrics.width));
	rectangle.set_height(i_frame.to_y_screen(i_metrics.height));
	set_rectangle(rectangle);
}

void J_UI_Letter_Box::set_buffer_data(
	int i_width, int i_height, const J_UI_Color& i_color, const j_ubyte* i_bitmap){
	
	set_image_width(i_width);
	set_image_height(i_height);


	const j_ubyte* color_data = convert_to_rgba32(i_width, i_height, i_color, i_bitmap);

	set_buffer(color_data);


}

void J_UI_Letter_Box::set_buffer_data(
	const Bitmap_Metrics& i_metrics, const J_UI_Color& i_color, const j_ubyte* i_data){
	set_buffer_data(i_metrics.buffer_width, i_metrics.buffer_height, i_color, i_data);
}

J_UI_Letter_Box::~J_UI_Letter_Box(){

}




}



