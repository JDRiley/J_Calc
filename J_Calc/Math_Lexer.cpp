#include "Math_Lexer.h"


namespace jomike{

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



/* Function: pre_action_set_up()
 * ------------------------------
 * This function is installed as the YY_USER_ACTION. This is a place
 * to group code common to all actions.
 * On each match, we fill in the fields to record its location and
 * update our column counter.
 */
void jomike::Math_Lexer::pre_action_set_up(){
   M_location.first_line = M_current_line;
   M_location.first_column = M_current_collumn;
   M_location.last_column = M_current_collumn + yyleng - 1;
   M_current_collumn += yyleng;
}

/* Function: GetLineNumbered()
 * ---------------------------
 * Returns string with contents of line numbered n or NULL if the
 * contents of that line are not available.  Our scanner copies
 * each line scanned and appends each to a list so we can later
 * retrieve them to report the context for errors.
 */



const std::string& Math_Lexer::get_line(j_size_t i_line)const{
	assert(between_inclusive(i_line, J_SIZE_T_ZERO, M_lines.size() - 1));
	return M_lines[i_line];
}

Math_Lexer::Math_Lexer(){
	init_scanner();
}

}



