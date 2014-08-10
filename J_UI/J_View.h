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

	virtual void add_ui_circle(J_UI_Circle_Shared_t);
	virtual void add_multi_state_text_box(Multi_State_Text_Box_Shared_t );

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

	
	virtual bool is_visible()const;
	virtual void set_cursor_pos(int,int);

	virtual void close_window();



	virtual void clear();
	void set_focused_object(J_UI_Object_Shared_t i_obj);
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