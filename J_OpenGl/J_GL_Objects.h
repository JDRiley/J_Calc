#ifndef J_GL_OBJECTS_H
#define J_GL_OBJECTS_H

#include "J_OpenGL_Fwd_Decl.h"


namespace jomike{

class J_GL_Object{
public:
	J_GL_Object(j_uint i_id):M_ID(i_id){
		assert(M_ID >= 0);
	}

	J_GL_Object(J_GL_Object&& irv_right){
		std::swap(M_ID, irv_right.M_ID);
	}

	bool valid()const{
		if(!M_ID){
			return 0;
		}
		return derived_valid();
	}

	j_uint get_ID()const{
		return M_ID;

	}
protected:
	
	void make_ID_zero();
	j_uint* ID_ptr(){
		return &M_ID;
	}
private:
	j_uint M_ID;

	virtual bool derived_valid()const = 0;
};

class J_GL_Framebuffer : public J_GL_Object{
public:
	J_GL_Framebuffer();
	J_GL_Framebuffer(const J_GL_Framebuffer&) = delete;
	J_GL_Framebuffer(J_GL_Framebuffer&& irv_right):J_GL_Object(std::move(irv_right)){
		

	}

	static const J_GL_Framebuffer& null_object();

	~J_GL_Framebuffer();
private:
	friend class J_Open_GL;
	J_GL_Framebuffer(j_uint i_id):J_GL_Object(i_id){ assert(!get_ID()); }
	bool derived_valid()const override;
	
};


class J_GL_Texture : public J_GL_Object{
public:
	J_GL_Texture();

	J_GL_Texture(J_GL_Texture&& irv_right):J_GL_Object(std::move(irv_right)){}
	static const J_GL_Texture& null_object(){
		static J_GL_Texture null_texture(0u);
		return null_texture;
	}
	J_GL_Texture(const J_GL_Texture&) = delete;
	~J_GL_Texture();
private:
	friend class J_Open_GL;
	bool derived_valid()const override;
	J_GL_Texture(j_uint i_id):J_GL_Object(i_id){ assert(!get_ID()); }
};

class J_GL_Texture_Render_Buffer : public J_GL_Texture{
public:
	J_GL_Texture_Render_Buffer();
	J_GL_Texture_Render_Buffer(const J_GL_Texture&) = delete;
	J_GL_Texture_Render_Buffer(J_GL_Texture_Render_Buffer&& irv_right)
		:J_GL_Texture_Render_Buffer(std::move(irv_right)){}
	~J_GL_Texture_Render_Buffer();
private:
	friend class J_Open_GL;

	bool derived_valid()const override;

};

class J_GL_Vertex_Array : public J_GL_Object{
public:
	J_GL_Vertex_Array();

	J_GL_Vertex_Array(J_GL_Texture&& irv_right):J_GL_Object(std::move(irv_right)){}

	J_GL_Vertex_Array(const J_GL_Texture&) = delete;
	~J_GL_Vertex_Array();
private:
	friend class J_Open_GL;
	bool derived_valid()const override;
};

}

#endif //J_GL_OBJECTS_H


