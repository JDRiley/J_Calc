#include "../../J_Error/J_Error.h"
#include "J_Image.h"


using std::numeric_limits;
//Math
#include <cmath>
using std::log;using std::pow;using std::max;

//Utilities 
#include <cassert>

namespace jomike{



int next_power_of_two(int i_x){
	assert(i_x > 0);
	for(int  i=0 ; i < log(numeric_limits<int>::max()); i++){
		if(i_x <= (1 << i)){
			return (1 << i);
		}
	}
	throw J_Error("Next Power Of Two Did Not Find A Value");
}

j_int j_num_channels(Image_Format i_format){
	switch(i_format){
	case Image_Format::BGR24_UBYTE:
	case Image_Format::RGB24_UBYTE:
		return 3;
	case Image_Format::RGBA32_UBYTE:
		return 4;
	default:
		assert(!"Unhandled Image Format");
		return 0;
	}
}

const J_UI_Color& J_BLACK = J_Color::Black;
const J_UI_Color& J_WHITE = J_Color::White;
const J_UI_Color& J_CLEAR = J_Color::Clear;
const J_UI_Color& J_CYAN = J_Color::Cyan;
const J_UI_Color& J_YELLOW = J_Color::Yellow;
const J_UI_Color& J_RED = J_Color::Red;
const J_UI_Color& J_BLUE = J_Color::Blue;
const J_UI_Color& J_GREEN = J_Color::Green;


}
