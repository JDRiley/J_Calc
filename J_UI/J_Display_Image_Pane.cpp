#include "J_Display_Image_Pane.h"
//
#include "../J_OpenGl/J_Shader_Program.h"
//
#include <J_OpenGl.h>
//
#include <ex_array.h>
//
#include <GL/glew.h>


using std::numeric_limits;
namespace jomike{


J_Display_Image_Pane::J_Display_Image_Pane(j_uint i_obj_id):J_Display_Box(i_obj_id){
	glGenTextures(1, &M_texture_id);
	
	M_width = M_height = 0;
	M_format = Image_Format::BGR24_UBYTE;
	M_image_program = new Image_Shader_Program(M_format);
}


void J_Display_Image_Pane::set_image_width(int i_width){
	glUseProgram(M_image_program->program_id());
	if(image_width() == i_width){
		return;
	}
	M_width = i_width;
	glDeleteTextures(1, &M_texture_id);
	glGenTextures(1, &M_texture_id);

	glBindTexture(GL_TEXTURE_2D, M_texture_id);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, image_width(), image_height());
	glUseProgram(0);
	clear_image();
}

void J_Display_Image_Pane::set_image_height(int i_height){
	glUseProgram(M_image_program->program_id());
	if(image_height() == i_height){
		return;
	}
	M_height = i_height;
	glDeleteTextures(1, &M_texture_id);
	glGenTextures(1, &M_texture_id);

	glBindTexture(GL_TEXTURE_2D, M_texture_id);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, image_width(), image_height());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glUseProgram(0);
	clear_image();
}

void J_Display_Image_Pane::set_buffer(const j_ubyte* i_buffer){
	glUseProgram(M_image_program->program_id());

	glBindTexture(GL_TEXTURE_2D, M_texture_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image_width(), image_height(), get_format_val()
	, GL_UNSIGNED_BYTE, i_buffer);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void J_Display_Image_Pane::set_buffer_mono(const j_ubyte* i_buffer){
	glUseProgram(M_image_program->program_id());

	glBindTexture(GL_TEXTURE_2D, M_texture_id);


	ex_array<j_ubyte> bitmap(320*240*3);

	const short* buffer = reinterpret_cast<const short*>(i_buffer);

	int num_pixels = 320*240;
	for(int i=0; i < num_pixels; i++){
		
		bitmap[3*i] = normalized_val<j_ubyte>(buffer[i]);
		bitmap[3*i + 1] = normalized_val<j_ubyte>(buffer[i]);
		bitmap[3*i +2] = normalized_val<j_ubyte>(buffer[i]);
	}

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 320, 240, GL_RGB
	, GL_UNSIGNED_BYTE, bitmap.data());

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}


void J_Display_Image_Pane::set_color_format(Image_Format i_format){
	if(M_format == i_format){
		return;
	}
	M_format = i_format;
	M_image_program->set_format(M_format);
}

void J_Display_Image_Pane::clear_image(){
	ex_array<j_ubyte> zeros(M_width*M_width*num_channels(), static_cast<j_ubyte>(0));
	glBindTexture(GL_TEXTURE_2D, M_texture_id);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, image_width(), image_height(), get_format_val()
		, GL_UNSIGNED_BYTE, zeros.data());
	glBindTexture(GL_TEXTURE_2D, 0);
}

j_uint J_Display_Image_Pane::get_format_val()const{
	j_uint format;
	switch(M_format){
	case Image_Format::RGB24_UBYTE:
		format = GL_RGB;
		break;
	case Image_Format::BGR24_UBYTE:
		format = GL_BGR;
		break;
	case Image_Format::RGBA32_UBYTE:
		format = GL_RGBA;
		break;
	default:
		format = 0;
		assert(!"Unrecognized Image Format. Internal Error");
	}
	return format;
}

J_Display_Image_Pane::~J_Display_Image_Pane(){
	delete M_image_program;
	glDeleteTextures(1, &M_texture_id);
}

void J_Display_Image_Pane::draw()const{
	
	assert(glIsProgram(M_image_program->program_id()));
	glBindVertexArray(get_box_vao());
	glUseProgram(M_image_program->program_id());

	glBindTexture(GL_TEXTURE_2D, M_texture_id);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	J_Display_Box::draw();
}

int J_Display_Image_Pane::image_height()const{return M_height;}
int J_Display_Image_Pane::image_width()const{return M_width;}

int J_Display_Image_Pane::num_channels()const{
	return j_num_channels(M_format);
}

void J_Display_Image_Pane::set_image_format(Image_Format i_format){
	if(M_format == i_format){
		return;
	}
	
	M_format = i_format;

}

void J_Display_Image_Pane::set_middle_line_color(J_UI_Color i_color){
	M_image_program->set_middle_line_color(i_color);
}

}