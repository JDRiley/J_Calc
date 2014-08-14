#ifndef SYMBOL_TYPES_H
#define SYMBOL_TYPES_H

//
#include <string>

namespace jomike{
enum class Symbol_Types{ 
	INT, BOOL, DOUBLE, STRING, NAMED, ARRAY, VOID_TYPE, THIS_PTR, NULL_TYPE, EMPTY, ERROR_TYPE, PROGRAM, VARIABLE_DECLARATION
	, PLACEHOLDER, FUNCTION_DECLARATION, CLASS_DECLARATION, INTERFACE_DECLARATION, PROTOTYPE_DECLARATION
	, STATEMENT_BLOCK, STATEMENT, IF_STATEMENT, WHILE_STATEMENT, FOR_STATEMENT
	, COMPOSITE, BREAK_STATEMENT, RETURN_STATEMENT, PRINT_STATEMENT
	, EXPRESSION_TYPE_UNINITIALIZED, ARGUMENTS, ROUTINE, LIST, END };



template<typename St>
struct CPP_To_Symbol_Type{};

template<>
struct CPP_To_Symbol_Type<j_dbl>{
	static Symbol_Types type(){ return Symbol_Types::DOUBLE; }
};

template<>
struct CPP_To_Symbol_Type<int>{
	static Symbol_Types type(){ return Symbol_Types::INT; }
};

template<>
struct CPP_To_Symbol_Type<long long>{
	static Symbol_Types type(){ return Symbol_Types::INT; }
};

template<>
struct CPP_To_Symbol_Type<bool>{
	static Symbol_Types type(){ return Symbol_Types::BOOL; }
};

template<>
struct CPP_To_Symbol_Type<std::string>{
	static Symbol_Types type(){ return Symbol_Types::STRING; }
};

template<>
struct CPP_To_Symbol_Type<const std::string>{
	static Symbol_Types type(){ return Symbol_Types::STRING; }
};

template<>
struct CPP_To_Symbol_Type<const std::string&>{
	static Symbol_Types type(){ return Symbol_Types::STRING; }
};


}



#endif //SYMBOL_TYPES_H


