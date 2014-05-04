#ifndef J_OPENGL_H
#define J_OPENGL_H

#include "J_OpenGL_Fwd_Decl.h"
//
#include <J_Error.h>

#include <J_Utile.h> //For Instance Pointer

#include <J_Image.h>
#include <j_tree.h>

namespace jomike{
typedef struct GLEWContextStruct GLEWContext;
J_FWD_DECL(J_Context)

class Context_RAII{
public:
	Context_RAII();
	~Context_RAII();
private:
	J_Context_Shared_t M_context;
};

class Contexts_Handler{
public:
	
	J_Context_Shared_t create_j_context(int i_width, int i_height
													  , const char* const i_title
													  , j_monitor_t i_monitor = nullptr
													  , j_window_t i_share_window = nullptr);

	static j_window_t get_window(J_Context_Shared_t);
	static int get_width(J_Context_Shared_t);
	static int get_height(J_Context_Shared_t);

	j_uint screen_box_vao();
	J_Context_Shared_t get_active_context();
	j_context_t get_glew_context();
	j_window_t get_active_window();
	void make_context_active(J_Context_Shared_t);
	void detach_active_context();
	j_float ratio()const;
	void move_main_context_to_here();
	static Contexts_Handler& get_instance();
	void destroy();
	~Contexts_Handler();
private:
	
	Contexts_Handler();
	friend class Instance_Pointer<Contexts_Handler>;
	typedef j_tree<J_Context_Shared_t> J_Context_Cont_t;

	//J_Context_Shared_t M_main_context;
	J_Context_Shared_t M_active_context;
	j_context_t M_active_glew;
	J_Context_Cont_t M_active_contexts;
	J_Context_Cont_t M_contexts;


};


#ifdef WIN32

#endif

void j_wait_things_finished();


bool open_gl_error();

/*void init_opengl*/
/*
	Initializes glfw and glew
*/
void init_view_library();

void j_open_window(J_Context_Shared_t);

void j_close_window(J_Context_Shared_t);

void j_swap_buffers(J_Context_Shared_t);

void j_focus_context(J_Context_Shared_t);

void j_free_glfw();

j_dbl j_get_time();

void j_clear_color(J_Color_RGBA<float>);
void j_clear();

bool j_should_window_close(J_Context_Shared_t);

bool j_is_window_open(J_Context_Shared_t);

void j_poll_events();

void j_view_port(int x, int y, int width, int height);

void j_set_window_size(J_Context_Shared_t, int,int);




class J_OpenGL_Init_Error : public J_Error{
public:
	J_OpenGL_Init_Error(const char* const ik_message)
		: J_Error(ik_message){}
};

int get_x_res(j_window_t);
int get_y_res(j_window_t);

j_float convert_x_coord(j_window_t, int i_x_pixel);
j_float convert_y_coord(j_window_t, int i_y_pixel);

j_dbl get_x_pixel(j_window_t i_window, j_dbl i_x_screen);
j_dbl get_y_pixel(j_window_t i_window, j_dbl i_y_screen);

j_uint x_uns_pixel(j_window_t i_window, j_dbl i_x_screen);
j_uint y_uns_pixel(j_window_t i_window, j_dbl i_y_screen);

j_uint x_pixels(j_window_t i_window, j_dbl i_x_screen);

j_uint y_pixels(j_window_t i_window, j_dbl i_y_screen);



j_float to_x_screen(j_window_t i_window, int i_x_pixel);
j_float to_y_screen(j_window_t i_window, int i_y_pixel);

Pen_Pos_FL_t screen_coords(j_window_t i_window, Pen_Pos_t pos);

void j_set_cursor_pos(j_window_t i_window, j_dbl i_x_pos, j_dbl i_y_pos);


class J_OpenGL{
public:

private:
};



}



#endif