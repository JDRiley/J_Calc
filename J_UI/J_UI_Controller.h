#ifndef J_UI_CONTROLLER_H
#define J_UI_CONTROLLER_H

#include "J_UI_Fwd_Decl.h"

//
#include <iosfwd>
//
#include <map>
namespace jomike{

class J_UI_Controller{
public:
	void init(int argc, char** argv);
	void initialize_text_data();
	static J_UI_Controller& get_instance();
	//virtual void notify_text_box_release(j_uint text_obj_id, j_size_t cursor_index);
	//virtual void notify_text_box_press(J_View_Shared_t, j_uint text_obj_id, j_size_t cursor_index);
	

	
	//virtual void notify_cursor_pos(j_uint i_obj_id, Pen_Pos_FL_t i_pos);
	virtual void char_input_cmd(j_window_t, int);
	virtual void key_input_cmd(j_window_t, int charcode, int, int, int);
	virtual void mouse_button_cmd(j_window_t, int mouse_key, int action, int modifiers);
	virtual void mouse_button_cmd_n(j_window_t, int mouse_key, int action, int modifiers, int i_count);
	virtual ~J_UI_Controller();
	virtual void clear_all() = 0;
	virtual void add_view(J_View_Shared_t);
	virtual void remove_view(j_window_t);
	virtual bool has_views()const;
	virtual void draw_views()const;
	//virtual void notify_object_press(J_View_Shared_t i_view, j_uint i_obj_id, int i_key, int i_modifiers, Pen_Pos_FL_t i_pen_pos);
	//virtual void notify_object_press_n(J_View_Shared_t i_view, j_uint i_obj_id
	//								 , int i_key, int i_modifiers
	//								 , Pen_Pos_FL_t i_pen_pos, int i_count);
	virtual void cursor_pos_input_cmd(j_window_t i_window, j_dbl i_x_pos, j_dbl i_y_pos);
	j_dbl fps()const;
	virtual void end_script_run();
	virtual void resize_cmd(j_window_t, int width, int height);
	int key_modifiers()const;
	void update();
	int current_key_modifiers()const;
protected:
	J_UI_Controller();
	void add_cursor_pos_text_updater(J_Text_Box_Object_Shared_t
		, J_View_Weak_t);
	void add_draw_fps_updater(J_Text_Box_Object_Shared_t);
	void add_update_fps_updater(J_Text_Box_Object_Shared_t);
	void defocus_active_object(j_window_t i_window);
	virtual void run_script(const std::string& irk_file_name);
private:
	
	virtual void derived_init(int argc, char** argv) = 0;
	typedef std::map<j_window_t, J_View_Shared_t> J_View_Cont_t;
	J_View_Cont_t M_j_views;

	typedef std::map<J_View_Shared_t, J_UI_Object_Weak_t> Active_UI_Obj_Cont_t;
	Active_UI_Obj_Cont_t M_active_ui_objs;
	bool M_script_run_flag = false;
	J_Frame_Counter_Shared_t M_frame_counter;
	
	int M_last_key_modifiers;
};




}




#endif