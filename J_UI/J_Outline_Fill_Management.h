#ifndef J_OUTLINE_FILL_MANAGEMENT_H
#define J_OUTLINE_FILL_MANAGEMENT_H

#include "J_UI_Fwd_Decl.h"
//
#include "J_Image/J_Image.h"

namespace jomike{
class J_Outline_Fill_Management{
public:
#ifndef VS_2013
	J_Outline_Fill_Management();
#endif // !VS_2013

	

	bool outline_visibility_status()const;
	bool fill_visibility_status()const;
	j_float outline_thickness()const;

	virtual void set_outline_visibility_status(bool);
	virtual void set_fill_visibility_status(bool);
	virtual void set_outline_thickness(j_float);
	void set_outline_and_fill_visibility_status(bool);

	virtual void set_fill_color(const J_UI_Color& i_color);
	virtual void set_outline_color(const J_UI_Color& i_color);
	const J_UI_Color& fill_color()const;
	const J_UI_Color& outline_color()const;
private:

	j_float M_outline_thickness = 1.0f;
	bool M_outline_visibility_status = false;
	bool M_fill_visibility_status = false;
	J_UI_Color M_fill_color;
	J_UI_Color M_outline_color;

	virtual void derived_set_fill_color() = 0;
	virtual void derived_set_outline_color() = 0;
};

}

#endif //J_OUTLINE_FILL_MANAGEMENT_H


