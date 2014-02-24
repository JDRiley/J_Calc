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
#include "J_Display_Letter_box.h" //Implement this here. something that'll hold each letter

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

//
#include <gl\glew.h>

//Utilities
#include <cassert>
#include "../J_Display_Object/J_Display_Line.h"
using std::mutex;
using std::pair;




using std::lock_guard;


namespace jomike{
static Instance_Pointer<Contexts_Handler> s_contexts;



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
	M_program_id = glCreateProgram();
	
	j_uint vert_shader_id = load_vertex_shader("Shaders/quad.vert");
	j_uint frag_shader_id = load_fragment_shader("Shaders/quad_texture.frag");

	glAttachShader(M_program_id, vert_shader_id);
	glAttachShader(M_program_id, frag_shader_id);
	glLinkProgram(M_program_id);

	enforce_program_status(M_program_id, GL_LINK_STATUS);

	M_cursor_program_id = glCreateProgram();
	j_uint outline_shader_id = load_vertex_shader("Shaders/quad.vert");
	j_uint outline_frag_shader_id = load_fragment_shader("Shaders/box_outline.frag");

	glAttachShader(M_cursor_program_id, outline_shader_id);
	glAttachShader(M_cursor_program_id, outline_frag_shader_id);
	glLinkProgram(M_cursor_program_id);

	//cerr << "\n Shader Program ID: " << M_program_id << "*********************************************************";
}

J_FT_Text_Display_Object::J_FT_Text_Display_Object(j_uint i_obj_id)
	:J_Display_Box(i_obj_id){}

J_FT_Text_Display_Object::J_FT_Text_Display_Object(j_uint i_object_id, j_uint i_id)
	:J_Display_Box(i_object_id, i_id){}

J_FT_Text_Display_Object::~J_FT_Text_Display_Object(){}


J_FT_Text_Display::J_FT_Text_Display(j_uint i_obj_id) : J_FT_Text_Display_Object(i_obj_id){
	//M_new_line_size = 30;
	//M_shader = new J_Text_Shader_Program;
	//M_program_id = M_shader->program_id();
}

J_FT_Text_Display::J_FT_Text_Display(j_uint i_object_id, j_uint i_id)
	:J_FT_Text_Display_Object(i_object_id, i_id){
	M_letter_box_string.set_front_buffer(0.0);
}










void J_FT_Text_Display::clear(){
	clear_from(0);
}

void J_FT_Text_Display::clear_from(j_size_t i_pos){
	assert(i_pos >= 0);
	M_letter_box_string.resize(i_pos);
}


/*void J_FT_Text_Display::draw()const*/
void J_FT_Text_Display::draw()const{
	J_Display_Box::draw();


	
	auto start_view_pos
		= lower_bound(M_letter_box_string.begin(), M_letter_box_string.end()
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
		= lower_bound(M_letter_box_string.begin(), M_letter_box_string.end()
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

}



void J_FT_Text_Display::delete_char(j_size_t i_pos){
	M_letter_box_string.erase(M_letter_box_string.begin() + i_pos);
}

void J_FT_Text_Display::erase_chars(j_size_t i_pos, j_size_t i_size){
	assert(i_pos >= 0);
	assert(i_size >= 0);
	assert( i_pos < M_letter_box_string.size());
	assert(i_pos + i_size <= M_letter_box_string.size());
	
	M_letter_box_string.erase(i_pos, i_size);
}




//
//void J_FT_Text_Display::mouse_button_press(J_View_Shared_t i_view, int i_button, int , Pen_Pos_FL_t i_pos){
//	
//	switch(i_button){
//	case J_LEFT_MOUSE_BUTTON:{
//		M_selection_boxes.clear();
//		M_selection_start_cursor_pos = get_cursor_index(i_pos);
//		M_left_mouse_button_pressed_status = true;
//		set_cursor_pos(M_selection_start_cursor_pos);
//		J_UI_Controller::get_instance().notify_text_box_press(i_view, get_object_ID()
//															, M_cursor_pos);
//}
//		break;
//	case J_MOUSE_WHEEL_UP:
//		scroll(lines_scrolled_per_tick());
//		break;
//	case J_MOUSE_WHEEL_DOWN:
//		scroll(-lines_scrolled_per_tick());
//		break;
//	default:
//		;
//	}
//}
//
//void J_FT_Text_Display::mouse_button_release(J_View_Shared_t, int i_button, int , Pen_Pos_FL_t i_pos){
//
//	switch(i_button){
//	case J_LEFT_MOUSE_BUTTON:
//		M_left_mouse_button_pressed_status = false;
//		set_cursor_pos(get_cursor_index(i_pos));
//		J_UI_Controller::get_instance().notify_text_box_release(get_object_ID()
//			, get_cursor_index(i_pos));
//		break;
//	default:
//		;
//	}
//}



void J_FT_Text_Display::pop_back(){

	assert(!"The method or operation is not yet check.");
}


//
//
//bool J_FT_Text_Display::cursor_visibility_status()const{
//	return M_cursor_visibility_status;
//}
//
//void J_FT_Text_Display::set_cursor_visibility_status(bool i_status){
//	M_cursor_visibility_status = i_status;
//}
//
//
//
//void J_FT_Text_Display::move_cursor_line_pos_up(j_size_t i_move_val){
//	auto new_pen_pos = M_last_set_cursor_pos;
//	new_pen_pos.second += i_move_val*new_line_screen_size();
//	set_cursor_pos(get_cursor_index(new_pen_pos));
//	M_last_set_cursor_pos = M_pen_poses[M_cursor_pos];
//	M_last_set_cursor_pos.first = new_pen_pos.first;
//
//	J_UI_Controller::get_instance().notify_text_box_press(J_View_Shared_t(), get_object_ID()
//														  , M_cursor_pos);
//}

//void J_FT_Text_Display::move_cursor_line_pos_down(j_size_t i_move_val){
//	auto new_pen_pos = M_last_set_cursor_pos;
//	new_pen_pos.second -= (i_move_val*new_line_screen_size() + FLOAT_DELTA);
//	set_cursor_pos(get_cursor_index(new_pen_pos));
//	M_last_set_cursor_pos = M_pen_poses[M_cursor_pos];
//	M_last_set_cursor_pos.first = new_pen_pos.first;
//
//	J_UI_Controller::get_instance().notify_text_box_press(J_View_Shared_t(), get_object_ID()
//														  , M_cursor_pos);
//}
//
//void J_FT_Text_Display::move_cursor_to_line_begin(){
//	auto pen_pos = M_pen_poses[M_cursor_pos];
//	pen_pos.first = x1();
//	set_cursor_pos(min(get_cursor_index(pen_pos)+1, M_pen_poses.size()-1));
//	J_UI_Controller::get_instance().notify_text_box_press(J_View_Shared_t(), get_object_ID()
//														  , M_cursor_pos);
//}
//
//void J_FT_Text_Display::move_cursor_to_line_end(){
//	auto pen_pos = M_pen_poses[M_cursor_pos];
//	pen_pos.first = x2();
//	set_cursor_pos(get_cursor_index(pen_pos));
//	J_UI_Controller::get_instance().notify_text_box_press(J_View_Shared_t(), get_object_ID()
//														  , M_cursor_pos);
//}
//
//void J_FT_Text_Display::scroll(int i_scroll_val){
//	if(!i_scroll_val){
//		return;
//	}
//	
//	j_float scroll_size = -i_scroll_val*new_line_screen_size();
//	
//	if((scroll_size < 0) && M_pen_poses.front() == default_pen_pos()){
//		return;
//	}
//
//	if(scroll_size < 0){
//		j_float scroll_size_to_default 
//			= default_pen_pos().second - M_pen_poses.front().second;
//		scroll_size = max(scroll_size, scroll_size_to_default);
//	} else if(M_pen_poses.back().second > y2()){
//		return;
//	}else{
//		scroll_size = min(scroll_size, y2() - M_pen_poses.back().second);
//	}
//	auto new_starting_pen_pos = M_pen_poses.front();
//	new_starting_pen_pos.second += scroll_size;
//
//	set_starting_pen_pos(new_starting_pen_pos);
//	scroll_selection_boxes(0.0f, scroll_size);
//}
//
//int J_FT_Text_Display::lines_scrolled_per_tick(){
//	return 1;
//}
//
//void J_FT_Text_Display::set_cursor_pos_no_scroll(j_size_t i_pos){
//	M_cursor_pos = i_pos;
//	auto pen_pos_x = M_pen_poses[i_pos].first;
//	auto pen_pos_y = M_pen_poses[i_pos].second;
//
//
//
//	glBindBuffer(GL_ARRAY_BUFFER, M_cursor_buffer_id);
//
//	std::array<j_float, 4> cursor_data
//		= {pen_pos_x, pen_pos_y
//		, pen_pos_x, pen_pos_y
//		+ new_line_screen_size()
//	};
//
//	glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(0), sizeof(cursor_data), cursor_data.data());
//
//
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//}
//
//void J_FT_Text_Display::alert_cursor_pos(Pen_Pos_FL_t i_pos){
//
//	if(!M_left_mouse_button_pressed_status){
//		return;
//	}
//	j_set_cursor_type(J_I_BEAM_CURSOR_ID);
//	j_size_t cursor_index = get_cursor_index(i_pos);
//	set_cursor_pos(cursor_index);
//	j_size_t low_bound = min(cursor_index, M_selection_start_cursor_pos);
//	j_size_t right_bound = max(cursor_index, M_selection_start_cursor_pos);
//
//
//
//	if(low_bound == right_bound){
//		M_selection_boxes.clear();
//		return;
//	}
//
//	int num_display_boxes = 0;
//	
//
//	auto cur_cursor_pos = low_bound;
//	while(cur_cursor_pos < right_bound){
//		J_Rectangle rectangle;
//		rectangle.set_box(M_pen_poses[cur_cursor_pos].first
//						  , M_pen_poses[cur_cursor_pos].first
//						  , M_pen_poses[cur_cursor_pos].second+ new_line_screen_size() 
//						  , M_pen_poses[cur_cursor_pos].second);
//
//		j_float end_x_pos = rectangle.x1();
//		while((cur_cursor_pos < right_bound)
//			  && (M_pen_poses[cur_cursor_pos].second == rectangle.y2())){
//			end_x_pos = M_pen_poses[cur_cursor_pos].first;
//			++cur_cursor_pos;
//		}
//		rectangle.set_x2(end_x_pos);
//
//		if(M_selection_boxes.size() <= num_display_boxes){
//			assert(M_selection_boxes.size() == num_display_boxes);
//
//			J_Display_Box_Shared_t display_box(
//				new J_Display_Box(0)
//				);
//			M_selection_boxes.push_back(display_box);
//		}
//		
//		set_selection_box_settings(M_selection_boxes[num_display_boxes]
//								   , rectangle);
//		
//		++num_display_boxes;
//	}
//	M_selection_boxes.resize(num_display_boxes);
//}

void J_FT_Text_Display::add_letter_box(j_size_t i_index
									   , const Pen_Pos_FL_t& i_pen_pos
									   , const Bitmap_Metrics& i_metrics
									   , const J_UI_Color& i_color
									   , const j_ubyte* i_bitmap){
	
	J_Display_Letter_Box_Shared_t new_letter_box(new J_Display_Letter_Box(0));
	new_letter_box->set_image_box(i_pen_pos, i_metrics);
	new_letter_box->set_buffer_data(i_metrics, i_color, i_bitmap);

	M_letter_box_string.insert(M_letter_box_string.begin() + i_index, new_letter_box);
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
	j_size_t initial_string_size = M_letter_box_string.size();
	M_letter_box_string.resize(i_size);
	for(j_size_t i = initial_string_size; i < i_size; i++){
		M_letter_box_string[i] = (J_Display_Letter_Box_Shared_t(new J_Display_Letter_Box));
	}
}

void J_FT_Text_Display::set_letter_box_poses(j_size_t i_index
											 , j_size_t i_size
											 , const Pen_Pos_FL_t* i_poses){
	
	auto letter_pos_start = M_letter_box_string.begin() + i_index;

	for(int i = 0; i < i_size; i++){
		letter_pos_start[i]->set_pos(i_poses[i]);
	}
}

void J_FT_Text_Display::insert_text_string(j_size_t i_pos, j_size_t i_size
										   , const Pen_Pos_FL_t* i_poses
										   , Bitmap_Metrics** i_metrics
										   , const J_UI_Color& i_color
										   , const j_ubyte* const * i_datas){
	ex_array<J_Display_Letter_Box_Shared_t> utility_cont;
	for(int i = 0; i < i_size; i++){
		J_Display_Letter_Box_Shared_t letter_box(new J_Display_Letter_Box(0));
		const Bitmap_Metrics& metric = *(i_metrics[i]);
		letter_box->set_image_box(i_poses[i], metric);
		letter_box->set_buffer_data(metric, i_color, i_datas[i]);

		utility_cont.push_back(letter_box);
	}
	M_letter_box_string.insert(M_letter_box_string.begin() + i_pos
							   , utility_cont.begin(), utility_cont.end());


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