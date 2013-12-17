#include "J_OpenGl.h"
#include "../J_String/J_String.h"

#define  J_GL_DEBUG 1

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


using std::istreambuf_iterator;
using std::unique_ptr;

#include <chrono>
namespace this_thread = std::this_thread;
namespace chrono = std::chrono;
using std::thread;
#define BUFFER_OFFSET(x)  ((const void*) (x))


using std::atomic_bool;
static jtl::Instance_Pointer<jtl::Contexts_Handler> s_contexts_handler;
using std::lock_guard; using std::mutex;
using std::recursive_mutex;
namespace jomike{

const int THREAD_WAIT_MS = 50;
class J_Context{
public:
	int M_width, M_height;
	j_window_t M_window;
	std::unique_ptr<GLEWContextStruct> M_context;
	~J_Context();

};

J_Context::~J_Context(){
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
	assert( s_contexts_handler->get_active_context() != M_context);
	assert(s_contexts_handler->get_active_context());
	s_contexts_handler->detach_active_context();
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

Contexts_Handler::~Contexts_Handler(){
	glfwTerminate();
}

void Contexts_Handler::make_context_active(J_Context_Shared_t i_context){
	//lock_guard<recursive_mutex> lg(M_mutex);
	//thread::id thread_id = this_thread::get_id();
	assert(i_context);
	if(i_context == M_active_context){
		return;
	}
	

		
	glfwMakeContextCurrent(i_context->M_window);
	return;
	}

	assert(M_contexts.count(i_context));

	assert(!M_active_contexts.count(i_context));
	if(M_contexts_by_thread_id.count(thread_id)){
		detach_active_context();
	}

	M_active_contexts.insert(i_context);
	M_contexts_by_thread_id[thread_id] = i_context;
	glfwMakeContextCurrent(i_context->M_window);
}

void Contexts_Handler::detach_active_context(){
	//lock_guard<recursive_mutex> lg(M_mutex);
	auto context_it = M_contexts_by_thread_id.find(this_thread::get_id());

	assert(M_contexts_by_thread_id.end() != context_it);

	
	assert(M_active_contexts.count(context_it->second));
	M_active_contexts.erase(context_it->second);
	M_contexts_by_thread_id.erase(context_it);
	glfwMakeContextCurrent(nullptr);

}

int Contexts_Handler::get_width(J_Context_Shared_t i_context){
	return i_context->M_width;
}

int Contexts_Handler::get_height(J_Context_Shared_t i_context){
	return i_context->M_height;
}

mutex& Contexts_Handler::main_mutex(){return M_main_mutex;}

void Contexts_Handler::move_main_context_to_here(){
	//lock_guard<recursive_mutex> lg(M_mutex);
	assert(0);
	if(M_active_contexts.count(M_main_context)){
		std::remove_if(make_pair_iter(M_contexts_by_thread_id.begin()),
			make_pair_iter(M_contexts_by_thread_id.end())
			, bind(std::equal_to<J_Context_Shared_t>(), _1, M_main_context));
	}
	M_active_contexts.insert(M_main_context);
	M_contexts_by_thread_id[this_thread::get_id()] = M_main_context;
	glfwMakeContextCurrent(M_main_context->M_window);
}

void Contexts_Handler::lock_main_context(){M_main_mutex.lock();}

void Contexts_Handler::unlock_main_context(){M_main_mutex.unlock();}

J_Context_Shared_t Contexts_Handler::create_j_context(int i_width, int i_height
													  , const char* const i_title
													  , j_monitor_t i_monitor
													  , j_window_t i_share_window){
	glfwWindowHint(GLFW_VISIBLE, false);

	J_Context_Shared_t new_context(new J_Context);
	M_main_context = M_main_context ? M_main_context : new_context;
	Context_RAII context_saver;
	new_context->M_width = i_width;
	new_context->M_height = i_height;

	if(!(new_context->M_window = glfwCreateWindow(i_width, i_height, i_title, i_monitor, i_share_window))){
		
		throw J_OpenGL_Init_Error("Could not creat)e glfw window!");
	}
	lock_guard<recursive_mutex> lg(M_mutex);
	glfwMakeContextCurrent(new_context->M_window);
	glClearColor(0.1f,0.0f,0.4f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(new_context->M_window);
	new_context->M_context = unique_ptr<GLEWContext>(new GLEWContext());

	M_active_contexts.insert(new_context);
	assert(!M_contexts_by_thread_id.count(this_thread::get_id()));


	M_contexts_by_thread_id[this_thread::get_id()] = new_context;
	M_contexts.insert(new_context);

	
	if(int error_code = glewInit()){
		cerr << "\nError Code: " << error_code << endl;
		throw J_OpenGL_Init_Error("Could Not Initialize Glew!");
	}
	return new_context;
}

j_window_t Contexts_Handler::get_window(J_Context_Shared_t i_context){
	return i_context->M_window;
}

void j_open_window(J_Context_Shared_t i_context){
	glfwShowWindow(i_context->M_window);
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

void j_view_port(int i_x, int i_y, int i_width, int i_height){
	glViewport(i_x, i_y, i_width, i_height);
}

void j_free_glfw(){
	glfwTerminate();
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

/*j_uint load_vertex_shader(const string&)*/
j_uint load_vertex_shader(const std::string& irk_filename){
	string vert_shader_str(get_string_from_file(irk_filename));

	cerr << '\n' << irk_filename << "\nVertex Shader Src:\n\n" << vert_shader_str << endl;
	
	j_uint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);

	const char* shader_cstr = vert_shader_str.data();
	glShaderSource(vertex_shader_id, 1, &shader_cstr, '\0');
	glCompileShader(vertex_shader_id);

#if J_GL_DEBUG > 0
	enforce_shader_status(vertex_shader_id, GL_COMPILE_STATUS);
#endif
	return vertex_shader_id;
}

/*j_uint load_fragment_Shader(const string&)*/
j_uint load_fragment_shader(const std::string& irk_filename){
	string frag_shader_str(get_string_from_file(irk_filename));

	cerr << '\n' << irk_filename << " Fragment Shader Src:\n\n" << frag_shader_str << endl;
	
	j_uint frag_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

	const char* shader_cstr = frag_shader_str.data();
	glShaderSource(frag_shader_id, 1, &shader_cstr, '\0');
	glCompileShader(frag_shader_id);

#if J_GL_DEBUG > 0
	enforce_shader_status(frag_shader_id, GL_COMPILE_STATUS);
#endif
	return frag_shader_id;
}

/*void enforce_chader_status(GLenum)*/
void enforce_shader_status(j_uint i_shader_id, GLenum i_status){
	GLint shader_status;
	glGetShaderiv(i_shader_id, i_status, &shader_status);

	if(shader_status){
		return;
	}

    GLsizei log_len;
    glGetShaderiv(i_shader_id, GL_INFO_LOG_LENGTH, &log_len );
    string log;
	log.resize(log_len+1);
    glGetShaderInfoLog(i_shader_id, log_len, &log_len, &log[0]);
    std::cerr << "\nShader Status Check Fail:\n" << log << std::endl;
    throw J_OpenGL_Init_Error("Shader Status Check Fail!");

}

/*void enforce_program_status(j_uint, GLenum)*/
void enforce_program_status(j_uint i_program_id, GLenum i_status){
	GLint program_status;
	glGetProgramiv(i_program_id, i_status, &program_status);

	if(program_status){
		return;
	}

    GLsizei log_len;
    glGetProgramiv(i_program_id, GL_INFO_LOG_LENGTH, &log_len );
    string log;
	log.resize(log_len+1);
    glGetProgramInfoLog(i_program_id, log_len, &log_len, &log[0]);
    std::cerr << "\nProgram Link Fail:\n" << log << std::endl;
    throw J_OpenGL_Init_Error("Could Not Link Program!");

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

/*j_float convert_x_coord(j_window_t, int)*/
j_float convert_x_coord(j_window_t i_window, int i_x_pixel){
	return static_cast<j_float>(2)*i_x_pixel/get_x_res(i_window) - 1.0f ;
}

/*j_float convert_y_coord(j_window_t, int)*/
j_float convert_y_coord(j_window_t i_window, int i_y_pixel){
	return 1.0f - static_cast<j_float>(2)*i_y_pixel/get_y_res(i_window);
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




}

extern "C" j_context_t glewGetContext(){
	assert(s_contexts_handler->get_active_context());
	return (s_contexts_handler->get_active_context()->M_context.get());
}


