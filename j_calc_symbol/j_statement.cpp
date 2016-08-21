#include "j_statement.h"


namespace jomike{






j_statement::j_statement(j_statement&& irv_source) : j_calc_symbol(std::move(irv_source)){

}

j_statement::j_statement(const j_statement& irk_source) : j_calc_symbol(irk_source){

}

}



