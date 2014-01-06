#ifndef BOOLEAN_TESTER_H
#define BOOLEAN_TESTER_H

#include "j_value.h"

namespace jomike{

class Boolean_Tester{
public:
	virtual bool eval(const j_value&)const;
	virtual bool eval(const j_value&, const j_value&)const;

	Boolean_Tester* get_copy()const;
private:

};

class equal_tester : public Boolean_Tester{
public:
	virtual bool eval(const j_value&, const j_value&)const;
private:
};

class not_equal_tester : public Boolean_Tester{
public:
	virtual bool eval(const j_value&, const j_value&)const;
private:
};



}

#endif //BOOLEAN_TESTER_H

