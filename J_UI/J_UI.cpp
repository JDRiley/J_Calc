#include "J_UI.h"
//
#include <J_OpenGL.h>

namespace jomike{



J_UI& J_UI::get_instance(){
	static J_UI ui_instance;
	return ui_instance;
}

jomike::j_dbl J_UI::time()const{
	return M_time;
}

void J_UI::update(){
	M_time = j_get_time();
}

}



