#ifndef J_VIEW_H
#define J_VIEW_H


#include "../J_UI/J_UI_Fwd_Decl.h"
#include <J_OpenGL_Fwd_Decl.h>
#include "J_Image/J_Image.h"
#include "J_UI_String/J_UI_String.h"
#include <RB_Tree.h>
#include <ex_array.h>
//Containers
#include <map>


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
	virtual void add_text_display(j_uint);
	virtual void add_display_box(j_uint);
	virtual void add_image_pane(j_uint);
	virtual void add_display_circle(j_uint);
	virtual void add_multi_state_text_box(j_uint);

	virtual void resize(int width, int height);
	virtual ~J_View();
	virtual j_uint mouse_button_press(J_View_Shared_t, int button, int modifier, Pen_Pos_FL_t pos);
	virtual void mouse_button_release(int button, int modifier, Pen_Pos_FL_t pos);
	virtual int get_x_resolution()const;
	virtual int get_y_resolution()const;
	virtual bool should_close()const;
	virtual void set_text_string(j_uint i_text_box_id, const J_UI_Multi_String&);
	virtual void set_text_cursor(j_uint i_text_box_id, j_uint cursor_pos);
	virtual void set_text_cursor_color(j_uint i_text_box_id, J_Color_RGBA<j_float>);
	virtual void set_cursor_visibility_status(j_uint i_text_box_id, bool i_status);
	virtual void set_clickable_status(j_uint i_obj_id, bool i_status);
	virtual void delete_char(j_uint text_box_id, j_size_t pos);
	virtual void erase_chars(j_uint text_box_id, j_size_t pos, j_size_t size);
	virtual void insert_char(j_uint text_box_it, j_size_t pos, J_UI_Char i_char);
	virtual void insert_chars(j_uint text_box_it, j_size_t pos, const J_UI_String&);
	virtual bool is_visible()const;
	virtual void set_cursor_pos(int,int);
	virtual void set_fill_visibility(j_uint i_disp_obj, bool status);
	virtual void set_outline_visibility(j_uint i_disp_obj, bool status);
	virtual void close_window();

	virtual void update_box_coordinates(j_uint i_obj_id, const J_Rectangle& i_rectangle);
	virtual void update_fill_color(j_uint, J_Color_RGBA<j_float>);
	virtual void update_outline_color(j_uint, J_Color_RGBA<j_float> );
	virtual void update_outline_thickness(j_uint, j_float);

	//circle
	virtual void update_circle_shape_data(j_uint i_circle_id, const J_Circle&);
	virtual void update_center(j_uint i_obj_id, j_float i_x_pos, j_float i_y_pos);
	virtual void update_radius(j_uint i_circle_id, j_float i_radius);

	//image
	virtual void update_image_clear(j_uint);
	virtual void update_image_buffer(j_uint, const j_ubyte*);
	virtual void update_image_buffer_mono(j_uint, const j_ubyte*);
	virtual void update_image_height(j_uint, int);
	virtual void update_image_width(j_uint, int);
	virtual void update_image_buffer_format(j_uint, Image_Format);
	virtual void update_middle_line_color(j_uint, J_UI_Color);


	virtual void update_add_multi_text_state(j_uint i_multi_text_state_id);
	virtual void update_multi_text_state(j_uint i_multi_state_text_id, j_size_t i_state);

	virtual void remove_text_box(j_uint i_obj_id);
	virtual void remove_display_box(j_uint i_obj_id);
	virtual void remove_display_object(j_uint i_obj_id);


	virtual void clear();
protected:
	J_Display_Object_Shared_t get_display_object_at_pos(Pen_Pos_FL_t);
	virtual void display_object_pressed(J_Display_Object_Shared_t, int action, int modifiers
		, Pen_Pos_FL_t);
	J_Display_Box_Shared_t get_display_box(j_uint i_obj_id);
	J_FT_Text_Display_Object_Shared_t get_text_display(j_uint i_obj_id);
	J_Display_Object_Shared_t get_display_object(j_uint i_obj_id);
	J_Display_Image_Pane_Shared_t get_image_pane(j_uint);
	J_Display_Circle_Shared_t get_display_circle(j_uint i_obj_id);
	J_FT_Text_Multi_State_Display_Shared_t get_multi_state_text_display(j_uint i_obj_id);
private:
	j_int M_view_id;
	J_Context_Shared_t M_context;

	unsigned M_background_program_id;
	
	std::map<j_uint, J_Display_Object_Shared_t> M_disp_objs_by_id;
	std::map<j_uint, J_Display_Box_Shared_t> M_disp_boxes;
	std::map<j_uint, J_FT_Text_Display_Object_Shared_t> M_text_displays;
	std::map<j_uint, J_Display_Image_Pane_Shared_t> M_image_panes;
	std::map<j_uint, J_Display_Circle_Shared_t> M_display_circles;
	std::map<j_uint, J_FT_Text_Multi_State_Display_Shared_t> M_multi_state_text_boxes;

	virtual void add_text_display(J_FT_Text_Display_Object_Shared_t);
	virtual void add_display_object(J_Display_Object_Shared_t);
	virtual void add_display_box(J_Display_Box_Shared_t);
	virtual void add_display_circle(J_Display_Circle_Shared_t);
	ex_array<J_Display_Object_Shared_t> M_disp_objs;
	
	j_float get_x_coord(int i_x_pos){
		return 2.0f*i_x_pos/get_x_resolution()-1.0f;
	}

	j_float get_y_coord(int i_y_pos){
		return 1.0f  - 2.0f*i_y_pos/get_y_resolution();
	}


	J_Display_Object_Shared_t M_cur_clicked_obj;
	Pen_Pos_FL_t M_cursor_pos;



	void draw_background()const;
};

j_window_t get_active_window();

J_Context_Shared_t get_active_context();


}

#endif