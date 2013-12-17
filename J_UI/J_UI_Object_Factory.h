#ifndef J_UI_OBJECT_FACTORY_H
#define J_UI_OBJECT_FACTORY_H

//
#include <J_Fwd_Decl.h>
//
#include "J_UI_Fwd_Decl.h"

namespace jomike{

J_FWD_DECL(J_Rectangle)

enum class UI_OBJECTS{Text_Box};

J_UI_Object* create_ui_object(UI_OBJECTS);


}

#endif