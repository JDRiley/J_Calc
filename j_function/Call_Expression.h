#ifndef CALL_EXPRESSION_H
#define CALL_EXPRESSION_H

#include "j_expression.h"


namespace jomike{
class Call_Expression : public j_expression{
public:

	Call_Expression(
		J_Symbol_Identifier* i_identifier, Arguments* i_args_list);


	Call_Expression(
		const j_expression& irk_base_expression
		, J_Symbol_Identifier* i_identifier, Arguments* i_args_list);




	Call_Expression* get_copy()const override{
		return new Call_Expression(*this);
	}

	Call_Expression* move_copy()override{
		return  new Call_Expression(std::move(*this));
	}

	J_UI_String get_display_name()override{
			return name();
	}

	bool has_value()const override;

	Call_Expression(const Call_Expression& irk_source);

	const Type_Syntax& return_type_syntax()const override;

	

	~Call_Expression();

	void process()override;
protected:
	j_symbol* get_symbol()const;
private:
	mutable j_expression* M_base_expression = nullptr;
	J_Symbol_Identifier* M_identifier;
	Arguments* M_args_list;
	void alert_symbol_scope_set()override;

	j_value derived_get_value(const Arguments& i_args)const override;


};

}

#endif //CALL_EXPRESSION_H


