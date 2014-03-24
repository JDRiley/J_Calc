#ifndef MATH_PARSER_H
#define MATH_PARSER_H

#include "J_Calc_Fwd_Decl.h"

//
#include "math_parsing_unit.hh"



typedef yy::Math_Parsing_Unit::token_type token_t;
typedef yy::Math_Parsing_Unit::semantic_type semantic_t;
yy::Math_Parsing_Unit::token_type
yylex(yy::Math_Parsing_Unit::semantic_type* yylval, jtl::Math_Parser* i_parser);

namespace jomike{
class Math_Parser{
public:
	Math_Parser();
	token_t lex(semantic_t* yylval);
	int parse(const std::string& irk_string);
private:
	Math_Lexer* M_lexer;
};



}

#endif //MATH_PARSER_H


