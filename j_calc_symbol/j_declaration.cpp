#include "j_declaration.h"
//
#include "Type_Syntax.h"

namespace jomike{



j_declaration::j_declaration(const j_declaration& irk_src) : j_statement(irk_src){

}

j_declaration::j_declaration(j_declaration&& irv_src): j_statement(std::move(irv_src)){

}



j_declaration::~j_declaration(){

}


}



