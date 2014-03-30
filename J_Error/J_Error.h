#ifndef J_ERROR_H
#define J_ERROR_H
//
#include <J_Fwd_Decl.h>
//
#include <string>

#pragma comment(lib, "J_Utile"J_LIB_APPEND)

namespace jomike{};
namespace jtl = jomike;

namespace jomike{

class J_Error{
public:
	J_Error(const std::string& ik_message):M_msg(ik_message){}

	void print()const;
	const char* const message()const{return M_msg.c_str();}
	std::string str()const{ return M_msg; }
private:
	std::string M_msg;
	J_Error& operator=(const J_Error&);
};

class J_Init_Error: public J_Error{
public:
	J_Init_Error(const char* const ik_message):J_Error(ik_message){}
private:
	J_Init_Error& operator=(const J_Init_Error&);
};

class J_Argument_Error: public J_Error{
public:
	J_Argument_Error(const char* const ik_message):J_Error(ik_message){}
private:
	J_Argument_Error& operator=(const J_Argument_Error&);
};

class J_Finished_Exception: public J_Error{
public:
	J_Finished_Exception(const char* const ik_message = ""):J_Error(ik_message){}
private:
	J_Finished_Exception& operator=(const J_Argument_Error&);
};

class J_Syntax_Error : public J_Error{
public:
	J_Syntax_Error(const std::string& ik_message);
};


}

#endif