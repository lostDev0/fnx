#pragma once

namespace fnx
{
	/* mimics the GLFW keys */
	enum class FNX_KEY
	{
		/* The unknown key */
		FK_UNKNOWN          = -1, 

		/* Printable keys */
		FK_SPACE            = 32,
		FK_APOSTROPHE       = 39,  /* ' */
		FK_COMMA            = 44,  /* , */
		FK_MINUS            = 45,  /* - */
		FK_PERIOD           = 46,  /* . */
		FK_SLASH            = 47,  /* / */
		FK_0                = 48,
		FK_1                = 49,
		FK_2                = 50,
		FK_3                = 51,
		FK_4                = 52,
		FK_5                = 53,
		FK_6                = 54,
		FK_7                = 55,
		FK_8                = 56,
		FK_9                = 57,
		FK_SEMICOLON        = 59,  /* ; */
		FK_EQUAL            = 61,  /* = */
		FK_A                = 65,
		FK_B                = 66,
		FK_C                = 67,
		FK_D                = 68,
		FK_E                = 69,
		FK_F                = 70,
		FK_G                = 71,
		FK_H                = 72,
		FK_I                = 73,
		FK_J                = 74,
		FK_K                = 75,
		FK_L                = 76,
		FK_M                = 77,
		FK_N                = 78,
		FK_O                = 79,
		FK_P                = 80,
		FK_Q                = 81,
		FK_R                = 82,
		FK_S                = 83,
		FK_T                = 84,
		FK_U                = 85,
		FK_V                = 86,
		FK_W                = 87,
		FK_X                = 88,
		FK_Y                = 89,
		FK_Z                = 90,
		FK_LEFT_BRACKET     = 91,  /* [ */
		FK_BACKSLASH        = 92,  /* \ */
		FK_RIGHT_BRACKET    = 93,  /* ] */
		FK_GRAVE_ACCENT     = 96,  /* ` */
		FK_WORLD_1          = 161, /* non-US #1 */
		FK_WORLD_2          = 162, /* non-US #2 */

		/* Function keys */
		FK_ESCAPE           = 256,
		FK_ENTER            = 257,
		FK_TAB              = 258,
		FK_BACKSPACE        = 259,
		FK_INSERT           = 260,
		FK_DELETE           = 261,
		FK_RIGHT            = 262,
		FK_LEFT             = 263,
		FK_DOWN             = 264,
		FK_UP               = 265,
		FK_PAGE_UP          = 266,
		FK_PAGE_DOWN        = 267,
		FK_HOME             = 268,
		FK_END              = 269,
		FK_CAPS_LOCK        = 280,
		FK_SCROLL_LOCK      = 281,
		FK_NUM_LOCK         = 282,
		FK_PRINT_SCREEN     = 283,
		FK_PAUSE            = 284,
		FK_F1               = 290,
		FK_F2               = 291,
		FK_F3               = 292,
		FK_F4               = 293,
		FK_F5               = 294,
		FK_F6               = 295,
		FK_F7               = 296,
		FK_F8               = 297,
		FK_F9               = 298,
		FK_F10              = 299,
		FK_F11              = 300,
		FK_F12              = 301,
		FK_F13              = 302,
		FK_F14              = 303,
		FK_F15              = 304,
		FK_F16              = 305,
		FK_F17              = 306,
		FK_F18              = 307,
		FK_F19              = 308,
		FK_F20              = 309,
		FK_F21              = 310,
		FK_F22              = 311,
		FK_F23              = 312,
		FK_F24              = 313,
		FK_F25              = 314,
		FK_KP_0             = 320,
		FK_KP_1             = 321,
		FK_KP_2             = 322,
		FK_KP_3             = 323,
		FK_KP_4             = 324,
		FK_KP_5             = 325,
		FK_KP_6             = 326,
		FK_KP_7             = 327,
		FK_KP_8             = 328,
		FK_KP_9             = 329,
		FK_KP_DECIMAL       = 330,
		FK_KP_DIVIDE        = 331,
		FK_KP_MULTIPLY      = 332,
		FK_KP_SUBTRACT      = 333,
		FK_KP_ADD           = 334,
		FK_KP_ENTER         = 335,
		FK_KP_EQUAL         = 336,
		FK_LEFT_SHIFT       = 340,
		FK_LEFT_CONTROL     = 341,
		FK_LEFT_ALT         = 342,
		FK_LEFT_SUPER       = 343,
		FK_RIGHT_SHIFT      = 344,
		FK_RIGHT_CONTROL    = 345,
		FK_RIGHT_ALT        = 346,
		FK_RIGHT_SUPER      = 347,
		FK_MENU             = 348,

		FK_LAST             = FK_MENU,
		FK_FIRST            = FK_SPACE
	};

	enum class FNX_BUTTON
	{
		BT_UNKNOWN = -1,
		BT_MOUSE_LEFT = 0,
		BT_MOUSE_RIGHT = 1,
		BT_MOUSE_MIDDLE = 2
	};

	/// @brief Button map for an attached controller
	/// @todo Implement joystick controllers
	enum class FNX_JOYSTICK
	{

	};

	extern char key_to_ascii(fnx::FNX_KEY key);

	struct FNX_KEY_HASH
	{
		template<typename T>
		auto operator()(T t) const
		{
			return static_cast<int>(t);
		}
	};
}
