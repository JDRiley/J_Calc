#include "J_Image_Pane.h"
//
#include <J_Open_GL.h>
//
#include <J_Utile.h>
//
#include <J_Shader_Program.h>
namespace jomike{

static J_Open_GL s_open_gl;
static int image_shader_id(){
	static Image_Shader_Program s_image_shader(Image_Format::RGBA32_UBYTE);
	return s_image_shader.program_id();
}
J_Image_Pane::J_Image_Pane(const J_Rectangle& ik_rec
	, GL_Pixel_Formats i_format, int i_image_width, int i_image_height)
	:J_UI_Box(ik_rec), M_image_width(i_image_width)
	, M_image_height(i_image_height){

	assert(image_height() >= 0);
	assert(image_width() >= 0);
	M_format = i_format;

	set_buffer(nullptr);
}

void J_Image_Pane::set_buffer(const j_ubyte* i_buffer){
	s_open_gl.use_program(image_shader_id());

	s_open_gl.bind_texture_2D(M_texture);
	if(image_width() && image_height()){
		s_open_gl.tex_sub_image_2D_ubyte(
			Texture_Target::TEXTURE_2D, 0, 0, 0, image_width(), image_height()
			, M_format, i_buffer);
	}

	s_open_gl.debind_texture(Texture_Target::TEXTURE_2D);
	s_open_gl.debind_program();
}

void J_Image_Pane::set_image_width(int i_image_width){
	assert(i_image_width >= 0);
	if(image_width() == i_image_width){
		return;
	}
	M_image_width = i_image_width;

	s_open_gl.use_program(image_shader_id());
	
	M_texture = J_GL_Texture();

	s_open_gl.bind_texture_2D(M_texture);
	s_open_gl.tex_storage_2D(
		Texture_Target::TEXTURE_2D, 1, M_internal_format, image_width()
		, image_height());
	set_texture_clamp_parameters();

	s_open_gl.debind_program();
	clear_image();
}

void J_Image_Pane::set_image_height(int i_image_height){
	assert(i_image_height >= 0);
	if(image_height() == i_image_height){
		return;
	}
	M_image_height = i_image_height;
	s_open_gl.use_program(image_shader_id());
	M_texture = J_GL_Texture();

	s_open_gl.bind_texture_2D(M_texture);
	s_open_gl.tex_storage_2D(
		Texture_Target::TEXTURE_2D, 1, M_internal_format, image_width(), image_height());
	set_texture_clamp_parameters();
	
	s_open_gl.debind_program();
	clear_image();
	
}


void J_Image_Pane::set_texture_clamp_parameters(){
	s_open_gl.texture_wrap_s(Texture_Target::TEXTURE_2D, Texture_Wrap_Type::CLAMP_TO_BORDER);

	s_open_gl.texture_wrap_t(Texture_Target::TEXTURE_2D, Texture_Wrap_Type::CLAMP_TO_BORDER);

	s_open_gl.texture_wrap_r(Texture_Target::TEXTURE_2D, Texture_Wrap_Type::CLAMP_TO_BORDER);
}



int J_Image_Pane::image_width()const{return M_image_width;}

int J_Image_Pane::image_height()const{return M_image_height;}

void J_Image_Pane::set_input_format(GL_Pixel_Formats i_format){
	M_format = i_format;
}

GL_Pixel_Formats J_Image_Pane::input_format()const{
	return M_format;
}

void J_Image_Pane::clear_image(){
	ex_array<j_ubyte> zeros(image_width()*image_height()*num_channels(), static_cast<j_ubyte>(0));

	s_open_gl.bind_texture_2D(M_texture);
	if(image_height() && image_width()){
		s_open_gl.tex_sub_image_2D_ubyte(
			Texture_Target::TEXTURE_2D, 0, 0, 0, image_width(), image_height(), M_format, zeros.data());
	}
	s_open_gl.debind_texture(Texture_Target::TEXTURE_2D);
}

void J_Image_Pane::draw()const{
	J_UI_Box::draw();
	
	s_open_gl.use_program(image_shader_id());
	s_open_gl.bind_vertex_array(get_box_vao());
	

	s_open_gl.bind_texture_2D(M_texture);
	s_open_gl.draw_arrays(Array_Draw_Mode::TRIANGLE_FAN, 0, 4);

	s_open_gl.debind_vertex_array();
	s_open_gl.debind_texture(Texture_Target::TEXTURE_2D);
	s_open_gl.debind_program();
}


int J_Image_Pane::num_channels(){
	switch(M_internal_format)	{
	case GL_Sized_Internal_Formats::R8:
		return 1;
	case GL_Sized_Internal_Formats::R8_SNORM:
		break;
	case GL_Sized_Internal_Formats::R16:
		return 1;
	case GL_Sized_Internal_Formats::R16_SNORM:
		break;
	case GL_Sized_Internal_Formats::RG8:
		break;
	case GL_Sized_Internal_Formats::RG8_SNORM:
		break;
	case GL_Sized_Internal_Formats::RG16:
		return 1;
	case GL_Sized_Internal_Formats::RG16_SNORM:
		break;
	case GL_Sized_Internal_Formats::R3_G3_B2:
		break;
	case GL_Sized_Internal_Formats::RGB4:
		break;
	case GL_Sized_Internal_Formats::RGB5:
		break;
	case GL_Sized_Internal_Formats::RGB8:
		break;
	case GL_Sized_Internal_Formats::RGB8_SNORM:
		break;
	case GL_Sized_Internal_Formats::RGB10:
		break;
	case GL_Sized_Internal_Formats::RGB12:
		break;
	case GL_Sized_Internal_Formats::RGB16_SNORM:
		break;
	case GL_Sized_Internal_Formats::RGBA2:
		break;
	case GL_Sized_Internal_Formats::RGBA4:
		break;
	case GL_Sized_Internal_Formats::RGB5_A1:
		break;
	case GL_Sized_Internal_Formats::RGBA8:
		return 4;
	case GL_Sized_Internal_Formats::RGBA8_SNORM:
		break;
	case GL_Sized_Internal_Formats::RGB10_A2:
		break;
	case GL_Sized_Internal_Formats::RGB10_A2UI:
		break;
	case GL_Sized_Internal_Formats::RGBA12:
		break;
	case GL_Sized_Internal_Formats::RGBA16:
		break;
	case GL_Sized_Internal_Formats::SRGB8:
		break;
	case GL_Sized_Internal_Formats::SRGB8_ALPHA8:
		break;
	case GL_Sized_Internal_Formats::R16F:
		break;
	case GL_Sized_Internal_Formats::RG16F:
		break;
	case GL_Sized_Internal_Formats::RGB16F:
		break;
	case GL_Sized_Internal_Formats::RGBA16F:
		break;
	case GL_Sized_Internal_Formats::R32F:
		break;
	case GL_Sized_Internal_Formats::RG32F:
		break;
	case GL_Sized_Internal_Formats::RGB32F:
		break;
	case GL_Sized_Internal_Formats::RGBA32F:
		break;
	case GL_Sized_Internal_Formats::R11F_G11F_B10F:
		break;
	case GL_Sized_Internal_Formats::RGB9_E5:
		break;
	case GL_Sized_Internal_Formats::R8I:
		break;
	case GL_Sized_Internal_Formats::R8UI:
		break;
	case GL_Sized_Internal_Formats::R16I:
		break;
	case GL_Sized_Internal_Formats::R16UI:
		break;
	case GL_Sized_Internal_Formats::R32I:
		break;
	case GL_Sized_Internal_Formats::R32UI:
		break;
	case GL_Sized_Internal_Formats::RG8I:
		break;
	case GL_Sized_Internal_Formats::RG8UI:
		break;
	case GL_Sized_Internal_Formats::RG16I:
		break;
	case GL_Sized_Internal_Formats::RG16UI:
		break;
	case GL_Sized_Internal_Formats::RG32I:
		break;
	case GL_Sized_Internal_Formats::RG32UI:
		break;
	case GL_Sized_Internal_Formats::RGB8I:
		break;
	case GL_Sized_Internal_Formats::RGB8UI:
		break;
	case GL_Sized_Internal_Formats::RGB16I:
		break;
	case GL_Sized_Internal_Formats::RGB16UI:
		break;
	case GL_Sized_Internal_Formats::RGB32I:
		break;
	case GL_Sized_Internal_Formats::RGB32UI:
		break;
	case GL_Sized_Internal_Formats::RGBA8I:
		break;
	case GL_Sized_Internal_Formats::RGBA8UI:
		break;
	case GL_Sized_Internal_Formats::RGBA16I:
		break;
	case GL_Sized_Internal_Formats::RGBA16UI:
		break;
	case GL_Sized_Internal_Formats::RGBA32I:
		break;
	case GL_Sized_Internal_Formats::RGBA32UI:
		break;
	default:
		break;
	}
	assert(!"Unhandled Sized Internal Format");
	return -1;
}



}