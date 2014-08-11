#ifndef J_UI_OBJECT_H
#define J_UI_OBJECT_H

#include "J_UI_Fwd_Decl.h"
//
#include "J_Outline_Fill_Management.h"
//Container
#include <ex_array.h>

#include <J_Shape.h>
//
#include "J_Notification_Handler.h"
namespace jomike{

j_dbl j_get_time();

typedef void (*Mouse_Press_Func_t)
		(J_UI_Object_Shared_t, int, int, Pen_Pos_FL_t);

typedef void(*Focus_Callback_Func_t)(J_UI_Object_Shared_t i_obj, bool i_focus);




class J_UI_Object_Update_Callback : J_UI_Handler{
public:
	virtual void operator()(J_UI_Object_Shared_t) = 0;
	virtual ~J_UI_Object_Update_Callback(){}
private:
};

template<typename Func_Obj_t>
class J_UI_Func_Obj_Callback : J_UI_Object_Update_Callback{
public:
	J_UI_Func_Obj_Callback(const Func_Obj_t&);
	void operator()(J_UI_Object_Shared_t)override;
private:
	Func_Obj_t M_func_object;
};

template<typename Func_Obj_t>
J_UI_Func_Obj_Callback<Func_Obj_t>::J_UI_Func_Obj_Callback(const Func_Obj_t& i_obj)
	:M_func_object(i_obj){}

template<typename Func_Obj_t>
void J_UI_Func_Obj_Callback<Func_Obj_t>::operator()(J_UI_Object_Shared_t i_obj){
	M_func_object(i_obj);
}

class J_UI_Object : public J_Outline_Fill_Management
	, public virtual J_Shape, public std::enable_shared_from_this<J_UI_Object>{
public:
	virtual void update();
	j_uint get_ID()const{return M_ID;}
#if VS_2013
	J_UI_Object& operator=(const J_UI_Object&) = delete;
#endif
	virtual void draw()const = 0;

	virtual void alert_cursor_pos(Pen_Pos_FL_t);
	virtual void set_left_click_on();
	virtual void set_left_click_off();
	bool clickable_status()const;
	virtual void broadcast_current_state()const;
	virtual void key_input_cmd(int charcode, int scancode, int action, int modifier);
	virtual void char_input_cmd(int /*charcode*/){}
	virtual void mouse_button_press(int,int, Pen_Pos_FL_t);
	virtual void mouse_button_press_n(int, int, Pen_Pos_FL_t, int);
	virtual void mouse_button_release(int,int, Pen_Pos_FL_t);
	virtual void set_mouse_button_press_callback(Mouse_Press_Func_t);
	virtual void set_mouse_button_release_callback(Mouse_Press_Func_t);
	void set_clickable_status(bool clickable);
	void toggle_n(int num_toggles);
	virtual ~J_UI_Object(){}
	virtual void toggle();
	bool toggle_status()const;
	void add_update_callback(J_UI_Object_Update_Callback_Shared_t);
	void copy_state(const J_UI_Object&);
	virtual void alert_resize(int, int);
	virtual void set_focus_status(bool i_status);
	bool focus_status()const;
	virtual void add_focus_callback(Focus_Callback_Func_t);
	bool left_click_on_status()const;
protected:
	J_UI_Object();
	J_UI_Object(j_uint i_id);
private:
	const j_uint M_ID;
	J_UI_Object_Shared_t M_parent;

	ex_array<J_UI_Object_Update_Callback_Shared_t> M_update_callbacks;
	ex_array<Focus_Callback_Func_t> M_focus_callbacks;
#ifdef VS_2013
	bool M_toggled_status = false;
	bool M_left_click_on_flag = false;
	Mouse_Press_Func_t M_mouse_press_callback = nullptr;
	Mouse_Press_Func_t M_mouse_release_callback = nullptr;
	bool M_clickable_flag = false;
	bool M_focus_status = false;
#else
	J_UI_Object& operator=(const J_UI_Object&);
	bool M_toggled_status;
	bool M_left_click_on_flag;
	Mouse_Press_Func_t M_mouse_press_callback;
	Mouse_Press_Func_t M_mouse_release_callback;
	bool M_focus_status;
	bool M_clickable_flag;
	void default_initialization();
#endif

};







template<typename UI_Object_t>
class Multi_State_UI_Object : public J_UI_Object{
public:
	template<typename Iter>
	Multi_State_UI_Object(Iter i_first, Iter i_second)
		:M_states(i_first, i_second){
#ifndef VS_2013
		default_initialization();
#endif //!VS_2013
	}

	Multi_State_UI_Object();

	void add_state(J_UI_Object_Shared_t);
	void set_state(j_size_t i_index);

	std::shared_ptr<UI_Object_t> operator[](j_size_t i_index);

private:
	ex_array<std::shared_ptr<UI_Object_t>> M_states;
	
#ifdef VS_2013
	int M_active_index = 0;
#else
	int M_active_index;
	void default_initialization();
#endif

};



template<typename UI_Object_t>
Multi_State_UI_Object<UI_Object_t>::Multi_State_UI_Object(){
#ifndef VS_2013
	default_initialization();
#endif //!VS_2013
}
#ifndef VS_2013
template<typename UI_Object_t>
void Multi_State_UI_Object<UI_Object_t>::default_initialization(){
	M_active_index = 0;
}
#endif // VS_2013


}




#endif