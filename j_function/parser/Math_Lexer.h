#ifndef MATH_LEXER_H
#define MATH_LEXER_H

#include "J_Fwd_Decl.h"
//
#include <FlexLexer.h>
//
#include "J_Symbol_Fwd_Decl.h"
//
#include <J_UI/J_UI_String.h>
//
#include "Math_Parser.h"
//
#include "Lexer_Location.h"
//
#include <ex_array.h>



typedef yy::Math_Parsing_Unit::token_type token_t;
typedef yy::Math_Parsing_Unit::semantic_type semantic_t;

#define YY_DECL		\
	token_t\
	Math_Lexer::yylex(semantic_t* yylval)





namespace jomike{
class Math_Parser;
struct Lexer_Location;
class Math_Lexer : public yyFlexLexer{
public:
	Math_Lexer();
	token_t yylex(semantic_t* yylval);
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


