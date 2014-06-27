#ifndef J_UI_H
#define J_UI_H

#include "J_UI_Fwd_Decl.h"


namespace jomike{
class J_UI{
public:
	static J_UI& get_instance();
	void update();
	j_dbl time()const;
private:
	J_UI();
	j_dbl M_time = 0.0f;
};

}

#endif //J_UI_H


