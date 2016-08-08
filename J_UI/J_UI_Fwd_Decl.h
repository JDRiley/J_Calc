#ifndef J_UI_FWD_DECL_H
#define J_UI_FWD_DECL_H

//
#include <J_Fwd_Decl.h>
//
#include <J_OpenGL_Fwd_Decl.h>

//Utilities
#include <memory>
#include <utility>


typedef struct FT_FaceRec_*  FT_Face;
typedef struct FT_LibraryRec_* FT_Library;



#pragma comment(lib, "J_UI" J_LIB_APPEND)

namespace jomike{
enum class UI_Object_Types{
	Line, Box, Text_Box_Object, Circle, Image_Pane, Multi_State_Text_Box
};

//copied from glfw change per library or build
const int J_LEFT_MOUSE_BUTTON = 0;
const int J_KEY_INTEGRAL = 105;
J_STRUCT_FWD_DECL(Bitmap_Metrics)
J_FWD_DECL(J_UI_Letter_Box)

J_FWD_DECL(J_UI_Line)
J_FWD_DECL(J_Line)
J_FWD_DECL(J_Shape)
J_FWD_DECL(J_Rectangle)
J_FWD_DECL(J_View)
J_FWD_DECL(J_Text_Box)
J_FWD_DECL(J_UI_Box)
J_FWD_DECL(J_UI_Object)
J_FWD_DECL(J_Display_Box)
J_FWD_DECL(J_Display_Object)
J_FWD_DECL(J_FT_Text_Display)
J_FWD_DECL(J_GL_Box_Shader)
J_FWD_DECL(J_GL_Circle_Shader)
J_FWD_DECL(J_GL_Shader_Program)
J_FWD_DECL(J_Text_Shader_Program)
J_FWD_DECL(J_UI_Model)
J_FWD_DECL(J_UI_String)
J_FWD_DECL(J_UI_Multi_String)
J_FWD_DECL(J_UI_Char)
J_FWD_DECL(Line_Input)
J_FWD_DECL(J_Camera_Manager)
J_FWD_DECL(J_Camera_View)
J_FWD_DECL(J_Camera_Stream)
J_FWD_DECL(J_Image_Pane)
J_FWD_DECL(J_Display_Image_Pane)
J_FWD_DECL(Image_Shader_Program)
J_FWD_DECL(J_Display_Circle)
J_FWD_DECL(J_UI_Circle)
J_FWD_DECL(J_Circle)
J_FWD_DECL(Multi_State_Text_Box)
J_FWD_DECL(J_Text_Box_Object)
J_FWD_DECL(J_FT_Text_Display_Object)
J_FWD_DECL(J_FT_Text_Multi_State_Display)
J_FWD_DECL(J_UI_Object_Update_Callback)
J_FWD_DECL(J_Cursor_Position)
J_FWD_DECL(J_UI_Line)
J_FWD_DECL(J_Display_Line)

template<typename Num_t>
struct J_Color_RGBA;
typedef J_Color_RGBA<j_float> J_UI_Color;

typedef class J_Font_Face__* J_Font_Face;

typedef int j_key_id_t;
#ifdef WIN32

/*! @name Key and button actions
 *  @{ */
/*! @brief The key or button was released.
 *  @ingroup input
 */
#define J_RELEASE                0
/*! @brief The key or button was pressed.
 *  @ingroup input
 */
#define J_PRESS                  1
/*! @brief The key was held down until it repeated.
 *  @ingroup input
 */
#define J_REPEAT                 2
/*! @} */

/*! @defgroup keys Keyboard keys
 *
 * These key codes are inspired by the *USB HID Usage Tables v1.12* (p. 53-60),
 * but re-arranged to map to 7-bit ASCII for printable keys (function keys are
 * put in the 256+ range).
 *
 * The naming of the key codes follow these rules:
 *  - The US keyboard layout is used
 *  - Names of printable alpha-numeric characters are used (e.g. "A", "R",
 *    "3", etc.)
 *  - For non-alphanumeric characters, Unicode:ish names are used (e.g.
 *    "COMMA", "LEFT_SQUARE_BRACKET", etc.). Note that some names do not
 *    correspond to the Unicode standard (usually for brevity)
 *  - Keys that lack a clear US mapping are named "WORLD_x"
 *  - For non-printable keys, custom names are used (e.g. "F4",
 *    "BACKSPACE", etc.)
 *
 *  @ingroup input
 *  @{
 */

/* The unknown key */
#define J_KEY_UNKNOWN            -1

/* Printable keys */
#define J_KEY_SPACE              32
#define J_KEY_APOSTROPHE         39  /* ' */
#define J_KEY_COMMA              44  /* , */
#define J_KEY_MINUS              45  /* - */
#define J_KEY_PERIOD             46  /* . */
#define J_KEY_SLASH              47  /* / */
#define J_KEY_0                  48
#define J_KEY_1                  49
#define J_KEY_2                  50
#define J_KEY_3                  51
#define J_KEY_4                  52
#define J_KEY_5                  53
#define J_KEY_6                  54
#define J_KEY_7                  55
#define J_KEY_8                  56
#define J_KEY_9                  57
#define J_KEY_SEMICOLON          59  /* ; */
#define J_KEY_EQUAL              61  /* = */
#define J_KEY_A                  65
#define J_KEY_B                  66
#define J_KEY_C                  67
#define J_KEY_D                  68
#define J_KEY_E                  69
#define J_KEY_F                  70
#define J_KEY_G                  71
#define J_KEY_H                  72
#define J_KEY_I                  73
#define J_KEY_J                  74
#define J_KEY_K                  75
#define J_KEY_L                  76
#define J_KEY_M                  77
#define J_KEY_N                  78
#define J_KEY_O                  79
#define J_KEY_P                  80
#define J_KEY_Q                  81
#define J_KEY_R                  82
#define J_KEY_S                  83
#define J_KEY_T                  84
#define J_KEY_U                  85
#define J_KEY_V                  86
#define J_KEY_W                  87
#define J_KEY_X                  88
#define J_KEY_Y                  89
#define J_KEY_Z                  90
#define J_KEY_LEFT_BRACKET       91  /* [ */
#define J_KEY_BACKSLASH          92  /* \ */
#define J_KEY_RIGHT_BRACKET      93  /* ] */
#define J_KEY_GRAVE_ACCENT       96  /* ` */
#define J_KEY_WORLD_1            161 /* non-US #1 */
#define J_KEY_WORLD_2            162 /* non-US #2 */

/* Function keys */
#define J_KEY_ESCAPE             256
#define J_KEY_ENTER              257
#define J_KEY_TAB                258
#define J_KEY_BACKSPACE          259
#define J_KEY_INSERT             260
#define J_KEY_DELETE             261
#define J_KEY_RIGHT              262
#define J_KEY_LEFT               263
#define J_KEY_DOWN               264
#define J_KEY_UP                 265
#define J_KEY_PAGE_UP            266
#define J_KEY_PAGE_DOWN          267
#define J_KEY_HOME               268
#define J_KEY_END                269
#define J_KEY_CAPS_LOCK          280
#define J_KEY_SCROLL_LOCK        281
#define J_KEY_NUM_LOCK           282
#define J_KEY_PRINT_SCREEN       283
#define J_KEY_PAUSE              284
#define J_KEY_F1                 290
#define J_KEY_F2                 291
#define J_KEY_F3                 292
#define J_KEY_F4                 293
#define J_KEY_F5                 294
#define J_KEY_F6                 295
#define J_KEY_F7                 296
#define J_KEY_F8                 297
#define J_KEY_F9                 298
#define J_KEY_F10                299
#define J_KEY_F11                300
#define J_KEY_F12                301
#define J_KEY_F13                302
#define J_KEY_F14                303
#define J_KEY_F15                304
#define J_KEY_F16                305
#define J_KEY_F17                306
#define J_KEY_F18                307
#define J_KEY_F19                308
#define J_KEY_F20                309
#define J_KEY_F21                310
#define J_KEY_F22                311
#define J_KEY_F23                312
#define J_KEY_F24                313
#define J_KEY_F25                314
#define J_KEY_KP_0               320
#define J_KEY_KP_1               321
#define J_KEY_KP_2               322
#define J_KEY_KP_3               323
#define J_KEY_KP_4               324
#define J_KEY_KP_5               325
#define J_KEY_KP_6               326
#define J_KEY_KP_7               327
#define J_KEY_KP_8               328
#define J_KEY_KP_9               329
#define J_KEY_KP_DECIMAL         330
#define J_KEY_KP_DIVIDE          331
#define J_KEY_KP_MULTIPLY        332
#define J_KEY_KP_SUBTRACT        333
#define J_KEY_KP_ADD             334
#define J_KEY_KP_ENTER           335
#define J_KEY_KP_EQUAL           336
#define J_KEY_LEFT_SHIFT         340
#define J_KEY_LEFT_CONTROL       341
#define J_KEY_LEFT_ALT           342
#define J_KEY_LEFT_SUPER         343
#define J_KEY_RIGHT_SHIFT        344
#define J_KEY_RIGHT_CONTROL      345
#define J_KEY_RIGHT_ALT          346
#define J_KEY_RIGHT_SUPER        347
#define J_KEY_MENU               348
#define J_KEY_LAST               J_KEY_MENU

/*! @} */

/*! @defgroup mods Modifier key flags
 *  @ingroup input
 *  @{ */

/*! @brief If this bit is set one or more Shift keys were held down.
 */
#define J_MOD_SHIFT           0x0001
/*! @brief If this bit is set one or more Control keys were held down.
 */
#define J_MOD_CONTROL         0x0002
/*! @brief If this bit is set one or more Alt keys were held down.
 */
#define J_MOD_ALT             0x0004
/*! @brief If this bit is set one or more Super keys were held down.
 */
#define J_MOD_SUPER           0x0008


#define J_MOUSE_BUTTON_1         0
#define J_MOUSE_BUTTON_2         1
#define J_MOUSE_BUTTON_3         2
#define J_MOUSE_BUTTON_4         3
#define J_MOUSE_BUTTON_5         4
#define J_MOUSE_BUTTON_6         5
#define J_MOUSE_BUTTON_7         6
#define J_MOUSE_BUTTON_8         7
/*JTL Addition*/
const j_key_id_t J_MOUSE_WHEEL_UP = 9;
const j_key_id_t J_MOUSE_WHEEL_DOWN = 10;
/*In JTL Addition*/

#define J_MOUSE_BUTTON_LAST      J_MOUSE_WHEEL_DOWN /*JTL Addition Original: J_MOUSE_BUTTON_8*/
#define J_MOUSE_BUTTON_LEFT      J_MOUSE_BUTTON_1
#define J_MOUSE_BUTTON_RIGHT     J_MOUSE_BUTTON_2

#define J_MOUSE_BUTTON_MIDDLE    J_MOUSE_BUTTON_3
/*! @} */

/*! @defgroup joysticks Joysticks
 *  @ingroup input
 *  @{ */
#define J_JOYSTICK_1             0
#define J_JOYSTICK_2             1
#define J_JOYSTICK_3             2
#define J_JOYSTICK_4             3
#define J_JOYSTICK_5             4
#define J_JOYSTICK_6             5
#define J_JOYSTICK_7             6
#define J_JOYSTICK_8             7
#define J_JOYSTICK_9             8
#define J_JOYSTICK_10            9
#define J_JOYSTICK_11            10
#define J_JOYSTICK_12            11
#define J_JOYSTICK_13            12
#define J_JOYSTICK_14            13
#define J_JOYSTICK_15            14
#define J_JOYSTICK_16            15
#define J_JOYSTICK_LAST          J_JOYSTICK_16

#endif




}

#endif