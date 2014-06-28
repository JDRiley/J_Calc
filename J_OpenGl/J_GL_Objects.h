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
		swap(irv_right);
	}
	J_GL_Object& operator=(J_GL_Object&& irv_right){
		swap(irv_right);
		return *this;
	}

	
	J_GL_Object(const J_GL_Object&) = delete;
	
	J_GL_Object& operator=(const J_GL_Object& ) = delete;
	

	
	~J_GL_Object(){}

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

	virtual void swap(J_GL_Object& ir_object){
		std::swap(M_ID, ir_object.M_ID);
	}

private:
	j_uint M_ID;

	virtual bool derived_valid()const = 0;
	
};

class J_GL_Framebuffer : public J_GL_Object{
public:
	J_GL_Framebuffer();
	J_GL_Framebuffer(const J_GL_Framebuffer&) = delete;
	J_GL_Framebuffer(J_GL_Framebuffer&& irv_right)
		:J_GL_Object(std::move(irv_right)){}
	
	J_GL_Framebuffer& operator=(J_GL_Framebuffer&& irv_right){
		swap(irv_right);
		return *this;
	}
	
	J_GL_Framebuffer& operator=(const J_GL_Framebuffer& ) = delete;
	
	static const J_GL_Framebuffer& null_object(){
		static J_GL_Framebuffer null_obj(0);
		return null_obj;
	}
	
	~J_GL_Framebuffer();
private:
	friend class J_Open_GL;
	J_GL_Framebuffer(j_uint i_id):J_GL_Object(i_id){ assert(!get_ID()); }
	bool derived_valid()const override;
	
};


class J_GL_Texture : public J_GL_Object{
public:
	J_GL_Texture();
	J_GL_Texture(const J_GL_Texture&) = delete;
	J_GL_Texture(J_GL_Texture&& irv_right)
		:J_GL_Object(std::move(irv_right)){}
	
	J_GL_Texture& operator=(J_GL_Texture&& irv_right){
		swap(irv_right);
		return *this;
	}
	
	J_GL_Texture& operator=(const J_GL_Texture& ) = delete;
	
	static const J_GL_Texture& null_object(){
		static J_GL_Texture null_obj(0);
		return null_obj;
	}
	
	~J_GL_Texture();
protected:
	J_GL_Texture(j_uint i_id):J_GL_Object(i_id){ assert(!get_ID()); }
private:
	friend class J_Open_GL;
	bool derived_valid()const override;
	
};






class J_GL_Texture_Render_Buffer : public J_GL_Texture{
public:
	J_GL_Texture_Render_Buffer();
	J_GL_Texture_Render_Buffer(const J_GL_Texture_Render_Buffer&) = delete;
	J_GL_Texture_Render_Buffer(J_GL_Texture_Render_Buffer&& irv_right)
		:J_GL_Texture(std::move(irv_right)){}
	
	J_GL_Texture_Render_Buffer& operator=(J_GL_Texture_Render_Buffer&& irv_right){
		swap(irv_right);
		return *this;
	}
	
	J_GL_Texture_Render_Buffer& operator=(const J_GL_Texture_Render_Buffer& ) = delete;
	
	static const J_GL_Texture_Render_Buffer& null_object(){
		static J_GL_Texture_Render_Buffer null_obj(0);
		return null_obj;
	}
	
	~J_GL_Texture_Render_Buffer();
private:
	friend class J_Open_GL;
	J_GL_Texture_Render_Buffer(int i_id):J_GL_Texture(i_id){ assert(!i_id); }
	bool derived_valid()const override;

};

class J_GL_Vertex_Array : public J_GL_Object{
public:
	J_GL_Vertex_Array();
	J_GL_Vertex_Array(const J_GL_Vertex_Array&) = delete;
	J_GL_Vertex_Array(J_GL_Vertex_Array&& irv_right)
		:J_GL_Object(std::move(irv_right)){}
	
	J_GL_Vertex_Array& operator=(J_GL_Vertex_Array&& irv_right){
		swap(irv_right);
		return *this;
	}
	
	J_GL_Vertex_Array& operator=(const J_GL_Vertex_Array& ) = delete;
	
	static const J_GL_Vertex_Array& null_object(){
		static J_GL_Vertex_Array null_obj;
		return null_obj;
	}
	
	~J_GL_Vertex_Array();
	J_GL_Vertex_Array(int i_index);
private:
	friend class J_Open_GL;
	bool derived_valid()const override;
	
};


class J_GL_Buffer : public J_GL_Object{
public:
	
	J_GL_Buffer();
	J_GL_Buffer(const J_GL_Buffer&) = delete;
	J_GL_Buffer(J_GL_Buffer&& irv_right)
		:J_GL_Object(std::move(irv_right)){}
	
	J_GL_Buffer& operator=(J_GL_Buffer&& irv_right){
		swap(irv_right);
		return *this;
	}
	
	J_GL_Buffer& operator=(const J_GL_Buffer& ) = delete;
	
	static const J_GL_Buffer& null_object(){
		static J_GL_Buffer null_obj;
		return null_obj;
	}
	
	~J_GL_Buffer();

	J_GL_Buffer(j_uint i_id):J_GL_Object(i_id){
		//Only to make null object
		assert(!get_ID());
	}
private:
	friend class J_Open_GL;
	bool derived_valid()const override;
	
};



}

#endif //J_GL_OBJECTS_H


