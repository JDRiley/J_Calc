#include "j_declaration.h"
//
#include "Type_Syntax.h"

namespace jomike{



j_declaration::j_declaration(
	Type_Syntax* i_type_syntax, J_Symbol_Identifier* irk_name)
	:j_symbol(irk_name, i_type_syntax->symbol_type()){
	set_type_syntax(i_type_syntax);
}

j_declaration::j_declaration(const j_declaration& irk_src) : j_symbol(irk_src){

}

j_declaration::~j_declaration(){

}


}



