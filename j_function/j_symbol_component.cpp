#include "j_symbol_component.h"
//
#include <J_UI/J_UI_String.h>
//
#include "J_Symbol_Identifier.h"
namespace jomike{

static int g_symbol_ids = 0;

j_symbol_component::j_symbol_component(
	Symbol_Types i_symbol_type)
	:M_ID(++g_symbol_ids), M_type(i_symbol_type){


}


j_symbol_component::~j_symbol_component(){

}

jomike::Symbol_Types j_symbol_component::symbol_type()const{
	return M_type;
}

}



