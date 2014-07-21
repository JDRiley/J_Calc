#include "J_Test_Suite.h"
//
#ifndef J_UI_STRING_DEBUG
#define J_UI_STRING_DEBUG
#endif
#include <J_UI/J_UI_String.h>
//
#include <J_UI/J_UI_Multi_String.h>
//
#include <J_UI/J_Font_Manager.h>

namespace jomike{
static Instance_Pointer<J_Font_Manager> s_font_manager;

void J_Test_Suite::test_ui_string(){
	


	J_UI_String consolas_string_black(M_consolas_font);
	J_UI_String times_string_black(M_times_font);

	assert(consolas_string_black.empty());
	assert(times_string_black.empty());
	consolas_string_black.push_back('a');
	for(int i = 0; i < 15; ++i){
		j_size_t original_size = consolas_string_black.size();
		(void)original_size;
		consolas_string_black.append(consolas_string_black);
		assert(consolas_string_black.size() ==  original_size*2);
	}




	assert(consolas_string_black != times_string_black);








	
}

void J_Test_Suite::test_ui_multi_string(){
	J_UI_Multi_String multi_string;
	
	J_UI_String consolas_string_black(M_consolas_font);
	J_UI_String times_string_white(M_times_font, J_WHITE);


	for(int i = 0; i < 5; ++i){
		multi_string.insert(multi_string.begin(), consolas_string_black);
	}
	assert(multi_string.size() == 0);
	assert(multi_string.num_strings() == 1);

	multi_string.insert(multi_string.begin(), times_string_white);
	assert(multi_string.num_strings() == 2);
	assert(multi_string.front() == times_string_white);
	assert(times_string_white == multi_string.front());
	assert(consolas_string_black == multi_string.back());


	consolas_string_black.push_back('a');
	multi_string.pop_back();
	J_UI_Multi_String multi_string_2(consolas_string_black);
	for(int i = 0; i < 10; i++){
		j_size_t original_size = multi_string.size();
		(void)original_size;
		multi_string.insert(multi_string.begin(), multi_string_2);
		assert((multi_string_2.size() + original_size )== multi_string.size());

	}
	
	assert(times_string_white == multi_string.back());
	assert(consolas_string_black.is_same_type(multi_string.front()));

	times_string_white.push_back('t');
	times_string_white.push_back('t');
	multi_string_2 = J_UI_Multi_String(times_string_white);

	for(int i = 0; i < 10; ++i){
		j_size_t original_size = multi_string.size();
		(void)original_size;
		multi_string.insert(multi_string.end(), times_string_white);
		assert((multi_string_2.size() + original_size )== multi_string.size());

	}


	return;
}

J_Test_Suite::J_Test_Suite(){
	s_font_manager->add_font("Fonts/Consolas.ttf", "consolas");
	s_font_manager->add_font("Fonts/times.ttf", "times");
	M_consolas_font = s_font_manager->get_font_face("consolas", 12);
	M_times_font = s_font_manager->get_font_face("times", 12);
}

J_Test_Suite::~J_Test_Suite(){
	s_font_manager->delete_font_face(M_consolas_font);
	s_font_manager->delete_font_face(M_times_font);
}

}



