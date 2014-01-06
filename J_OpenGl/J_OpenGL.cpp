#include "J_OpenGL.h"
//
#include "J_Frame_Counter.h"
//
#include <J_String.h>

#define  J_GL_DEBUG 1
#define PRINT_SHADER_SOURCE 0
#include <algorithm>
#include <functional>
using std::bind;
using std::mem_fn;
using namespace std::placeholders;
using std::remove_if;

//Libraries
#include <GL\glew.h>
#include <GLFW\glfw3.h>

//Containers
#include <string>

using std::string; 

//IO Facilities 
#include <fstream>
#include <iostream>
using std::cerr; using std::endl; using std::ifstream;

//Utilities
#include <iterator>
#include <numeric>
using std::accumulate;
using std::istreambuf_iterator;
using std::unique_ptr;

#include <chrono>
namespace chrono = std::chrono;
#define BUFFER_OFFSET(x)  ((const void*) (x))



static jtl::Instance_Pointer<jtl::Contexts_Handler> s_contexts_handler;

namespace jomike{

const int THREAD_WAIT_MS = 50;
class J_Context{
public:

	int M_width, M_height;
	j_window_t M_window = nullptr;
	GLEWContextStruct* M_context = nullptr;
	~J_Context();

};

J_Context::~J_Context(){
	delete M_context;
	glfwDestroyWindow(M_window);
}



Contexts_Handler& Contexts_Handler::get_instance(){
	static Contexts_Handler contexts_handler;
	return contexts_handler;
}

Contexts_Handler::Contexts_Handler(){init_view_library();}

Context_RAII::Context_RAII():M_context(s_contexts_handler->get_active_context()){
	if(M_context){
		s_contexts_handler->detach_active_context();
	}
}



Context_RAII::~Context_RAII(){

	if (s_contexts_handler->get_active_context()){
		s_contexts_handler->detach_active_context();
	}

	if(M_context){
		s_contexts_handler->make_context_active(M_context);
	}
}


//J_Context---------------------------------------------------------------------------------------

J_Context_Shared_t Contexts_Handler::get_active_context(){
	//std::thread::id thread_id= this_thread::get_id();
	//lock_guard<recursive_mutex> lg(M_mutex);
	return M_active_context;
}

j_context_t Contexts_Handler::get_glew_context(){
	return M_active_glew;
}

Contexts_Handler::~Contexts_Handler(){
	destroy();
}
void Contexts_Handler::destroy(){
	glfwTerminate();
	//this_thread::sleep_for(chrono::milliseconds(2000));
}



j_window_t Contexts_Handler::get_active_window(){
	return M_active_context->M_window;
}

void Contexts_Handler::make_context_active(J_Context_Shared_t i_context){
	//lock_guard<recursive_mutex> lg(M_mutex);
	//thread::id thread_id = this_thread::get_id();
	assert(i_context);
	if(i_context == M_active_context){
		return;
	}
	M_active_context = i_context;
	M_active_glew = M_active_context->M_context;
	glfwMakeContextCurrent(i_context->M_window);
}

void Contexts_Handler::detach_active_context(){
	M_active_context.reset();
	M_active_glew = nullptr;
	glfwMakeContextCurrent(nullptr);

}

int Contexts_Handler::get_width(J_Context_Shared_t i_context){
	return i_context->M_width;
}

int Contexts_Handler::get_height(J_Context_Shared_t i_context){
	return i_context->M_height;
}


void Contexts_Handler::move_main_context_to_here(){
	/*lock_guard<recursive_mutex> lg(M_mutex);
	assert(0);
	if(M_active_contexts.count(M_main_context)){
		std::remove_if(make_pair_iter(M_contexts_by_thread_id.begin()),
			make_pair_iter(M_contexts_by_thread_id.end())
			, bind(std::equal_to<J_Context_Shared_t>(), _1, M_main_context));
	}
	M_active_contexts.insert(M_main_context);
	M_contexts_by_thread_id[this_thread::get_id()] = M_main_context;
	glfwMakeContextCurrent(M_main_context->M_window);
	*/
}



J_Context_Shared_t Contexts_Handler::create_j_context(int i_width, int i_height
													  , const char* const i_title
													  , j_monitor_t i_monitor
													  , j_window_t i_share_window){
	glfwWindowHint(GLFW_VISIBLE, false);

	J_Context_Shared_t new_context(new J_Context);
	//M_main_context = M_main_context ? M_main_context : new_context;
	Context_RAII context_saver;
	new_context->M_width = i_width;
	new_context->M_height = i_height;

	if(!(new_context->M_window = glfwCreateWindow(i_width, i_height, i_title, i_monitor, i_share_window))){
		
		throw J_OpenGL_Init_Error("Could not create glfw window!");
	}

	glfwMakeContextCurrent(new_context->M_window);
	glClearColor(0.1f,0.0f,0.4f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(new_context->M_window);
	new_context->M_context = new GLEWContext();

	

	M_active_context = new_context;
	M_active_glew = new_context->M_context;
	if(int error_code = glewInit()){
		cerr << "\nError Code: " << error_code << endl;
		throw J_OpenGL_Init_Error("Could Not Initialize Glew!");
	}


	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return new_context;
}

j_window_t Contexts_Handler::get_window(J_Context_Shared_t i_context){
	return i_context->M_window;
}

void j_open_window(J_Context_Shared_t i_context){
	glfwShowWindow(i_context->M_window);
}

void j_close_window(J_Context_Shared_t i_context){
	glfwHideWindow(i_context->M_window);
}

void j_clear_color(J_Color_RGBA<float> i_color){
	glClearColor(i_color.red(), i_color.green(), i_color.blue(), i_color.alpha());
}
void j_clear(){
	glClear(GL_COLOR_BUFFER_BIT);
}

void j_init_context(j_context_t i_context){
	glewContextInit(i_context);
}

#ifdef WIN32
j_context_t j_create_glew_context(){
	return new GLEWContext();
}

void j_delete_glew_context(j_context_t i_context){
	
	delete i_context;
}

#endif



J_Frame_Counter::J_Frame_Counter(int i_frame_threshold):M_frame_threshold(i_frame_threshold){
	M_fps = M_frame_count = 0;
	M_time_since_last_count = j_get_time();
	M_fps_memory.resize(M_memory_size, 0.0);
}

void J_Frame_Counter::count_frame(){


	if(++M_frame_count > M_frame_threshold){
		j_dbl time = j_get_time();
		
		M_fps_memory.pop_back();
		M_fps_memory.push_front(M_frame_count / (time - M_time_since_last_count));
		M_fps = accumulate(M_fps_memory.begin(), M_fps_memory.end(), 0.0)/M_memory_size;
		M_time_since_last_count = time;
		M_frame_count = 0;

		assert(M_memory_size == M_fps_memory.size());

	}
	
	
}

j_dbl J_Frame_Counter::fps()const{return M_fps;}


void j_view_port(int i_x, int i_y, int i_width, int i_height){
	s_contexts_handler->get_active_context()->M_width = i_width;
	s_contexts_handler->get_active_context()->M_height = i_height;
	glViewport(i_x, i_y, i_width, i_height);
}

void j_free_glfw(){
	glfwTerminate();
}

j_float Contexts_Handler::ratio()const{
	return static_cast<j_float>(M_active_context->M_width)/ M_active_context->M_height;
}
void j_poll_events(){
	glfwPollEvents();
}
void j_set_window_size(J_Context_Shared_t i_context, int i_width, int i_height){
	glfwSetWindowSize(i_context->M_window, i_width, i_height);
	i_context->M_height = i_height;
	i_context->M_width = i_width;
	j_view_port(0,0, i_width, i_height);
}

j_dbl j_get_time(){
	return glfwGetTime();
}

void j_swap_buffers(J_Context_Shared_t i_context){
	glfwSwapBuffers(i_context->M_window);
}

void j_make_context_current(j_window_t i_window){
	glfwMakeContextCurrent(i_window);
}

const j_window_t j_current_context(){
	return glfwGetCurrentContext();
}
void j_wait_things_finished(){
	glFlush();
}

bool j_should_window_close(J_Context_Shared_t i_context){
	return glfwWindowShouldClose(i_context->M_window);
}

bool j_is_window_open(J_Context_Shared_t i_context){
	return glfwGetWindowAttrib(i_context->M_window, GLFW_VISIBLE);
}

void delete_j_window(j_window_t i_window){glfwDestroyWindow(i_window);}

void init_view_library(){

	if(!glfwInit()){
		throw jtl::J_OpenGL_Init_Error("glfw could not start");
	}
	glewExperimental = true;

}

j_window_t create_window(int i_width, int i_height, const char* ik_title
						 , j_monitor_t i_monitor, j_window_t i_share_window){
	return glfwCreateWindow(i_width, i_height, ik_title, i_monitor, i_share_window);
}


/*int get_x_res(j_window_t)*/
int get_x_res(j_window_t i_window){
	int x_res;int dummy_val;
	glfwGetWindowSize(i_window, &x_res, &dummy_val);
	return x_res;
}

/*int get_y_res(j_window_t)*/
int get_y_res(j_window_t i_window){
	int y_res;int dummy_val;
	glfwGetWindowSize(i_window, &dummy_val, &y_res);
	return y_res;
}



/*int get_x_res(j_window_t)*/
int get_x_res(J_Context_Shared_t i_context){
	return i_context->M_width;
}
/*int get_y_res(j_window_t)*/
int get_y_res(J_Context_Shared_t i_context){
	return i_context->M_height;
}


j_dbl get_x_pixel(j_window_t i_window, j_dbl i_x_screen){
	return (i_x_screen + 1.0f)*get_x_res(i_window) / 2.0f;
}
j_dbl get_y_pixel(j_window_t i_window, j_dbl i_y_screen){
	return -(i_y_screen - 1.0f)*get_y_res(i_window) / 2.0f;
}
/*j_float convert_x_coord(j_window_t, int)*/
j_float convert_x_coord(j_window_t i_window, int i_x_pixel){
	return -1.0f + to_x_screen(i_window, i_x_pixel);
}

/*j_float convert_y_coord(j_window_t, int)*/
j_float convert_y_coord(j_window_t i_window, int i_y_pixel){
	return 1.0f - to_y_screen(i_window, i_y_pixel);
}

j_float to_x_screen(j_window_t i_window, int i_x_pixel){
	return static_cast<j_float>(2)*i_x_pixel/get_x_res(i_window);
}

j_float to_y_screen(j_window_t i_window, int i_y_pixel){
	return static_cast<j_float>(2)*i_y_pixel/get_y_res(i_window);
}

Pen_Pos_FL_t screen_coords(j_window_t i_window, Pen_Pos_t i_pos){
	return Pen_Pos_FL_t(convert_x_coord(i_window, i_pos.first), convert_y_coord(i_window, i_pos.second));
}


void j_focus_context(J_Context_Shared_t i_context){
	glfwShowWindow(i_context->M_window);
}

void j_set_cursor_pos(j_window_t i_window, j_dbl i_x_pos, j_dbl i_y_pos){
	j_dbl x_pixel = get_x_pixel(i_window, i_x_pos);
	j_dbl y_pixel = get_y_pixel(i_window, i_y_pos);
	glfwSetCursorPos(i_window, x_pixel, y_pixel);
}

}

extern "C" j_context_t glewGetContext(){
	//assert(s_contexts_handler->get_active_context());
	return (s_contexts_handler->get_glew_context());
}


