#ifndef J_IMAGE_H
#define J_IMAGE_H
#include "../J_UI_Fwd_Decl.h"

#include <limits>
//
//Algorithm
#include <algorithm>
//
#include <functional>

//Containers
#include <array>
//
#include <ex_array.h>


namespace jomike{
enum class Image_Format{ YUY2, BGR24_UBYTE, RGB24_UBYTE, RGBA32_UBYTE
	, NV12, DEPTH16, DEPTH_VERTICES};

j_int j_num_channels(Image_Format);

template<typename Num_t>
struct J_Color_RGBA{
	std::array<Num_t, 4> M_data;
	Num_t& red(){return M_data[0];}
	Num_t& green(){return M_data[1];}
	Num_t& blue(){return M_data[2];}
	Num_t& alpha(){return M_data[3];}
	const Num_t red()const{ return M_data[0]; }
	const Num_t green()const{ return M_data[1]; }
	const Num_t blue()const{ return M_data[2]; }
	const Num_t alpha()const{ return M_data[3]; }
	J_Color_RGBA<j_float>& alpha(j_float i_alpha){
		M_data[3] = i_alpha;
		return *this;
	}

	J_Color_RGBA<j_float> alpha(j_float i_alpha)const{
		J_Color_RGBA<j_float> new_color(*this);
		new_color.alpha() = i_alpha;
		return new_color;
	}

	J_Color_RGBA(){M_data.fill(0);}
	J_Color_RGBA(Num_t i_red, Num_t i_green, Num_t i_blue, Num_t i_alpha){
		red() = i_red;
		green() = i_green;
		blue() = i_blue;
		alpha() = i_alpha;
	}
	bool operator==(const J_Color_RGBA& irk_right)const{
		return std::equal(M_data.begin(), M_data.end(), irk_right.M_data.begin()
			, std::equal_to<j_float>());
	}
	J_Color_RGBA operator*(j_float i_multiply_val)const{
		J_Color_RGBA new_color;
		std::transform(M_data.begin(), --M_data.end(),
			new_color.M_data.begin(), std::bind(std::multiplies<j_float>(),
			std::placeholders::_1, i_multiply_val));
		new_color.alpha(alpha());
		return new_color;
	}
	J_Color_RGBA operator+(const J_Color_RGBA& i_color)const{
		J_Color_RGBA new_color;
		for (int i = 0; i < M_data.size(); i++){
			new_color.M_data[i] = M_data[i] + i_color.M_data[i];
		}
		return new_color;
	}

	bool operator<(const J_Color_RGBA& i_right)const{
		if(red() < i_right.red()){
			return true;
		}

		if(green() < i_right.green()){
			return true;
		}

		if(blue() < i_right.blue()){
			return true;
		}
		return alpha() < i_right.alpha();
	}
};



namespace J_Color{
	const J_Color_RGBA<j_float> White(1.0f,1.0f,1.0f,1.0f);
	const J_Color_RGBA<j_float> Clear(0.0f, 0.0f, 0.0f, 0.0f);
	const J_Color_RGBA<j_float> Yellow(1.0f, 1.0f, 0.0f, 1.0f);
	const J_Color_RGBA<j_float> Cyan(0.0f, 1.0f, 1.0f, 1.0f);
	const J_Color_RGBA<j_float> Blue(0.0f, 0.0f, 1.0f, 1.0f);
	const J_Color_RGBA<j_float> Black(0.0f,0.0f, 0.0f, 1.0f);
	const J_Color_RGBA<j_float> Red(1.0f, 0.0f, 0.0f, 1.0f);
	const J_Color_RGBA<j_float> Green(0.0f, 1.0f, 0.0f, 1.0f);
}

extern const J_UI_Color& J_BLACK;
extern const J_UI_Color& J_WHITE;
extern const J_UI_Color& J_CLEAR;
extern const J_UI_Color& J_CYAN;
extern const J_UI_Color& J_YELLOW;
extern const J_UI_Color& J_BLUE;
extern const J_UI_Color& J_RED; 
extern const J_UI_Color& J_GREEN;


template<typename Data_t, int M_Channels = 4>
class j_bitmap{
public:
	//Constructors
	j_bitmap(int i_width, int i_height):M_data(M_Channels*i_width*i_height)
		,M_width(i_width), M_height(i_height){}
	j_bitmap():M_width(0), M_height(0){}
	j_bitmap(int i_width, int i_height, J_Color_RGBA<j_float> i_color):M_data(M_Channels*i_width*i_height)
		,M_color(i_color), M_width(i_width), M_height(i_height){}
	
	
	//Info Functions
	int width()const{return M_width;}
	int height()const{return M_height;}

	//Mutation Functions
	void resize(int i_width, int i_height){
		M_width = i_width;
		M_height = i_height;
		M_data.resize(M_Channels*i_width*i_height);
	}
	//Data Access
	Data_t& at_index(size_t i_x, size_t i_y){return M_data[i_x*M_width*M_Channels + i_y];}
	const Data_t* data()const{return M_data.data();}
	const ex_array<Data_t>& get_array()const{ return M_data; }
private:
	ex_array<Data_t> M_data;
	J_Color_RGBA<j_float> M_color;
	int M_width;
	int M_height;
};

template<typename Num_t, typename Src_Num_t, int I_Channels>
void text_bitmap_formatter(j_bitmap<Num_t, 4>* i_dest_bitmap
					  , size_t i_width, size_t i_rows, Src_Num_t* i_bitmap, J_Color_RGBA<Num_t> i_color){
	
	size_t new_width = next_power_of_two(std::max(static_cast<size_t>(4),i_width)); //minimum pixel value is 4
	size_t new_height = i_rows;

	j_bitmap<Num_t, I_Channels>& expanded_bitmap = *i_dest_bitmap;
	expanded_bitmap.resize(new_width*I_Channels, new_height);

	for(size_t i=0; i < new_height; i++){
		for(size_t j=0; j < new_width*I_Channels; j+=I_Channels){
			if((j < i_width*I_Channels) && ( i < i_rows)){
				j_float byte_normalized_color_val = byte_normalized(i_bitmap[i*i_width + j]);
				for(int k=0 ; k < I_Channels; k++){
					expanded_bitmap.at_index(i,j+k) = i_color[k]*byte_normalized_color_val;
					}
			}else{
				expanded_bitmap.at_index(i,j) = expanded_bitmap.at_index(i,j+1) 
					= expanded_bitmap.at_index(i,j+2) = expanded_bitmap.at_index(i,j+3) 
					= 0;
			}
		}
	}
}

template<typename Num_t, typename Src_Num_t>
void text_bitmap_formatter(j_bitmap<Num_t, 1>* i_dest_bitmap
					  , int i_width, int i_rows, Src_Num_t* i_bitmap){
	
	int new_width = next_power_of_two(std::max(static_cast<int>(4),i_width)); //minimum pixel value is 4
	int new_height = next_power_of_two(std::max(static_cast<int>(4),i_rows));//i_rows;

	j_bitmap<j_ubyte, 1>& expanded_bitmap = *i_dest_bitmap;
	expanded_bitmap.resize(new_width, new_height);


	for(int i=0; i < new_height; i++){
		for(int j=0; j < new_width; j++){
			if((j < i_width) && ( i < i_rows)){
				//j_float byte_normalized_color_val = byte_normalized(i_bitmap[i*i_width + j]);
				//expanded_bitmap.at_index(i,j) = i_dest_max_val*byte_normalized_color_val;
				//reversing bitmap
				expanded_bitmap.at_index(i,j) = i_bitmap[i_width*(i_rows - i-1) +j];
			}else{
				expanded_bitmap.at_index(i,j) = 0;
			}
		}
	}
}


int next_power_of_two(int i_x);

template<typename int_t>
j_float byte_normalized(int_t i_val){
	int x = ((1 << (sizeof(i_val))*8) -1 );
	return static_cast<j_float>(i_val)/x;
}

const j_ubyte* convert_to_rgba32(int i_width, int i_height, const J_UI_Color& i_color, const j_ubyte* i_bitmap);


void convert_to_rgba32(j_ubyte_arr_t* ir_bitmap_arr, int i_width, int i_height, const J_UI_Color& i_color, const j_ubyte* i_bitmap);

}







#endif