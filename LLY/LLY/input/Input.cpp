#include "Input.h"
#include <GLFW/glfw3.h>

namespace lly {
	void Input::on_mouse_scroll(float x, float y)
	{
		for (auto& listener : _scroll_listeners)
			listener.second(x, y);
	}

	void Input::add_listener(int id, std::function<void(float x, float y)> f)
	{
		_scroll_listeners[id] = f;
	}

	void Input::remove_listener(int id)
	{
		_scroll_listeners.erase(id);
	}

	Input::Key to_key_from_glfw(int key)
	{
		switch (key)
		{
		case GLFW_KEY_SPACE: return Input::Key::SPACE;
		case GLFW_KEY_APOSTROPHE: return Input::Key::APOSTROPHE;
		case GLFW_KEY_COMMA: return Input::Key::COMMA;
		case GLFW_KEY_MINUS: return Input::Key::MINUS;
		case GLFW_KEY_PERIOD: return Input::Key::PERIOD;
		case GLFW_KEY_SLASH: return Input::Key::SLASH;
		case GLFW_KEY_0: return Input::Key::KEY_0;
		case GLFW_KEY_1: return Input::Key::KEY_1;
		case GLFW_KEY_2: return Input::Key::KEY_2;
		case GLFW_KEY_3: return Input::Key::KEY_3;
		case GLFW_KEY_4: return Input::Key::KEY_4;
		case GLFW_KEY_5: return Input::Key::KEY_5;
		case GLFW_KEY_6: return Input::Key::KEY_6;
		case GLFW_KEY_7: return Input::Key::KEY_7;
		case GLFW_KEY_8: return Input::Key::KEY_8;
		case GLFW_KEY_9: return Input::Key::KEY_9;
		case GLFW_KEY_SEMICOLON: return Input::Key::SEMICOLON;
		case GLFW_KEY_EQUAL: return Input::Key::EQUAL;
		case GLFW_KEY_A: return Input::Key::A;
		case GLFW_KEY_B: return Input::Key::B;
		case GLFW_KEY_C: return Input::Key::C;
		case GLFW_KEY_D: return Input::Key::D;
		case GLFW_KEY_E: return Input::Key::E;
		case GLFW_KEY_F: return Input::Key::F;
		case GLFW_KEY_G: return Input::Key::G;
		case GLFW_KEY_H: return Input::Key::H;
		case GLFW_KEY_I: return Input::Key::I;
		case GLFW_KEY_J: return Input::Key::J;
		case GLFW_KEY_K: return Input::Key::K;
		case GLFW_KEY_L: return Input::Key::L;
		case GLFW_KEY_M: return Input::Key::M;
		case GLFW_KEY_N: return Input::Key::N;
		case GLFW_KEY_O: return Input::Key::O;
		case GLFW_KEY_P: return Input::Key::P;
		case GLFW_KEY_Q: return Input::Key::Q;
		case GLFW_KEY_R: return Input::Key::R;
		case GLFW_KEY_S: return Input::Key::S;
		case GLFW_KEY_T: return Input::Key::T;
		case GLFW_KEY_U: return Input::Key::U;
		case GLFW_KEY_V: return Input::Key::V;
		case GLFW_KEY_W: return Input::Key::W;
		case GLFW_KEY_X: return Input::Key::X;
		case GLFW_KEY_Y: return Input::Key::Y;
		case GLFW_KEY_Z: return Input::Key::Z;
		case GLFW_KEY_LEFT_BRACKET: return Input::Key::LEFT_BRACKET;
		case GLFW_KEY_BACKSLASH: return Input::Key::BACKSLASH;
		case GLFW_KEY_RIGHT_BRACKET: return Input::Key::RIGHT_BRACKET;
		case GLFW_KEY_GRAVE_ACCENT: return Input::Key::GRAVE_ACCENT;
		case GLFW_KEY_WORLD_1: return Input::Key::WORLD_1;
		case GLFW_KEY_WORLD_2: return Input::Key::WORLD_2;
		case GLFW_KEY_ESCAPE: return Input::Key::ESCAPE;
		case GLFW_KEY_ENTER: return Input::Key::ENTER;
		case GLFW_KEY_TAB: return Input::Key::TAB;
		case GLFW_KEY_BACKSPACE: return Input::Key::BACKSPACE;
		case GLFW_KEY_INSERT: return Input::Key::INSERT;
		case GLFW_KEY_DELETE: return Input::Key::KEY_DELETE;
		case GLFW_KEY_RIGHT: return Input::Key::RIGHT;
		case GLFW_KEY_LEFT: return Input::Key::LEFT;
		case GLFW_KEY_DOWN: return Input::Key::DOWN;
		case GLFW_KEY_UP: return Input::Key::UP;
		case GLFW_KEY_PAGE_UP: return Input::Key::PAGE_UP;
		case GLFW_KEY_PAGE_DOWN: return Input::Key::PAGE_DOWN;
		case GLFW_KEY_HOME: return Input::Key::HOME;
		case GLFW_KEY_END: return Input::Key::END;
		case GLFW_KEY_CAPS_LOCK: return Input::Key::CAPS_LOCK;
		case GLFW_KEY_SCROLL_LOCK: return Input::Key::SCROLL_LOCK;
		case GLFW_KEY_NUM_LOCK: return Input::Key::NUM_LOCK;
		case GLFW_KEY_PRINT_SCREEN: return Input::Key::PRINT_SCREEN;
		case GLFW_KEY_PAUSE: return Input::Key::PAUSE;
		case GLFW_KEY_F1: return Input::Key::F1;
		case GLFW_KEY_F2: return Input::Key::F2;
		case GLFW_KEY_F3: return Input::Key::F3;
		case GLFW_KEY_F4: return Input::Key::F4;
		case GLFW_KEY_F5: return Input::Key::F5;
		case GLFW_KEY_F6: return Input::Key::F6;
		case GLFW_KEY_F7: return Input::Key::F7;
		case GLFW_KEY_F8: return Input::Key::F9;
		case GLFW_KEY_F9: return Input::Key::F9;
		case GLFW_KEY_F10: return Input::Key::F10;
		case GLFW_KEY_F11: return Input::Key::F11;
		case GLFW_KEY_F12: return Input::Key::F12;
		case GLFW_KEY_KP_0: return Input::Key::KP_0;
		case GLFW_KEY_KP_1: return Input::Key::KP_1;
		case GLFW_KEY_KP_2: return Input::Key::KP_2;
		case GLFW_KEY_KP_3: return Input::Key::KP_3;
		case GLFW_KEY_KP_4: return Input::Key::KP_4;
		case GLFW_KEY_KP_5: return Input::Key::KP_5;
		case GLFW_KEY_KP_6: return Input::Key::KP_6;
		case GLFW_KEY_KP_7: return Input::Key::KP_7;
		case GLFW_KEY_KP_8: return Input::Key::KP_8;
		case GLFW_KEY_KP_9: return Input::Key::KP_9;
		case GLFW_KEY_KP_DECIMAL: return Input::Key::KP_DECIMAL;
		case GLFW_KEY_KP_DIVIDE: return Input::Key::KP_DIVIDE;
		case GLFW_KEY_KP_MULTIPLY: return Input::Key::KP_MULTIPLY;
		case GLFW_KEY_KP_SUBTRACT: return Input::Key::KP_SUBTRACT;
		case GLFW_KEY_KP_ADD: return Input::Key::KP_ADD;
		case GLFW_KEY_KP_ENTER: return Input::Key::KP_ENTER;
		case GLFW_KEY_KP_EQUAL: return Input::Key::KP_EQUAL;
		case GLFW_KEY_LEFT_SHIFT: return Input::Key::LEFT_SHIFT;
		case GLFW_KEY_LEFT_CONTROL: return Input::Key::LEFT_CONTROL;
		case GLFW_KEY_LEFT_ALT: return Input::Key::LEFT_ALT;
		case GLFW_KEY_LEFT_SUPER: return Input::Key::LEFT_SUPER;
		case GLFW_KEY_RIGHT_SHIFT: return Input::Key::RIGHT_SHIFT;
		case GLFW_KEY_RIGHT_CONTROL: return Input::Key::RIGHT_CONTROL;
		case GLFW_KEY_RIGHT_ALT: return Input::Key::RIGHT_ALT;
		case GLFW_KEY_RIGHT_SUPER: return Input::Key::RIGHT_SUPER;
		case GLFW_KEY_MENU: return Input::Key::MENU;
		case GLFW_MOUSE_BUTTON_1: return Input::Key::MBUTTON_1;
		case GLFW_MOUSE_BUTTON_2: return Input::Key::MBUTTON_2;
		case GLFW_MOUSE_BUTTON_3: return Input::Key::MBUTTON_3;
		case GLFW_MOUSE_BUTTON_4: return Input::Key::MBUTTON_4;
		case GLFW_MOUSE_BUTTON_5: return Input::Key::MBUTTON_5;
		case GLFW_MOUSE_BUTTON_6: return Input::Key::MBUTTON_6;
		case GLFW_MOUSE_BUTTON_7: return Input::Key::MBUTTON_7;
		case GLFW_MOUSE_BUTTON_8: return Input::Key::MBUTTON_8;
		default: return Input::Key::UNKNOWN;
		}
	}
}
