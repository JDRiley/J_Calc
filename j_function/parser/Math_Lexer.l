/* File:  scanner.l
 * ----------------
 * Lex inupt file to generate the scanner for the compiler.
 */



%{

#include "Math_Lexer.h"
#include "../Constant_Symbol_Factory.h"
#include "parser.h" // for token codes, yylval
#include "Math_Lexer.h"
#include "../J_Symbol_Identifier.h"
#include <ex_array.h>
#include <iostream>
using std::cin; using std::cout; using std::cerr;

using namespace jomike;
#ifdef WIN32
#define TAB_SIZE 8
#else
#define TAB_SIZE 8
#endif


 
#define YY_USER_ACTION pre_action_set_up();
#define MANAGE_LOC *i_loc = M_location;
%}

/* States
 * ------
 * A little wrinkle on states is the COPY exclusive state which
 * I added to first match each line and copy it ot the list of lines
 * read before re-processing it. This allows us to print the entire
 * line later to provide context on errors.
 */
%s N
%x COPY COMM
%option stack
%option yyclass="jomike::Math_Lexer" c++ noyywrap

/* Definitions
 * -----------
 * To make our rules more readable, we establish some definitions here.
 */

DIGIT             ([0-9])
HEX_DIGIT         ([0-9a-fA-F])
HEX_INTEGER       (0[Xx]{HEX_DIGIT}+)
INTEGER           ({DIGIT}+)
EXPONENT          ([Ee][-+]?{INTEGER})
DOUBLE            ({INTEGER}"."{DIGIT}*{EXPONENT}?)
BEG_STRING        (\"[^"\n]*)
STRING            ({BEG_STRING}\")
IDENTIFIER        ([a-zA-Z][a-zA-Z_0-9]*)
OPERATOR          ([-+/*%=.,;!<>()[\]{}\?\:])
BEG_COMMENT       ("/*")
END_COMMENT       ("*/")
SINGLE_COMMENT    ("//"[^\n]*)

%%
             /* BEGIN RULES SECTION */

<COPY>.*               { //char curLine[512];
                         //strncpy(curLine, yytext, sizeof(curLine));
                         M_lines.push_back(yytext);
                         M_current_collumn = 1; yy_pop_state(); yyless(0); }
<COPY><<EOF>>          { yy_pop_state();}
<*>\n                  { M_current_line++; M_current_collumn = 1;                          
						if (YYSTATE == COPY) M_lines.push_back("");
                         else yy_push_state(COPY); }

[ ]+                { /* ignore all spaces */  }
<*>[\t]                { M_current_collumn += (TAB_SIZE - (M_current_collumn - 1) % TAB_SIZE) % TAB_SIZE; }

 /* -------------------- Comments ----------------------------- */
{BEG_COMMENT}          { BEGIN(COMM); }
<COMM>{END_COMMENT}    { BEGIN(N); }
<COMM><<EOF>>          { assert(0); 
                         return static_cast<token_t>(0); }
<COMM>.                { /* ignore everything else that doesn't match */ }
{SINGLE_COMMENT}       { /* skip to end of line for // comment */ }



 /* --------------------- Keywords ------------------------------- */
"int"				{MANAGE_LOC  return token_t::T_INT;		}
"double"|"dbl"			{MANAGE_LOC  return token_t::T_DOUBLE;		}
"bool"				{MANAGE_LOC  return token_t::T_BOOL;		}
"string"			{MANAGE_LOC  return token_t::T_STRING;		}
"null_ptr"			{MANAGE_LOC  return token_t::T_NULL_PTR;        }			
"void"|"Void"			{MANAGE_LOC  return token_t::T_VOID;		}
"transfer"|"Transfer"		{MANAGE_LOC  return token_t::T_TRANSFER;	}
"for"|"For"			{MANAGE_LOC  return token_t::T_FOR;		}


 /* -------------------- Operators ----------------------------- */
"<="                { MANAGE_LOC return token_t::T_LESS_EQUAL;   }
">="                { MANAGE_LOC return token_t::T_GREATER_EQUAL;}
"=="                { MANAGE_LOC return token_t::T_EQUAL;       }
"!="                { MANAGE_LOC return token_t::T_NOT_EQUAL;    }
"&&"                { MANAGE_LOC return token_t::T_AND;         }
"||"                { MANAGE_LOC return token_t::T_OR;          }
"<-"                { MANAGE_LOC return token_t::T_LEFT_ARROW;          }
"->"                { MANAGE_LOC return token_t::T_RIGHT_ARROW;          }
"++"                { MANAGE_LOC return token_t::T_INCREMENT;          }
"--"                { MANAGE_LOC return token_t::T_DECREMENT;          }





<<EOF>> {
	MANAGE_LOC 
	return token_t::T_END;
}

{OPERATOR}          {MANAGE_LOC  return static_cast<token_t>(yytext[0]);}
    




 /* -------------------- Constants ------------------------------ */
"true"|"false"      {yylval->constant_symbol
	= make_bool_constant_symbol('t' == yytext[0] ? true : false, M_location);
	MANAGE_LOC return token_t::T_BOOL_CONSTANT;
}


{INTEGER}           {
	int val = strtol(yytext, NULL, 10);
	yylval->constant_symbol
		= make_int_constant_symbol(
		val, M_location);
	MANAGE_LOC return token_t::T_INTEGER_CONSTANT;
}
{HEX_INTEGER}       {int val = strtol(yytext, NULL, 16);
	yylval->constant_symbol
		= make_int_constant_symbol(
		val, M_location);
	MANAGE_LOC return token_t::T_INTEGER_CONSTANT;}
{DOUBLE}            {j_dbl val = atof(yytext);
	yylval->constant_symbol
		= make_double_constant_symbol(val, M_location);
	MANAGE_LOC return token_t::T_DOUBLE_CONSTANT;}

{STRING}            {
	std::string string_val(yytext+1);
	assert('"' == string_val.back());
	string_val.pop_back();
	yylval->constant_symbol = make_string_constant_symbol(string_val.c_str(), M_location);
	MANAGE_LOC return token_t::T_STRING_CONSTANT;
}
{BEG_STRING}        {MANAGE_LOC throw J_Syntax_Error("Unterminaned String");}


 /* -------------------- Identifiers --------------------------- */
{IDENTIFIER}        {yylval->identifier = new J_Symbol_Identifier(J_UI_String(yytext));
						MANAGE_LOC return token_t::T_IDENTIFIER; }


 /* -------------------- Default rule (error) -------------------- */
.                   { std::cerr << "Unrecognized Character";MANAGE_LOC  return token_t(0);}

%%


void jomike::Math_Lexer::init_scanner(){
    yy_flex_debug = false;
    BEGIN(N);
    yy_push_state(COPY); // copy first line at start
    M_current_line = 1;
    M_current_collumn = 1;
}