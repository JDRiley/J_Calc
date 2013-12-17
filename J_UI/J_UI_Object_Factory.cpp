#include "J_UI_Object_Factory.h"

#include "J_FT_Text_Displayer/J_Text_Box.h"


namespace jomike{


//Creation---------------------------------------------------------------------------------------

J_Text_Box* create_text_box();

J_UI_Object* create_ui_object(UI_OBJECTS i_object_type){
	J_UI_Object* new_object;
	switch(i_object_type){
	case UI_OBJECTS::Text_Box:
		new_object = create_text_box();
	default:
		new_object = nullptr;
	}

	return new_object;
}








}