#ifndef MATH_LEXER_H
#define MATH_LEXER_H

#include "J_Fwd_Decl.h"
//
#include <FlexLexer.h>
//
#include <J_Symbol_Fwd_Decl.h>
//
#include <J_UI/J_UI_String.h>
//
#include "math_parsing_unit.hh"
//
#include "Lexer_Location.h"
//
#include <ex_array.h>


#define YY_DECL		\
	yy::Math_Parsing_Unit::token_type\
	Math_Lexer::yylex(yy::Math_Parsing_Unit::semantic_type* yylval)




typedef yy::Math_Parsing_Unit::token_type token_t;

namespace jomike{
class Math_Parser;
struct Lexer_Location;
class Math_Lexer : public yyFlexLexer{
public:
	Math_Lexer();
	yy::Math_Parsing_Unit::token_type
		Math_Lexer::yylex(yy::Math_Parsing_Unit::semantic_type* yylval);
	void init_scanner();
	void pre_action_set_up();
	const std::string& get_line(j_size_t i_line)const;
private:
	Lexer_Location M_location;
	yy_buffer_state* yy_current_buffer;

	ex_array<std::string> M_lines;

	int M_current_line;
	int M_current_collumn;


};

}

#endif //MATH_LEXER_H


