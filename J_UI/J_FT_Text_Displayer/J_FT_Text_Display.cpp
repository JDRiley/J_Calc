#include "J_FT_Text_Display.h"
//
#include "../J_Image/J_Image.h"
//
#include <J_OpenGL.h>
//
#include <J_Shader_Program.h>
//
#include <J_Utile.h>
//
#include "../J_UI_Controller.h"
//
#include "../j_cursor_management.h"

//
#include "../J_Font_Manager.h"
//
#include "J_UI_Letter_box.h" //Implement this here. something that'll hold each letter
//
#include <J_Open_GL.h>
//Algorithms
#include <algorithm>
#include <functional>
#include <numeric>
using std::bind; using std::copy; using std::equal_to; using std::for_each; using std::is_sorted;
using std::max; using std::min; using std::none_of; using std::accumulate;
using std::lower_bound;
using namespace std::placeholders;using std::max_element;

//Containers
#include <ex_array.h>

using std::array; using std::string;

//IO Facilities

#include <thread>
using std::thread;

#include <iostream>
using std::cerr; using std::endl;

//Libraries


//Utilities
#include <cassert>
//
#include "../J_Display_Object/J_Display_Line.h"

using std::pair;

using std::function;


using std::lock_guard;


namespace jomike{
static Instance_Pointer<Contexts_Handler> s_contexts;
static J_Open_GL s_open_gl;

class Modifier_Manger{
	typedef ex_array<J_UI_Letter_Box_Shared_t> Arr_t;
	Arr_t& operator*(){
		M_notification_function();
		return M_letter_box_arr;
	}

	Arr_t* operator->(){
		M_notification_function();
		return &M_letter_box_arr;
	}
private:
	Arr_t M_letter_box_arr;
	function<void()> M_notification_function;
};

const char* const DEFAULT_FONT_PATH_NAME 
	= "J:\\JoMike Library\\J_FT_Text_Display\\Fonts\\times.ttf";


class J_Text_Shader_Program{
public:
	J_Text_Shader_Program();
	int program_id(){return M_program_id;};
	int cursor_program_id(){return M_cursor_program_id;}
private:
	
	int M_program_id;
	j_uint M_cursor_program_id;
};

J_Text_Shader_Program::J_Text_Shader_Program(){
	M_program_id = s_open_gl.create_program();
	
	j_uint vert_shader_id = load_vertex_shader("Shaders/quad.vert");
	j_uint frag_shader_id = load_fragment_shader("Shaders/quad_texture.frag");

	s_open_gl.attach_shader(M_program_id, vert_shader_id);
	s_open_gl.attach_shader(M_program_id, frag_shader_id);
	s_open_gl.link_program(M_program_id);

	enforce_program_status(M_program_id, GL_Statuses::LINK_STATUS);

	M_cursor_program_id = s_open_gl.create_program();
	j_uint outline_shader_id = load_vertex_shader("Shaders/quad.vert");
	j_uint outline_frag_shader_id = load_fragment_shader("Shaders/box_outline.frag");

	s_open_gl.attach_shader(M_cursor_program_id, outline_shader_id);
	s_open_gl.attach_shader(M_cursor_program_id, outline_frag_shader_id);
	s_open_gl.link_program(M_cursor_program_id);

	//cerr << "\n Shader Program ID: " << M_program_id << "*********************************************************";
}

J_FT_Text_Display_Object::J_FT_Text_Display_Object(j_uint i_obj_id)
	:J_Display_Box(i_obj_id){}

J_FT_Text_Display_Object::J_FT_Text_Display_Object(j_uint i_object_id, j_uint i_id)
	:J_Display_Box(i_object_id, i_id){}

J_FT_Text_Display_Object::~J_FT_Text_Display_Object(){
	
}


J_FT_Text_Display::J_FT_Text_Display(j_uint i_obj_id) : J_FT_Text_Display_Object(i_obj_id)
	, M_letter_box_string(bind(&J_FT_Text_Display::alert_changed, this)){
	initialize_frame_buffer();


}



J_FT_Text_Display::J_FT_Text_Display(j_uint i_object_id, j_uint i_id)
	:J_FT_Text_Display_Object(i_object_id, i_id)
	, M_letter_box_string(bind(&J_FT_Text_Display::alert_changed, this)){
	M_letter_box_string->set_front_buffer(0.0);

	initialize_frame_buffer();


}










void J_FT_Text_Display::clear(){
	clear_from(0);
}

void J_FT_Text_Display::clear_from(j_size_t i_pos){
	assert(i_pos >= 0);
	M_letter_box_string->resize(i_pos);
}


/*void J_FT_Text_Display::draw()const*/
void J_FT_Text_Display::draw()const{
	J_Display_Box::draw();
	assert(!open_gl_error());
	if(M_changed_flag){
		render_frame_buffer();
	}

	assert(M_framebuffer.valid());

	s_open_gl.bind_draw_framebuffer(J_GL_Framebuffer::null_object());



	//j_uint dest_x1 = x_uns_pixel(window, x1());
	//j_uint dest_x2 = x_uns_pixel(window, x2());

	//j_uint dest_y1 = y_uns_pixel(window, y1());
	//j_uint dest_y2 = y_uns_pixel(window, y2());
	//glBlitFramebuffer(
	//	0, 0, x_pixels(), y_pixels(), dest_x1
	//	, dest_y1, dest_x2, dest_y2	
	//	, GL_COLOR_BUFFER_BIT, GL_NEAREST);


	s_open_gl.use_program(image_shader_program_id());

	s_open_gl.bind_texture_2D(M_texture_render_buffer);

	s_open_gl.bind_vertex_array(s_contexts->screen_box_vao());

	s_open_gl.bind_texture_2D(M_texture_render_buffer);


	s_open_gl.draw_arrays(Array_Draw_Mode::TRIANGLE_FAN, 0, 4);
	assert(!open_gl_error());
	s_open_gl.bind_buffer(GL_Buffer_Targets::ARRAY_BUFFER, J_GL_Buffer::null_object());
	assert(!open_gl_error());

	s_open_gl.bind_texture_2D(J_GL_Texture::null_object());
	
	s_open_gl.use_program(0);
	assert(!open_gl_error());

}

void J_FT_Text_Display::render_frame_buffer()const{
	j_uint prev_width = get_x_res(s_contexts->get_active_window());
	j_uint prev_height = get_y_res(s_contexts->get_active_window());;
	
	
	s_open_gl.bind_draw_framebuffer(M_framebuffer);
	
	//
	//j_uint x_size = x_pixels();
	//j_uint y_size = y_pixels();
	//const j_uint A_TOO_LARGE_NUMBER_OF_PIXELS = 1234222;
	//assert(y_size < A_TOO_LARGE_NUMBER_OF_PIXELS);
	//assert(x_size < A_TOO_LARGE_NUMBER_OF_PIXELS);

	s_open_gl.viewport(0, 0, prev_width, prev_height);
	s_open_gl.set_clear_color(0.0f, 0.0f, 0.0f, 0.0f);
	j_clear();

	
	auto start_view_pos
		= lower_bound(M_letter_box_string->begin(), M_letter_box_string->end()
		, J_Rectangle_Shared_t(new J_Rectangle(0.0f, y1(), 0.0f, 0.0f))
		, [](J_Rectangle_Shared_t i_left, J_Rectangle_Shared_t i_right){
		j_float compare_val = i_left->y1() - i_right->y1();
		//hot fix
		if(compare_val > 0.01f){
			return true;
		} else if(compare_val < -0.01f){
			return false;
		} else{
			return i_left->x1() < i_left->x2();
		}
	});

	auto end_view_pos 
		= lower_bound(M_letter_box_string->begin(), M_letter_box_string->end()
		, J_Rectangle_Shared_t(new J_Rectangle(0.0f, y2(), 0.0f, 0.0f))
		, [](J_Rectangle_Shared_t i_left, J_Rectangle_Shared_t i_right){
		j_float compare_val = i_left->y2() - i_right->y2();
		//hot fix
		if(compare_val > 0.01f){
			return true;
		} else if(compare_val < -0.01f){
			return false;
		} else{
			return i_left->x1() < i_left->x2();
		}
	});
	



	while(start_view_pos != end_view_pos){
		(*start_view_pos++)->draw();
	}
	
	s_open_gl.bind_draw_framebuffer(J_GL_Framebuffer::null_object()); 

	s_open_gl.viewport(0, 0, prev_width, prev_height);
	M_changed_flag = false;
	assert(!open_gl_error());
}



void J_FT_Text_Display::delete_char(j_size_t i_pos){
	M_letter_box_string->erase(M_letter_box_string->begin() + i_pos);
}

void J_FT_Text_Display::erase_chars(j_size_t i_pos, j_size_t i_size){
	assert(i_pos >= 0);
	assert(i_size >= 0);
	assert( i_pos < M_letter_box_string->size());
	assert(i_pos + i_size <= M_letter_box_string->size());
	
	M_letter_box_string->erase(i_pos, i_size);
}



void J_FT_Text_Display::pop_back(){

	assert(!"The method or operation is not yet check.");
}

void J_FT_Text_Display::add_letter_box(j_size_t i_index
									   , const Pen_Pos_FL_t& i_pen_pos
									   , const Bitmap_Metrics& i_metrics
									   , const J_UI_Color& i_color
									   , const j_ubyte* i_bitmap){
	
	J_Display_Letter_Box_Shared_t new_letter_box(new J_UI_Letter_Box(0));
	new_letter_box->set_image_box(i_pen_pos, i_metrics);
	new_letter_box->set_buffer_data(i_metrics, i_color, i_bitmap);

	M_letter_box_string->insert(M_letter_box_string->begin() + i_index, new_letter_box);



}

void J_FT_Text_Display::set_letter_box_rectangle(j_size_t i_pos, const Pen_Pos_FL_t& i_pen_pos
							  , const Bitmap_Metrics& i_metrics){
	M_letter_box_string[i_pos]->set_image_box(i_pen_pos, i_metrics);
}

void J_FT_Text_Display::set_letter_box_data(j_size_t i_index
											, const Bitmap_Metrics& i_metrics
											, const J_UI_Color& i_color
											, const j_ubyte* i_data){
	M_letter_box_string[i_index]->set_buffer_data(i_metrics, i_color, i_data);
}

void J_FT_Text_Display::set_text_string_size(j_size_t i_size){
	j_size_t initial_string_size = M_letter_box_string->size();
	M_letter_box_string->resize(i_size);
	for(j_size_t i = initial_string_size; i < i_size; i++){
		M_letter_box_string[i] = (J_UI_Letter_Box_Shared_t(new J_UI_Letter_Box));
	}
}

void J_FT_Text_Display::set_letter_box_poses(j_size_t i_index
											 , j_size_t i_size
											 , const Pen_Pos_FL_t* i_poses){
	
	auto letter_pos_start = M_letter_box_string->begin() + i_index;

	for(int i = 0; i < i_size; i++){
		letter_pos_start[i]->set_pos(i_poses[i]);
	}
}

void J_FT_Text_Display::insert_text_string(j_size_t i_pos, j_size_t i_size
										   , const Pen_Pos_FL_t* i_poses
										   , Bitmap_Metrics** i_metrics
										   , const J_UI_Color& i_color
										   , const j_ubyte* const * i_datas){
	static ex_array<J_UI_Letter_Box_Shared_t> utility_cont;
	utility_cont.resize(0);
	utility_cont.reserve(i_size);
	for(int i = 0; i < i_size; i++){
		J_UI_Letter_Box_Shared_t letter_box(new J_UI_Letter_Box(0));
		const Bitmap_Metrics& metric = *(i_metrics[i]);
		letter_box->set_image_box(i_poses[i], metric);
		letter_box->set_buffer_data(metric, i_color, i_datas[i]);

		utility_cont.push_back(letter_box);
	}
	M_letter_box_string->insert(M_letter_box_string->begin() + i_pos
							   , utility_cont.begin(), utility_cont.end());


}



j_uint J_FT_Text_Display::x_pixels()const{
	return ::jtl::x_pixels(s_contexts->get_active_window(), width());
}

j_uint J_FT_Text_Display::y_pixels()const{
	return ::jtl::y_pixels(s_contexts->get_active_window(), height());
}

void J_FT_Text_Display::alert_changed()const{
	M_changed_flag = true;
}

void J_FT_Text_Display::initialize_frame_buffer(){
	s_open_gl.bind_framebuffer(M_framebuffer);

	s_open_gl.bind_texture_2D(M_texture_render_buffer);
	auto window = s_contexts->get_active_window();
	j_uint width = get_x_res(window);
	j_uint height = get_y_res(window);

	s_open_gl.tex_storage_2D(
		Texture_Target::TEXTURE_2D, 1
		, GL_Sized_Internal_Formats::RGB8, width, height);

	{
		const ex_array<j_ubyte> image_data(4 * width*height, 0);

		s_open_gl.tex_sub_image_2D_ubyte(
			Texture_Target::TEXTURE_2D, 0, 0, 0, width, height
			, GL_Pixel_Formats::RGBA, image_data.data());
	}


	s_open_gl.attach_draw_framebuffer_texture_2D(
		GL_Attachment_Points::COLOR_ATTACHMENT0
		, Texture_Target::TEXTURE_2D, M_texture_render_buffer, 0);


	s_open_gl.bind_texture_2D(J_GL_Texture::null_object());
}



J_FT_Text_Multi_State_Display::J_FT_Text_Multi_State_Display(j_uint i_obj_id)
	: J_FT_Text_Display_Object(i_obj_id){}

void J_FT_Text_Multi_State_Display::delete_char(j_size_t i_pos){
	M_current_state->delete_char(i_pos);
}

void J_FT_Text_Multi_State_Display::erase_chars(j_size_t i_start, j_size_t i_end){
	M_current_state->erase_chars(i_start, i_end);
}


void J_FT_Text_Multi_State_Display::draw()const{
	M_current_state->draw();
}

void J_FT_Text_Multi_State_Display::alert_resize(int i_width, int i_height){
	M_current_state->alert_resize(i_width, i_height);
}

void J_FT_Text_Multi_State_Display::pop_back(){
	M_current_state->pop_back();
}


void J_FT_Text_Multi_State_Display::mouse_button_press(J_View_Shared_t i_view
	, int i_button, int i_modifiers, Pen_Pos_FL_t i_pen_pos){
	M_current_state->mouse_button_press(i_view, i_button, i_modifiers, i_pen_pos);
}

void J_FT_Text_Multi_State_Display::mouse_button_release(J_View_Shared_t i_view
	, int i_button, int i_modifiers, Pen_Pos_FL_t i_pen_pos){
	M_current_state->mouse_button_release(i_view, i_button, i_modifiers, i_pen_pos);
}


void J_FT_Text_Multi_State_Display::add_state(){
	M_text_states.push_back(
		J_FT_Text_Display_Shared_t(new J_FT_Text_Display(get_object_ID(), get_ID()))
		);

}

void J_FT_Text_Multi_State_Display::set_state(j_size_t i_index){
	assert(between_inclusive(i_index, J_SIZE_T_ZERO, M_text_states.size()));
	M_current_state = M_text_states[i_index];
}

void J_FT_Text_Multi_State_Display::set_box(const J_Rectangle& irk_rec){
	M_current_state->set_rectangle(irk_rec);
}

void J_FT_Text_Multi_State_Display::set_fill_color(const J_UI_Color& i_color){
	M_current_state->set_fill_color(i_color);
}

void J_FT_Text_Multi_State_Display::set_outline_color(const J_UI_Color& i_color){
	M_current_state->set_outline_color(i_color);
}

void J_FT_Text_Multi_State_Display::set_fill_visibility_status(bool i_status){
	M_current_state->set_fill_visibility_status(i_status);
}

void J_FT_Text_Multi_State_Display::set_outline_visibility_status(bool i_status){
	M_current_state->set_outline_visibility_status(i_status);
}

void J_FT_Text_Multi_State_Display::set_outline_thickness(j_float i_float){
	M_current_state->set_outline_thickness(i_float);
}




void J_FT_Text_Multi_State_Display::alert_cursor_pos(Pen_Pos_FL_t i_pos){
	M_current_state->alert_cursor_pos(i_pos);
}

void J_FT_Text_Multi_State_Display
		::add_letter_box(j_size_t i_index, const Pen_Pos_FL_t& i_pen_pos
						, const Bitmap_Metrics& i_metrics
						, const J_UI_Color& i_color, const j_ubyte* i_bitmap){
	M_current_state->add_letter_box(i_index, i_pen_pos
									, i_metrics, i_color, i_bitmap);
}


void J_FT_Text_Multi_State_Display::set_letter_box_rectangle(j_size_t i_pos
															 , const Pen_Pos_FL_t& i_pen_pos
															 , const Bitmap_Metrics& i_metrics){
	M_current_state->set_letter_box_rectangle(i_pos, i_pen_pos, i_metrics);
}

void J_FT_Text_Multi_State_Display::set_letter_box_data(j_size_t i_index, const Bitmap_Metrics& i_metrics, const J_UI_Color& i_color, const j_ubyte* i_data){
	M_current_state->set_letter_box_data(i_index, i_metrics, i_color, i_data);
}

void J_FT_Text_Multi_State_Display::set_text_string_size(j_size_t i_size){
	M_current_state->set_text_string_size(i_size);
}

void J_FT_Text_Multi_State_Display::set_letter_box_poses(j_size_t i_index, j_size_t i_size, const Pen_Pos_FL_t* i_poses){
	M_current_state->set_letter_box_poses(i_index, i_size, i_poses);
}

void J_FT_Text_Multi_State_Display::insert_text_string(j_size_t i_pos, j_size_t i_size, const Pen_Pos_FL_t* i_poses, Bitmap_Metrics** i_metrics, const J_UI_Color& i_color, const j_ubyte* const * i_datas){
	M_current_state->insert_text_string(i_pos, i_size, i_poses, i_metrics, i_color, i_datas);
}



}

}