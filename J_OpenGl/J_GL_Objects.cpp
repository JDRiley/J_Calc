#include "J_GL_Objects.h"
//
#include "J_Open_GL.h"


namespace jomike{
static J_Open_GL s_open_gl;

J_GL_Framebuffer::~J_GL_Framebuffer(){
	if(!get_ID()){
		return;
	}
	s_open_gl.delete_framebuffers(1, ID_ptr());
	
}


J_GL_Framebuffer::J_GL_Framebuffer():J_GL_Object(0){
	s_open_gl.gen_framebuffers(1, ID_ptr());

}

bool J_GL_Framebuffer::derived_valid()const {
	return s_open_gl.is_framebuffer(get_ID());
}


J_GL_Texture::J_GL_Texture(): J_GL_Object(0){
	s_open_gl.gen_textures(1, ID_ptr());

}

J_GL_Texture::~J_GL_Texture(){
	if(!get_ID()){
		return;
	}
	s_open_gl.delete_textures(1, ID_ptr());
}

bool J_GL_Texture::derived_valid()const {
	return s_open_gl.is_texture(get_ID());
}





void J_GL_Object::make_ID_zero(){
	M_ID = 0;
}


J_GL_Buffer::J_GL_Buffer():J_GL_Object(0){
	s_open_gl.gen_buffers(1, ID_ptr());

}

J_GL_Buffer::~J_GL_Buffer(){
	if(!get_ID()){
		return;
	}
	s_open_gl.delete_buffers(1, ID_ptr());
}

bool J_GL_Buffer::derived_valid()const {
	return s_open_gl.is_buffer(get_ID());
}


J_GL_Vertex_Array::J_GL_Vertex_Array(int i_index): J_GL_Object(i_index){
	assert(0 == i_index);
	assert(0 == get_ID());
}



J_GL_Vertex_Array::J_GL_Vertex_Array():J_GL_Object(0){
	s_open_gl.gen_vertex_arrays(1, ID_ptr());
	
	
}



J_GL_Vertex_Array::~J_GL_Vertex_Array(){
	if(!get_ID()){
		return;
	}

	s_open_gl.delete_vertex_arrays(1, ID_ptr());
}

bool J_GL_Vertex_Array::derived_valid()const {
	return s_open_gl.is_vertex_array(get_ID());
}

}



