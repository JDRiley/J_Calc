#ifndef J_TEST_SUITE_H
#define J_TEST_SUITE_H

#include "J_Fwd_Decl.h"
//
#include <J_UI/J_UI_Fwd_Decl.h>

namespace jomike{

class J_Test_Suite{
public:
	J_Test_Suite();
	void test_ui_string();
	void test_ui_multi_string();

	~J_Test_Suite();
private:
	J_Font_Face M_consolas_font;
	J_Font_Face M_times_font;
};
}

#endif //J_TEST_SUITE_H


