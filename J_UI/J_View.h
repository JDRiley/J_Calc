#ifndef J_VIEW_H
#define J_VIEW_H


#include "../J_UI/J_UI_Fwd_Decl.h"
//
#include <J_OpenGL_Fwd_Decl.h>
//
#include "J_Image/J_Image.h"
//
#include "J_UI_String/J_UI_String.h"
//
#include <j_tree.h>
//
#include <ex_array.h>
//Containers
#include <j_map.h>


namespace jomike{

class J_View : public std::enable_shared_from_this<J_View>{
public:

	J_View(int i_width, int i_height, const char* i_title = "A J_Calc_View Object!"
									, j_monitor_t i_monitor = nullptr
									, j_window_t i_share_window = nullptr);

	//Window Management
	void open_window();
	void make_active_context()const;
	void add_to_buffer()const;
	void set_context(J_Context_Shared_t);
	bool has_children()const;
	virtual void draw()const;
	virtual void clear_window()const;
	J_Context_Shared_t get_context();
	j_window_t get_window()const;
	virtual void add_ui_line(J_UI_Line_Shared_t i_line_id);
	virtual void add_text_box(J_Text_Box_Object_Shared_t i_text_box);
	virtual void update();
	virtual void add_ui_box(J_UI_Box_Shared_t i_ui_box);
	virtual void add_ui_object(J_UI_Object_Shared_t);
	//virtual void add_image_pane(j_uint);
	virtual void add_ui_circle(J_UI_Circle_Shared_t);
	virtual void add_multi_state_text_box(Multi_State_Text_Box_Shared_t );
	//virtual void add_managed_display_object(j_uint i_obj_id, j_uint i_new_obj_id, UI_Object_Types i_obj_type);
	//virtual void remove_managed_display_object(j_uint i_obj_id, UI_Object_Types i_obj_type);
	//
	//virtual void update_text_string_size(j_uint i_obj_id, j_size_t i_size);
	virtual void resize(int width, int height);
	virtual ~J_View();
	virtual j_uint mouse_button_press(J_View_Shared_t, int button, int modifier, Pen_Pos_FL_t pos);
	virtual j_uint mouse_button_press_n(J_View_Shared_t, int button
										, int modifier, Pen_Pos_FL_t pos, int i_count);
	virtual void mouse_button_release(J_View_Shared_t i_view, int i_button, int i_modifiers, Pen_Pos_FL_t i_pos);
	virtual int get_x_resolution()const;
	virtual int get_y_resolution()const;
	virtual bool should_close()const;
	void subscribe_cursor_updates(J_UI_Object_Weak_t i_ui_obj);

	/*virtual void update_letter_box_rectangle(j_uint i_text_box_id, j_size_t i_index
											 , const Pen_Pos_FL_t& i_rec
											 , const Bitmap_Metrics& i_metrics);
	virtual void update_letter_box_data(j_uint i_text_box_id, j_size_t i_index
										, const Bitmap_Metrics& i_metrics
										, const J_UI_Color& i_color
										, const j_ubyte* i_data);


	virtual void update_letter_box_poses(j_uint i_text_box_id, j_size_t i_pos
										 , j_size_t i_size
										 , const Pen_Pos_FL_t* i_poses);*/

	//virtual void update_text_string_add(j_uint i_text_box_id, j_size_t i_pos, j_size_t i_size, const Pen_Pos_FL_t* i_poses, Bitmap_Metrics** i_metrics, const J_UI_Color& i_color, const j_ubyte* const * i_datas);
	//virtual void set_clickable_status(j_uint i_obj_id, bool i_status);
	//virtual void delete_char(j_uint text_box_id, j_size_t pos);
	//virtual void erase_chars(j_uint text_box_id, j_size_t pos, j_size_t size);
	//virtual void insert_char(j_uint i_text_box_id, j_size_t i_pos, const Pen_Pos_FL_t& i_pen_pos
	//						 , const Bitmap_Metrics& i_metrics, const J_UI_Color& i_color
	//						 , const j_ubyte* i_data);
	
	virtual bool is_visible()const;
	virtual void set_cursor_pos(int,int);
	//virtual void set_fill_visibility(j_uint i_disp_obj, bool status);
	//virtual void set_outline_visibility(j_uint i_disp_obj, bool status);
	virtual void close_window();

	//virtual void update_box_coordinates(j_uint i_obj_id, const J_Rectangle& i_rectangle);
	//virtual void update_fill_color(j_uint, J_Color_RGBA<j_float>);
	//virtual void update_outline_color(j_uint, J_Color_RGBA<j_float> );
	//virtual void update_outline_thickness(j_uint, j_float);

	//Line
	//virtual void update_line_data(j_uint i_obj_id, const J_Line& i_line);

	//circle
	//virtual void update_circle_shape_data(j_uint i_circle_id, const J_Circle&);
	//virtual void update_center(j_uint i_obj_id, j_float i_x_pos, j_float i_y_pos);
	//virtual void update_radius(j_uint i_circle_id, j_float i_radius);

	//image
	//virtual void update_image_clear(j_uint);
	//virtual void update_image_buffer(j_uint, const j_ubyte*);
	//virtual void update_image_buffer_mono(j_uint, const j_ubyte*);
	//virtual void update_image_height(j_uint, int);
	//virtual void update_image_width(j_uint, int);
	//virtual void update_image_buffer_format(j_uint, Image_Format);
	//virtual void update_middle_line_color(j_uint, J_UI_Color);


	//virtual void update_add_multi_text_state(j_uint i_multi_text_state_id);
	//virtual void update_multi_text_state(j_uint i_multi_state_text_id, j_size_t i_state);

	//virtual void remove_text_display(j_uint i_obj_id);
	//virtual void remove_display_box(j_uint i_obj_id);
	//virtual void remove_display_object(j_uint i_obj_id);
	//virtual void remove_display_circle(j_uint i_obj_id);
	//virtual void remove_image_pane(j_uint i_obj_id);
	//virtual void remove_multi_state_text_box(j_uint i_obj_id);
	//virtual void remove_display_line(j_uint i_line_id);
	//virtual void position_after(j_uint i_front_pos_id, j_uint i_after_pos_id);
	//virtual void position_before(j_uint i_front_pos_id, j_uint i_before_pos_id);
	//bool is_display_object_present(j_uint i_obj_id)const;

	virtual void clear();

	virtual void char_input_cmd(j_uint i_charcode);
	virtual void key_input_cmd(int i_key, int i_scancode, int i_action, int i_modifiers);
protected:
	J_UI_Object_Shared_t get_display_object_at_pos(Pen_Pos_FL_t);
	virtual void display_object_pressed(J_UI_Object_Shared_t, int action, int modifiers
		, Pen_Pos_FL_t);
	J_UI_Box_Shared_t get_display_box(j_uint i_obj_id);
	J_Text_Box_Shared_t get_text_display(j_uint i_obj_id);
	J_UI_Object_Shared_t get_display_object(j_uint i_obj_id);
	J_Image_Pane_Shared_t get_image_pane(j_uint);
	J_UI_Circle_Shared_t get_display_circle(j_uint i_obj_id);
	Multi_State_Text_Box_Shared_t get_multi_state_text_display(j_uint i_obj_id);
	J_UI_Line_Shared_t get_display_line(j_uint i_obj_id);

private:
	j_int M_view_id;
	J_Context_Shared_t M_context;


	j_shared_ptr_tree<J_UI_Object> M_ui_objects;
	j_map<j_uint, J_UI_Box_Shared_t> M_disp_boxes;
	j_shared_ptr_tree<J_Text_Box_Object> M_text_box_objects;

	j_map<j_uint, J_Image_Pane_Shared_t> M_image_panes;
	j_map<j_uint, J_UI_Circle_Shared_t> M_display_circles;
	j_map<j_uint, Multi_State_Text_Box_Shared_t> M_multi_state_text_boxes;
	j_map<j_uint, J_UI_Line_Shared_t> M_display_lines;

	j_tree<J_UI_Object_Weak_t,J_UI_Object_Weak_t
		, std::owner_less<J_UI_Object_Weak_t>> M_objects_using_cursor_data;


	


	
	ex_array<J_UI_Object_Shared_t> M_ui_object_stack;
	
	j_float get_x_coord(int i_x_pos){
		return 2.0f*i_x_pos/get_x_resolution()-1.0f;
	}

	j_float get_y_coord(int i_y_pos){
		return 1.0f  - 2.0f*i_y_pos/get_y_resolution();
	}


	J_UI_Object_Weak_t M_focused_object;
	Pen_Pos_FL_t M_cursor_pos;


	
	void draw_background()const;
};

j_window_t get_active_window();

J_Context_Shared_t get_active_context();


}

#endif