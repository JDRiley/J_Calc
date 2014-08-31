#include "j_statement.h"


namespace jomike{



j_statement::j_statement(){

}

j_statement::j_statement(J_Symbol_Identifier* i_identifier, Symbol_Types i_type)
	:j_symbol(i_identifier, i_type){

}

j_statement::j_statement(j_statement&& irv_source) : j_symbol(std::move(irv_source)){

}

j_statement::j_statement(const j_statement& irk_source) : j_symbol(irk_source){

}

}



