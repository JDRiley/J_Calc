#include "J_UI_Model.h"
#include <J_Error.h>

#include "J_FT_Text_Displayer/J_Text_Box.h"
#include "J_View.h"
//
#include <J_Frame_Counter.h>
//
#include <J_Utile.h>
//
#include "J_Image_Pane.h"
//
#include "J_UI_Line.h"
//
#include "J_FT_Text_Displayer/Multi_State_Text_Box.h"
//Algorithm
#include <algorithm>
#include <functional>

using std::bind; using std::for_each; using std::mem_fn; using std::transform;
using namespace std::placeholders;

//Utilities
#include <cassert>


//
#include <thread>

//
#include <future>
using std::future;
using std::async;

#ifdef VS_2013
using std::launch;
#else

namespace launch = std::launch;
#endif // VS_2013



#include <iostream>
using std::cerr; using std::endl;

const int MATH_FONT_FACE_SIZE = 36;
const int INPUT_FONT_FACE_SIZE = 18;
const int LOG_FONT_FACE_SIZE = 20;
const int ERROR_FONT_FACE_SIZE = 20;
namespace jomike{
#if _DEBUG
const int MODEL_FRAME_COUNTER_BUFFER = 200;
#else
const int MODEL_FRAME_COUNTER_BUFFER = 100;
#endif //_DEBUG
static Instance_Pointer<J_UI_Model> s_ui_data;

static FT_Face create_face(FT_Library, const char* pathname, int level);

J_UI_Model::J_UI_Model(): M_frame_counter(new J_Frame_Counter(MODEL_FRAME_COUNTER_BUFFER)){
	M_time = 0;
}



//J_Context_Shared_t share_context(J_Context_Shared_t);

void J_UI_Model::update(){
	M_time = j_get_time();
	for_each(M_ui_objects.begin()
		,M_ui_objects.end()
		, mem_fn(&J_UI_Object::update));
	assert(M_frame_counter);
	M_frame_counter->count_frame();
}

void J_UI_Model::attach_view(J_View_Shared_t i_view_ptr){
	bool inserted = M_views.insert(i_view_ptr).second;

	if(!inserted){
		throw J_Argument_Error("View Already A Member of J_UI_Model");
	}

	M_cursor_positions[i_view_ptr]
		= J_Cursor_Position_Shared_t(new J_Cursor_Position);
}

void J_UI_Model::add_ui_circle(J_UI_Circle_Shared_t i_circle_ptr){
	add_ui_object(i_circle_ptr);

	assert(!M_ui_circles.count(i_circle_ptr->get_ID()));
	M_ui_circles[i_circle_ptr->get_ID()] = i_circle_ptr;
}


void J_UI_Model::add_text_box_object(J_Text_Box_Object_Shared_t i_text_box_ptr){
	add_box(i_text_box_ptr);

	assert(!M_text_boxes.count(i_text_box_ptr->get_ID()));
	M_text_boxes[i_text_box_ptr->get_ID()] = i_text_box_ptr;
}

void J_UI_Model::add_multi_state_text_box(Multi_State_Text_Box_Shared_t i_text_box_ptr){
	add_text_box_object(i_text_box_ptr);

	assert(!M_multi_text_boxes.count(i_text_box_ptr->get_ID()));
	M_multi_text_boxes[i_text_box_ptr->get_ID()] = i_text_box_ptr;
}

void J_UI_Model::add_image_pane(J_Image_Pane_Shared_t i_image_pane_ptr){
	add_box(i_image_pane_ptr);

	assert(!M_image_panes.count(i_image_pane_ptr->get_ID()));
	M_image_panes[i_image_pane_ptr->get_ID()] = i_image_pane_ptr;
	
	
}


void J_UI_Model::add_ui_object(J_UI_Object_Shared_t i_obj_ptr){
	assert(!M_ui_objects.count(i_obj_ptr->get_ID()));
	M_ui_objects[i_obj_ptr->get_ID()] = i_obj_ptr;
	i_obj_ptr->broadcast_current_state();
}

void J_UI_Model::add_box(J_UI_Box_Shared_t i_display_box_ptr){
	add_ui_object(i_display_box_ptr);

	assert(!M_ui_boxes.count(i_display_box_ptr->get_ID()));
	M_ui_boxes[i_display_box_ptr->get_ID()] = i_display_box_ptr;
}

void J_UI_Model::remove_ui_object(j_uint i_obj_id){
	assert(M_ui_objects.count(i_obj_id));
	M_ui_objects.erase(i_obj_id);

	notify_ui_box_gone(i_obj_id);
}

void J_UI_Model::remove_box(j_uint i_obj_id){
	remove_ui_object(i_obj_id);
	assert(M_ui_boxes.count(i_obj_id));
	
	M_ui_boxes.erase(i_obj_id);
}

void J_UI_Model::remove_text_box_object(j_uint i_obj_id){
	remove_box(i_obj_id);
	assert(M_text_boxes.count(i_obj_id));

	M_text_boxes.erase(i_obj_id);

	notify_text_box_removal(i_obj_id);
}

J_Text_Box_Object_Shared_t J_UI_Model::get_text_box(j_uint i_id){
	if(!M_text_boxes.count(i_id)){
		assert(0);
	}

	return M_text_boxes[i_id];
}

J_UI_Box_Shared_t J_UI_Model::get_ui_box(j_uint i_id){
	if (!M_ui_boxes.count(i_id)){
		assert(0);
	}

	return M_ui_boxes[i_id];
}


J_UI_Object_Shared_t J_UI_Model::get_ui_object(j_uint i_id){
	if(!M_ui_objects.count(i_id)){
		assert(0);
	}

	return M_ui_objects[i_id];
}

J_UI_Circle_Shared_t J_UI_Model::get_ui_circle(j_uint i_id){
	if (!M_ui_circles.count(i_id)){
		assert(0);
	}
	return M_ui_circles[i_id];
}

void J_UI_Model::notify_text_box_removal(j_uint i_obj_id){
	for(auto f_view : M_views){
		f_view->remove_text_display(i_obj_id);
	}
}

void J_UI_Model::notify_ui_box_gone(j_uint i_obj_id){
	for(auto f_view : M_views){
		f_view->remove_display_box(i_obj_id);
	}
}

void J_UI_Model::notify_ui_object_removal(j_uint i_obj_id){
	for(auto f_view : M_views){
		f_view->remove_display_object(i_obj_id);
	}
}

void J_UI_Model::notify_char_delete(j_uint i_text_box_id, j_size_t i_pos){
	assert(M_text_boxes.count(i_text_box_id));

	for_each(M_views.begin(), M_views.end(), bind(&J_View::delete_char, _1, i_text_box_id, i_pos));
}

void J_UI_Model::notify_erase_chars(j_uint i_text_box_id, j_size_t i_pos, j_size_t i_size){
	assert(M_text_boxes.count(i_text_box_id));

	for_each(M_views.begin(), M_views.end(), bind(&J_View::erase_chars, _1, i_text_box_id, i_pos, i_size));
}

void J_UI_Model::notify_char_add(j_uint i_text_box_id, j_size_t i_index, const Pen_Pos_FL_t& i_pen_pos
					 , const Bitmap_Metrics& i_metrics, const J_UI_Color& i_color
					 , const j_ubyte* i_data){
	assert(M_text_boxes.count(i_text_box_id));

	for(auto& view : M_views){
		view->insert_char(i_text_box_id, i_index, i_pen_pos, i_metrics, i_color, i_data);
	}
}

void J_UI_Model::notify_fill_visibility_status(j_uint i_disp_obj_id, bool i_status){
	assert(M_ui_objects.count(i_disp_obj_id));
	for_each(M_views.begin(), M_views.end(), bind(&J_View::set_fill_visibility, _1, i_disp_obj_id, i_status));
}

void J_UI_Model::notify_outline_visibility_status(j_uint i_disp_obj_id, bool i_status){
	assert(M_ui_objects.count(i_disp_obj_id));
	for_each(M_views.begin(), M_views.end(), bind(&J_View::set_outline_visibility, _1, i_disp_obj_id, i_status));
}

void J_UI_Model::notify_box_coordinates(j_uint i_obj_id, const J_Rectangle& i_rectangle)
{
	M_views.apply(bind(&J_View::update_box_coordinates,_1, i_obj_id, i_rectangle));
}
void J_UI_Model::notify_fill_color(j_uint i_obj_id, const J_UI_Color& i_color)
{
	M_views.apply(bind(&J_View::update_fill_color, _1, i_obj_id, i_color));
}
void J_UI_Model::notify_outline_color(j_uint i_obj_id
										  , const J_Color_RGBA<j_float> i_color){
	M_views.apply(bind(&J_View::update_outline_color, _1, i_obj_id, i_color));
}

void J_UI_Model::notify_circle_shape_data(j_uint i_obj_id, const J_Circle& irk_circle){
	M_views.apply(bind(&J_View::update_circle_shape_data, _1, i_obj_id, irk_circle));
}

void J_UI_Model::notify_center(j_uint i_obj_id, j_float i_x_pos, j_float i_y_pos){
	M_views.apply(bind(&J_View::update_center, _1, i_obj_id, i_x_pos, i_y_pos));
}

void J_UI_Model::notify_radius(j_uint i_obj_id, j_float i_radius){
	M_views.apply(bind(&J_View::update_radius, _1, i_obj_id, i_radius));
}

void J_UI_Model::notify_image_clear(j_uint i_obj_id){
	M_views.apply(bind(&J_View::update_image_clear, _1, i_obj_id));
}

void J_UI_Model::notify_new_image_buffer(j_uint i_obj_id, const j_ubyte* i_buffer){
	M_views.apply(bind(&J_View::update_image_buffer, _1, i_obj_id, i_buffer));
}

void J_UI_Model::notify_new_image_buffer_mono(j_uint i_obj_id, const j_ubyte* i_buffer){
	M_views.apply(bind(&J_View::update_image_buffer_mono, _1, i_obj_id, i_buffer));
}

void J_UI_Model::notify_image_height(j_uint i_obj_id, int i_height){
	M_views.apply(bind(&J_View::update_image_height, _1, i_obj_id, i_height));
}

void J_UI_Model::notify_image_width(j_uint i_obj_id, int i_width){
	M_views.apply(bind(&J_View::update_image_width, _1, i_obj_id, i_width));
}


void J_UI_Model::notify_outline_thickness(j_uint i_obj_id, j_float i_thickness){
	M_views.apply(bind(&J_View::update_outline_thickness, _1, i_obj_id, i_thickness));
}

void J_UI_Model::notify_image_input_format(j_uint i_obj_id, Image_Format i_format){
	M_views.apply(bind(&J_View::update_image_buffer_format, _1, i_obj_id, i_format));
}


void J_UI_Model::notify_middle_line_color(j_uint i_obj_id, J_UI_Color i_color){
	M_views.apply(bind(&J_View::update_middle_line_color, _1, i_obj_id, i_color));
}


void J_UI_Model::notify_multi_text_state(j_uint i_multi_state_text_id, j_size_t i_state){
	M_views.apply(bind(&J_View::update_multi_text_state, _1, i_multi_state_text_id, i_state));
}

void J_UI_Model::notify_add_multi_text_state(j_uint i_multi_state_text_id){
	M_views.apply(bind(&J_View::update_add_multi_text_state, _1, i_multi_state_text_id));
}


bool J_UI_Model::is_text_box_present(j_uint i_text_box_id){
	return (0 != M_text_boxes.count(i_text_box_id));
}


void J_UI_Model::notify_clickable_status(j_uint i_obj_id, bool i_status){
	assert(M_ui_objects.count(i_obj_id));
	for_each(M_views.begin(), M_views.end(), bind(&J_View::set_clickable_status, _1, i_obj_id, i_status));
}

void J_UI_Model::broadcast_current_state()const{
	for(auto obj : M_ui_objects){
		obj->broadcast_current_state();	
	}
}

void J_UI_Model::remove_view(J_View_Shared_t i_view){
	M_views.erase(i_view);
	M_cursor_positions.erase(i_view);
}

J_UI_Model::~J_UI_Model(){}

void J_UI_Model::clear_data(){
	M_ui_objects.clear();
	M_ui_boxes.clear();
	M_text_boxes.clear();
	M_image_panes.clear();
	M_views.clear();

}


j_dbl J_UI_Model::time()const{
	return M_time;
}

void J_UI_Model::set_cursor_pos(J_View_Shared_t i_view, j_dbl i_x_pos, j_dbl i_y_pos){
	assert(M_cursor_positions.count(i_view));
	M_cursor_positions[i_view]
		->set_cursor_position(i_view->get_window(), i_x_pos, i_y_pos);
}

const J_Cursor_Position& J_UI_Model::cursor_pos(J_View_Shared_t i_view)const{
	return **M_cursor_positions.find(i_view);
}

j_dbl J_UI_Model::fps()const{
	assert(M_frame_counter);
	return M_frame_counter->fps();
}

void J_UI_Model
	::notify_letter_box_rectangle(j_uint i_text_box_id
								, j_size_t i_index , const Pen_Pos_FL_t& i_pen_pos
								, const Bitmap_Metrics& i_metrics){
		for(auto& view : M_views){
			view->update_letter_box_rectangle(i_text_box_id, i_index
											  , i_pen_pos, i_metrics);
		}
}

void J_UI_Model::add_managed_display_object(j_uint i_obj_id, j_uint i_new_obj_id
											, UI_Object_Types i_obj_type){
	for(auto& view : M_views){
		view->add_managed_display_object(i_obj_id, i_new_obj_id, i_obj_type);
	}
}

void J_UI_Model::remove_managed_display_object(j_uint i_obj_id, UI_Object_Types i_obj_type){
	for(auto view : M_views){
		view->remove_managed_display_object(i_obj_id, i_obj_type);
	}
}

void J_UI_Model::remove_image_pane(j_uint i_obj_id){
	auto found_pos = M_image_panes.find(i_obj_id);
	if(M_image_panes.end() == found_pos){
		return;
	}
	M_image_panes.erase(found_pos);
	remove_box(i_obj_id);
}

void J_UI_Model::remove_multi_state_text_box(j_uint i_obj_id){
	auto found_pos = M_multi_text_boxes.find(i_obj_id);
	if(M_multi_text_boxes.end() == found_pos){
		return;
	}
	M_multi_text_boxes.erase(found_pos);
	remove_text_box_object(i_obj_id);
}

void J_UI_Model::notify_line_data(j_uint i_line_id, const J_Line& i_line){
	for(auto view : M_views){
		view->update_line_data(i_line_id, i_line);
	}
}

void J_UI_Model::add_ui_line(J_UI_Line_Shared_t i_line){
	add_ui_object(i_line);
	assert(!M_ui_lines.count(i_line->get_ID()));
	M_ui_lines[i_line->get_ID()] = i_line;
}

void J_UI_Model::notify_letter_box_data(j_uint i_text_box_id, j_size_t i_index
										,const Bitmap_Metrics& i_metrics
										, const J_UI_Color& i_color
										, const j_ubyte* i_data){
	for(auto view : M_views){
		view->update_letter_box_data(i_text_box_id, i_index, i_metrics, i_color, i_data);
	}

}

void J_UI_Model::notify_text_string_size(j_uint i_obj_id, j_size_t i_size){
	for(auto view : M_views){
		view->update_text_string_size(i_obj_id, i_size);
	}
}

void J_UI_Model::notify_letter_box_poses(j_uint i_text_box_id, j_size_t i_pos, j_size_t i_size, const Pen_Pos_FL_t* i_poses){
	for(auto view : M_views){
		view->update_letter_box_poses(i_text_box_id, i_pos, i_size, i_poses);
	}
}

void J_UI_Model::notify_text_string_add(j_uint i_text_box_id, j_size_t i_pos, j_size_t i_size
										, const Pen_Pos_FL_t* i_poses, Bitmap_Metrics** i_metrics
										, const J_UI_Color& i_color
										, const j_ubyte* const * i_datas){
	for(auto view : M_views){
		view->update_text_string_add(i_text_box_id, i_pos, i_size, i_poses, i_metrics
									 , i_color, i_datas);
	}
}

void J_UI_Model::notify_object_draw_position_before(j_uint i_object_id, j_uint i_before_id){
	for(auto f_view : M_views){
		f_view->position_before(i_object_id, i_before_id);
	}
}

void J_UI_Model::notify_object_draw_position_after(j_uint i_object_id, j_uint i_after_id){
	for(auto f_view : M_views){
		f_view->position_after(i_object_id, i_after_id);
	}
}


Pen_Pos_FL_t screen_coords(j_window_t i_window, Pen_Pos_t pos);

void J_Cursor_Position::set_cursor_position(j_window_t i_window, j_dbl i_x_pos, j_dbl i_y_pos){
	M_cursor_pos.first = static_cast<int>(i_x_pos);
	M_cursor_pos.second = static_cast<int>(i_y_pos);
	M_cursor_pos_fl = screen_coords(i_window, M_cursor_pos);
}

Pen_Pos_FL_t J_Cursor_Position::cursor_pos_fl()const{
	return M_cursor_pos_fl;
}

Pen_Pos_t J_Cursor_Position::cursor_pos()const{
	return M_cursor_pos;
}

j_dbl get_model_time(){
	return J_UI_Model::get_instance().time();
}

}
