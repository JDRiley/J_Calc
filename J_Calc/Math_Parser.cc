#include "Math_Parser.h"
//
#include "Math_Lexer.h"
//
#include <sstream>

//
#include <iostream>

 using std::cerr; using std::cout; using std::endl;
using std::stringstream;
namespace jomike{



token_t Math_Parser::lex(semantic_t* yylval){
	return M_lexer->yylex(yylval);
}

Math_Parser::Math_Parser(){
	M_lexer = new Math_Lexer;
}

int Math_Parser::parse(const std::string& irk_string){
	stringstream str_stream(irk_string);
	M_lexer->yyrestart(&str_stream);
	yy::Math_Parsing_Unit math_unit(this);
	return math_unit.parse();
}



}

void yy::Math_Parsing_Unit
::error(const location_type& , const std::string& irk_message){
	cerr << irk_message << endl;
}


yy::Math_Parsing_Unit::token_type yylex(
	yy::Math_Parsing_Unit::semantic_type* yylval, jtl::Math_Parser* i_parser){
	return i_parser->lex(yylval);
}



