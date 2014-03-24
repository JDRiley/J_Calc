/* File:  scanner.l
 * ----------------
 * Lex inupt file to generate the scanner for the compiler.
 */



%{

#include "Math_Lexer.h"
#include <Constant_Symbol_Factory.h>
#include "parser.h" // for token codes, yylval
#include "Math_Lexer.h"
#include <ex_array.h>
#include <iostream>
using std::cin; using std::cout; using std::cerr;

using namespace jomike;
#ifdef WIN32
#define TAB_SIZE 8
#else
#define TAB_SIZE 8
#endif
/* Global variables
 * ----------------
 * (For shame!) But we need a few to keep track of things that are
 * preserved between calls to yylex or used outside the scanner.
 */
static int curLineNum, curColNum;
static ex_array<std::string> g_saved_lines;
 
#define YY_USER_ACTION pre_action_set_up();

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
                         g_saved_lines.push_back(yytext);
                         curColNum = 1; yy_pop_state(); yyless(0); }
<COPY><<EOF>>          { yy_pop_state();}
<*>\n                  { curLineNum++; curColNum = 1;                          
						if (YYSTATE == COPY) g_saved_lines.push_back("");
                         else yy_push_state(COPY); }

[ ]+                { /* ignore all spaces */  }
<*>[\t]                { curColNum += (TAB_SIZE - (curColNum - 1) % TAB_SIZE) % TAB_SIZE; }

 /* -------------------- Comments ----------------------------- */
{BEG_COMMENT}          { BEGIN(COMM); }
<COMM>{END_COMMENT}    { BEGIN(N); }
<COMM><<EOF>>          { assert(0);
                         return static_cast<token_t>(0); }
<COMM>.                { /* ignore everything else that doesn't match */ }
{SINGLE_COMMENT}       { /* skip to end of line for // comment */ }



 /* --------------------- Keywords ------------------------------- */
"int"				{ return token_t::T_INT;}
"double"|"dbl"			{ return token_t::T_DOUBLE;      }
"bool"				{ return token_t::T_BOOL;        }
"string"			{ return token_t::T_STRING;      }
"null_ptr"			{ return token_t::T_NULL_PTR;        }			





 /* -------------------- Operators ----------------------------- */
"<="                { return token_t::T_LESS_EQUAL;   }
">="                { return token_t::T_GREATER_EQUAL;}
"=="                { return token_t::T_EQUAL;       }
"!="                { return token_t::T_NOT_EQUAL;    }
"&&"                { return token_t::T_AND;         }
"||"                { return token_t::T_OR;          }






<<EOF>> {
	return static_cast<token_t>(0);
}

{OPERATOR}          { return static_cast<token_t>(yytext[0]);}
    
"[]"                { return token_t::T_DIMENSIONS;}



 /* -------------------- Constants ------------------------------ */
"true"|"false"      {yylval->constant_symbol
	= make_bool_constant_symbol('t' == yytext[0] ? true : false, M_location);
	return token_t::T_BOOL_CONSTANT;
}
{INTEGER}           {
	int val = strtol(yytext, NULL, 10);
	yylval->constant_symbol
		= make_int_constant_symbol(
		val, M_location);
	return token_t::T_INTEGER_CONSTANT;
}
{HEX_INTEGER}       {int val = strtol(yytext, NULL, 16);
	yylval->constant_symbol
		= make_int_constant_symbol(
		val, M_location);
	return token_t::T_INTEGER_CONSTANT;}
{DOUBLE}            {int val = atof(yytext);
	yylval->constant_symbol
		= make_double_constant_symbol(val, M_location);
	return token_t::T_DOUBLE_CONSTANT;}
{STRING}            {yylval->constant_symbol = make_string_constant_symbol(yytext, M_location);
	return token_t::T_STRING_CONSTANT;}
{BEG_STRING}        {assert(!"Unterminaned String");}


 /* -------------------- Identifiers --------------------------- */
{IDENTIFIER}        {yylval->identifier = new jomike::J_UI_String(yytext);
						return token_t::T_IDENTIFIER; }


 /* -------------------- Default rule (error) -------------------- */
.                   { assert(!"Unrecognized Character"); }

%%


void jomike::Math_Lexer::init_scanner(){
    yy_flex_debug = false;
    BEGIN(N);
    yy_push_state(COPY); // copy first line at start
    M_current_line = 1;
    M_current_collumn = 1;
}