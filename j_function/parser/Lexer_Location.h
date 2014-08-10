#ifndef LEXER_LOCATION_H
#define LEXER_LOCATION_H

#include <J_Fwd_Decl.h>


namespace jomike{

struct Lexer_Location{
		int first_line;
		int first_column;
		int last_line;
		int last_column;
};

}

#endif //LEXER_LOCATION_H


