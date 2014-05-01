#ifndef J_FT_TEXT_DISPLAY_H
#define J_FT_TEXT_DISPLAY_H
#include <J_Error.h>
#include <J_Image.h>
#include "../J_Display_Object/J_Display_Object.h"
#include "../J_UI_String/J_UI_String.h"
//Containers
#include <array>
#include <ex_array.h>

#include <utility>
//
#include "../J_Font_Manager.h"
#include <mutex>
namespace jomike{


class J_FT_Text_Display_Object : public J_Display_Box{
public:
	J_FT_Text_Display_Object(j_uint);

	virtual void delete_char(j_size_t i_pos) = 0;
	virtual void erase_chars(j_size_t i_start, j_size_t i_end) = 0;


	virtual void set_letter_box_rectangle(j_size_t i_pos
										  , const Pen_Pos_FL_t& i_pen_pos
										  , const Bitmap_Metrics& i_metrics) = 0;
	virtual void pop_back() = 0;


	virtual void add_letter_box(j_size_t i_index, const Pen_Pos_FL_t& i_pen_pos
								, const Bitmap_Metrics& i_metrics
								, const J_UI_Color& i_color
								, const j_ubyte* i_bitmap) = 0;


	virtual void set_letter_box_data(j_size_t i_index
							 , const Bitmap_Metrics& i_metrics, const J_UI_Color& i_color
							 , const j_ubyte* i_data) = 0;

	virtual void set_text_string_size(j_size_t i_size) = 0;

	virtual void set_letter_box_poses(j_size_t i_index, j_size_t i_size
									  , const Pen_Pos_FL_t* i_poses) = 0;
	virtual void insert_text_string(j_size_t i_pos
									, j_size_t i_size, const Pen_Pos_FL_t* i_poses
									, Bitmap_Metrics** i_metrics
									, const J_UI_Color& i_color
									, const j_ubyte* const * i_datas) = 0;
	~J_FT_Text_Display_Object();

protected:
	J_FT_Text_Display_Object(j_uint, j_uint);
private:
	
};

class J_FT_Text_Multi_State_Display : public J_FT_Text_Display_Object{
public:
	J_FT_Text_Multi_State_Display(j_uint);

	void delete_char(j_size_t i_pos)override;
	void erase_chars(j_size_t i_start, j_size_t i_end)override;

	void draw()const override;
	void alert_resize(int, int)override;
	void pop_back()override;


	void mouse_button_press(J_View_Shared_t, int i_button, int modifiers, Pen_Pos_FL_t)override;
	void mouse_button_release(J_View_Shared_t, int i_button, int modifiers, Pen_Pos_FL_t)override;

	void set_box(const J_Rectangle& irk_rec)override;

	void alert_cursor_pos(Pen_Pos_FL_t i_pos);


	void add_letter_box(j_size_t i_index, const Pen_Pos_FL_t& i_pos
						, const Bitmap_Metrics& i_metrics
						, const J_UI_Color& i_color, const j_ubyte* i_bitmap)override;

	void add_state();
	void set_state(j_size_t i_index);

	void set_fill_color(const J_UI_Color& i_color);
	void set_outline_color(const J_UI_Color& i_color);

	void set_fill_visibility_status(bool i_status);
	void set_outline_visibility_status(bool i_status);

	void set_outline_thickness(j_float)override;

	void set_letter_box_data(j_size_t i_index
							 , const Bitmap_Metrics& i_metrics, const J_UI_Color& i_color
							 , const j_ubyte* i_data)override;




	void set_letter_box_rectangle(j_size_t i_pos, const Pen_Pos_FL_t& i_pen_pos
								  , const Bitmap_Metrics& i_metrics)override;

	void set_text_string_size(j_size_t i_size)override;
	void set_letter_box_poses(j_size_t i_index, j_size_t i_size, const Pen_Pos_FL_t* i_poses)override;
	void insert_text_string(j_size_t i_pos, j_size_t i_size, const Pen_Pos_FL_t* i_poses, Bitmap_Metrics** i_metrics, const J_UI_Color& i_color, const j_ubyte* const * i_datas)override;
private:
	ex_array<J_FT_Text_Display_Shared_t> M_text_states;
	J_FT_Text_Display_Shared_t M_current_state;


};

class J_FT_Text_Display : public J_FT_Text_Display_Object{
public:
	J_FT_Text_Display(j_uint);
	J_FT_Text_Display(j_uint i_object_id, j_uint i_id);

	//void add_font_face(J_Font_Face);

	void add_letter_box(j_size_t i_index, const Pen_Pos_FL_t& i_pen_pos
						, const Bitmap_Metrics& i_metrics
						, const J_UI_Color& i_color
						, const j_ubyte* i_bitmap)override;



	void delete_char(j_size_t i_pos)override;
	void erase_chars(j_size_t i_start, j_size_t i_end)override;

	void draw()const override;

	void pop_back()override;
	void set_text_string_size(j_size_t i_size)override;
	void set_letter_box_data(j_size_t i_index
							 , const Bitmap_Metrics& i_metrics, const J_UI_Color& i_color
							 , const j_ubyte* i_data)override;

	//void mouse_button_press(J_View_Shared_t, int i_button, int modifiers, Pen_Pos_FL_t)override;
	//void mouse_button_release(J_View_Shared_t, int i_button, int modifiers, Pen_Pos_FL_t)override;

	void set_letter_box_rectangle(j_size_t i_pos, const Pen_Pos_FL_t& i_pen_pos
								  , const Bitmap_Metrics& i_metrics)override;

	void set_letter_box_poses(j_size_t i_index, j_size_t i_size, const Pen_Pos_FL_t* i_poses)override;
	void insert_text_string(j_size_t i_pos
							, j_size_t i_size, const Pen_Pos_FL_t* i_poses
							, Bitmap_Metrics** i_metrics
							, const J_UI_Color& i_color
							, const j_ubyte* const * i_datas)override;
	~J_FT_Text_Display();
protected:
	
private:
	//Render Settings
	void clear();
	void clear_from(j_size_t pos);
	mutable bool M_changed_flag = true;
	
	void alert_changed()const;
	
	class Modifier_Manger{
	public:
		typedef ex_array<J_Display_Letter_Box_Shared_t> Arr_t;
		
		Modifier_Manger(const std::function<void()>& irk_func);
		Arr_t& operator*(){
			M_notification_function();
			return M_letter_box_arr;
		}

		Arr_t::value_type& operator[](j_size_t i_index){
			return operator->()->operator[](i_index);
		}

		Arr_t* operator->(){
			M_notification_function();
			return &M_letter_box_arr;
		}

		const Arr_t* operator->()const{
			M_notification_function();
			return &M_letter_box_arr;
		}
	private:
		Arr_t M_letter_box_arr;
		std::function<void()> M_notification_function;
	};
	Modifier_Manger M_letter_box_string;
	j_uint M_frame_buffer_id;
	j_uint M_texture_buffer_id;
	void render_frame_buffer()const;
	j_uint x_pixels()const;
	j_uint y_pixels()const;
};





}

#endif