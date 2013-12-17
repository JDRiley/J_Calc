#ifndef J_OPENGL_FWD_DECL_H
#define J_OPENGL_FWD_DECL_H

#include <J_Fwd_Decl.h>

#pragma comment(lib, "J_OpenGL"J_LIB_APPEND)

typedef struct GLFWmonitor* j_monitor_t;
typedef struct GLEWContextStruct* j_context_t;
typedef struct GLFWwindow* j_window_t;


namespace jomike{
J_FWD_DECL(J_Context)
J_FWD_DECL(J_Frame_Counter)
}


#endif // !J_OPENGL_FWD_DECL_H




