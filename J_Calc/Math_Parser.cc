
//
#include "Math_Parser.h"
//
#include "Math_Lexer.h"
//
#include <sstream>
//
#include <iostream>
using std::stringstream;
namespace jomike{




token_t Math_Parser::lex(semantic_t* yylval){
	return M_lexer->yylex(yylval);
}

j_symbol_component* Math_Parser::parse(const std::string& irk_string){
	j_symbol_component* symbol = nullptr;
	yy::Math_Parsing_Unit parsing_unit(&symbol, this);
	stringstream str_stream(irk_string);
	M_lexer->yyrestart(&str_stream);
	parsing_unit.parse();
	return symbol;

}

Math_Parser::Math_Parser(){
	M_lexer = new Math_Lexer;
}

}

yy::Math_Parsing_Unit::token_type yylex(
	yy::Math_Parsing_Unit::semantic_type* yylval, jtl::Math_Parser* i_parser){
	return i_parser->lex(yylval);
}


void yy::Math_Parsing_Unit::error(const location_type& , const std::string& irk_string){
	std::cerr << "Error Encountered: " << irk_string << "\n";
}