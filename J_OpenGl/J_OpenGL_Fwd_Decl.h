#ifndef J_OPENGL_FWD_DECL_H
#define J_OPENGL_FWD_DECL_H

#include <J_Fwd_Decl.h>

#pragma comment(lib, "J_OpenGL"J_LIB_APPEND)

typedef struct GLFWmonitor* j_monitor_t;
typedef struct GLEWContextStruct* j_context_t;
typedef struct GLFWwindow* j_window_t;


namespace jomike{

J_FWD_DECL(J_Context)
J_FWD_DECL(J_Open_GL)
J_FWD_DECL(J_Frame_Counter)
J_FWD_DECL(J_GL_Texture)
J_FWD_DECL(J_GL_Object)
J_FWD_DECL(J_GL_Texture_Render_Buffer)
J_FWD_DECL(J_GL_Vertex_Array)
J_FWD_DECL(J_GL_Framebuffer)
}


#endif // !J_OPENGL_FWD_DECL_H




