#ifndef J_CALC_LEXER_LEXER_H
#define J_CALC_LEXER_LEXER_H

#include <J_Fwd_Decl.h>
//
#ifdef yyFlexLexer
#undef yyFlexLexer
#endif
#define yyFlexLexer jcFlexLexer
#include <j_symbol/FlexLexer.h>
//
#include <j_symbol/J_Symbol_Fwd_Decl.h>
//
#include "J_Calc_Parser.h"

//
#include <ex_array.h>
//
#include <J_String.h>

//
#include "../J_Calc_Symbol_Fwd_Decl.h"

typedef yy::J_Calc_Parsing_Unit::token_type token_t;
typedef yy::J_Calc_Parsing_Unit::semantic_type semantic_t;


#ifdef YY_DECL
#undef YY_DECL
#endif

#define YY_DECL		\
	token_t\
	J_Calc_Lexer::yylex(semantic_t* yylval, yy::location* i_loc, jtl::J_Calc_Parser* i_parser)





namespace jomike{
class J_Calc_Parser;

class J_Calc_Lexer : public yyFlexLexer{
public:
	J_Calc_Lexer(){
		init_scanner();
	}
	token_t yylex(semantic_t* yylval, yy::location* i_loc, jtl::J_Calc_Parser* i_parser);
	void init_scanner();
	/* Function: pre_action_set_up()
 	* ------------------------------
 	* This function is installed as the YY_USER_ACTION. This is a place
 	* to group code common to all actions.
 	* On each match, we fill in the fields to record its location and
 	* update our column counter.
 	*/
	void pre_action_set_up(){
	   M_location.begin.line = M_current_line;
	   M_location.begin.column = M_current_collumn;
	   M_location.end.column = M_current_collumn + yyleng - 1;
	   M_current_collumn += yyleng;
	
	   bool is_end_line = std::string("\n") == yytext;
	
	   int num_lines_to_grab = yylineno - M_current_line;
	   num_lines_to_grab = is_end_line  ? 0 : num_lines_to_grab;
	
	   M_push_to_line_flag = !is_end_line && M_push_to_line_flag;
	
	   if(M_push_to_line_flag && !num_lines_to_grab){
		   M_lines.back() += to_binary_wide_string<jc_string_t, std::string>(yytext);
	   }
	   
	   const int size = sizeof(wchar_t);
	
	   char* text_ptr = yytext;
	   auto find_next_end_line = [&text_ptr](){
		   while(*text_ptr && '\n' != *text_ptr){
			   ++text_ptr;
		   }
		   
		   ++text_ptr;
	   };
	
	   find_next_end_line();
	   for(int i = 0; i < num_lines_to_grab; ++i){
		   char* last_pos = text_ptr;
		   find_next_end_line();
		   if(0 == i && M_push_to_line_flag){
			   M_lines.back() += jc_string_t(last_pos, text_ptr);
		   }
		   else{
			   M_lines.emplace_back(last_pos, text_ptr);
		   }
	   }
	   M_push_to_line_flag = '\n' != *--text_ptr && (M_push_to_line_flag || (num_lines_to_grab > 0));
	   
	   M_current_line = yylineno;
	   M_location.end.line = M_current_line;
	}

	
	/* Function: GetLineNumbered()
 	* ---------------------------
 	* Returns string with contents of line numbered n or NULL if the
 	* contents of that line are not available.  Our scanner copies
 	* each line scanned and appends each to a list so we can later
 	* retrieve them to report the context for errors.
 	*/

	const jc_string_t& get_line(j_size_t i_line)const{
		assert(between_inclusive(i_line, J_SIZE_T_ZERO, M_lines.size() - 1));
		return M_lines[i_line];
	}
private:
	yy::J_Calc_Parsing_Unit::location_type M_location;
	yy_buffer_state* yy_current_buffer;

	ex_array<jc_string_t> M_lines;

	int M_current_line;
	int M_current_collumn;
	bool M_push_to_line_flag = false;

};





}

#endif //J_CALC_LEXER_LEXER_H


