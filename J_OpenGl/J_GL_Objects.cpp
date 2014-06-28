#include "J_GL_Objects.h"
//
#include "J_Open_GL.h"
//
#include "J_OpenGL.h"
//
#include <gl\glew.h>

namespace jomike{
static J_Open_GL s_open_gl;
J_GL_Framebuffer::~J_GL_Framebuffer(){
	if(!get_ID()){
		return;
	}
	glDeleteFramebuffers(1, ID_ptr());
	assert(!open_gl_error());
}


J_GL_Framebuffer::J_GL_Framebuffer():J_GL_Object(0){
	glGenFramebuffers(1, ID_ptr());
	assert(!open_gl_error());
}

bool J_GL_Framebuffer::derived_valid()const {
	return glIsFramebuffer(get_ID());
}


J_GL_Texture::J_GL_Texture(): J_GL_Object(0){
	glGenTextures(1, ID_ptr());
	assert(!open_gl_error());
}

J_GL_Texture::~J_GL_Texture(){
	if(!get_ID()){
		return;
	}
	glDeleteTextures(1, ID_ptr());
	assert(!open_gl_error());
}

bool J_GL_Texture::derived_valid()const {
	return glIsTexture(get_ID());
}


J_GL_Texture_Render_Buffer::J_GL_Texture_Render_Buffer(){

}

J_GL_Texture_Render_Buffer::~J_GL_Texture_Render_Buffer(){
	if(!get_ID()){
		return;
	}

	glDeleteRenderbuffers(1, ID_ptr());
	assert(!open_gl_error());

	make_ID_zero();
}

bool J_GL_Texture_Render_Buffer::derived_valid()const {
	return glIsRenderbuffer(get_ID());
}


void J_GL_Object::make_ID_zero(){
	M_ID = 0;
}


J_GL_Buffer::J_GL_Buffer():J_GL_Object(0){
	glGenBuffers(1, ID_ptr());
	assert(!open_gl_error());
}

J_GL_Buffer::~J_GL_Buffer(){
	if(!get_ID()){
		return;
	}
	glDeleteBuffers(1, ID_ptr());
	assert(!open_gl_error());
}

bool J_GL_Buffer::derived_valid()const {
	return glIsBuffer(get_ID());
}


J_GL_Vertex_Array::J_GL_Vertex_Array(int i_index): J_GL_Object(i_index){
	assert(0 == i_index);
	assert(0 == get_ID());
}



J_GL_Vertex_Array::J_GL_Vertex_Array():J_GL_Object(0){
	glGenVertexArrays(1, ID_ptr());
	
	
	assert(!open_gl_error());
}



J_GL_Vertex_Array::~J_GL_Vertex_Array(){
	if(!get_ID()){
		return;
	}
	glDeleteVertexArrays(1, ID_ptr());
	assert(!open_gl_error());
}

bool J_GL_Vertex_Array::derived_valid()const {
	return glIsVertexArray(get_ID());
}

}



