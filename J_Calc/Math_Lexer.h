#ifndef MATH_LEXER_H
#define MATH_LEXER_H

#include "J_Fwd_Decl.h"
//
#include <FlexLexer.h>

namespace jomike{
class math_lexer : public yyFlexLexer{
public:
	math_lexer();
	int yylex();

private:
};

}

#endif //MATH_LEXER_H


