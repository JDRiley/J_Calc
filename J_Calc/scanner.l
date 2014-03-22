/* File:  scanner.l
 * ----------------
 * Lex inupt file to generate the scanner for the compiler.
 */



%{

#include "scanner.h"
#include "parser.h" // for token codes, yylval
#include "Math_Lexer.h"
#include <ex_array.h>



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

static void DoBeforeEachAction(); 
#define YY_USER_ACTION DoBeforeEachAction();

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
%option yyclass="jomike::math_lexer"

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
                         return 0; }
<COMM>.                { /* ignore everything else that doesn't match */ }
{SINGLE_COMMENT}       { /* skip to end of line for // comment */ }



 /* --------------------- Keywords ------------------------------- */
"int"				{ return T_INT;}
"double"|"dbl"		{ return T_DOUBLE;      }
"bool"				{ return T_BOOL;        }
"string"			{ return T_STRING;      }
"null_ptr"			{ return T_NULL_PTR;        }			





 /* -------------------- Operators ----------------------------- */
"<="                { return T_LESS_EQUAL;   }
">="                { return T_GREATER_EQUAL;}
"=="                { return T_EQUAL;       }
"!="                { return T_NOT_EQUAL;    }
"&&"                { return T_AND;         }
"||"                { return T_OR;          }



<<EOF>> {
	yylex_destroy();
	return 0;
}

{OPERATOR}          { return yytext[0];}
    
"[]"                { return T_DIMENSION;}



 /* -------------------- Constants ------------------------------ */
"true" | "false"      {yylval.symbol_component*
		= make_bool_symbol("t" == yytext[0] ? true : false, yylloc);
	return T_BOOL_CONSTANT;}
{INTEGER}           {	int val = strtol(yytext, NULL, 10);
	yylval.symbol_component
		= make_int_constant_expression(
		val, yylloc);
	return T_INTEGER_CONSTANT;}
{HEX_INTEGER}       {int val = strtol(yytext, NULL, 16);
	yylval.symbol_component
		= make_int_constant_expression(
		val, yylloc);
	return T_INTEGER_CONSTANT;}
{DOUBLE}            {int val = atof(yytext);
	yylval.symbol_component
		= make_double_number_symbol(val, yylloc);
	return T_DOUBLE_CONSTANT;}
{STRING}            {yylval.identifier = new J_UI_String(yytext);
	return T_STRING_CONSTANT;}
{BEG_STRING}        {assert(!"Unterminaned String");}


 /* -------------------- Identifiers --------------------------- */
{IDENTIFIER}        {yylval.identifier = new jomike::J_UI_String(yytext);
						return T_IDENTIFIER; }

 /* -------------------- Default rule (error) -------------------- */
.                   { assert(!"Unrecognized Character"); }

%%

/* Function: InitScanner
 * ---------------------
 * This function will be called before any calls to yylex().  It is designed
 * to give you an opportunity to do anything that must be done to initialize
 * the scanner (set global variables, configure starting state, etc.). One
 * thing it already does for you is assign the value of the global variable
 * yy_flex_debug that controls whether flex prints debugging information
 * about each token and what rule was matched. If set to false, no information
 * is printed. Setting it to true will give you a running trail that might
 * be helpful when debugging your scanner. Please be sure the variable is
 * set to false when submitting your final version.
 */
void init_scanner(){
    PrintDebug("lex", "Initializing scanner");
    yy_flex_debug = false;
    BEGIN(N);
    yy_push_state(COPY); // copy first line at start
    curLineNum = 1;
    curColNum = 1;
}

const char *GetLineNumbered(int num) {
   if (num <= 0 || num > g_saved_lines.size()) return NULL;
   return g_saved_lines[num-1].c_str();
}

static void DoBeforeEachAction(){
   yylloc.first_line = curLineNum;
   yylloc.first_column = curColNum;
   yylloc.last_column = curColNum + yyleng - 1;
   curColNum += yyleng;
}
