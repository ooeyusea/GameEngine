#ifndef _INPUT_H_
#define _INPUT_H_
#include <unordered_set>
#include <glm/glm.hpp>
#include <list>
#include <functional>
#include <unordered_map>

namespace lly {
	class Input
	{
	public:
		enum class Key {
			UNKNOWN,
			SPACE, APOSTROPHE, COMMA, MINUS, PERIOD, SLASH,
			KEY_0, KEY_1, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, SEMICOLON, EQUAL,
			A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
			LEFT_BRACKET, BACKSLASH, RIGHT_BRACKET, GRAVE_ACCENT, WORLD_1, WORLD_2,
			ESCAPE, ENTER, TAB, BACKSPACE, INSERT, KEY_DELETE,
			RIGHT, LEFT, DOWN, UP,
			PAGE_UP, PAGE_DOWN, HOME, END, CAPS_LOCK, SCROLL_LOCK, NUM_LOCK, PRINT_SCREEN, PAUSE,
			F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
			KP_0, KP_1, KP_2, KP_3, KP_4, KP_5, KP_6, KP_7, KP_8, KP_9,
			KP_DECIMAL, KP_DIVIDE, KP_MULTIPLY, KP_SUBTRACT, KP_ADD, KP_ENTER, KP_EQUAL,
			LEFT_SHIFT, LEFT_CONTROL, LEFT_ALT, LEFT_SUPER, RIGHT_SHIFT, RIGHT_CONTROL, RIGHT_ALT, RIGHT_SUPER,
			MENU,
			MBUTTON_1, MBUTTON_2, MBUTTON_3, MBUTTON_4, MBUTTON_5, MBUTTON_6, MBUTTON_7, MBUTTON_8,
		};

		Input() {}
		~Input() {}

		void on_key_press(Key key) { _key_state.insert(key); }
		void on_key_release(Key key) { _key_state.erase(key); }
		bool get_key(Key key) { return _key_state.find(key) != _key_state.end(); }

		void on_mouse_move(float x, float y) { _mouse_position = glm::vec2(x, y); }
		void on_mouse_scroll(float x, float y);
		void add_listener(int id, std::function<void(float x, float y)> f);
		void remove_listener(int id);

	private:
		std::unordered_set<Key> _key_state;
		glm::vec2 _mouse_position;
		std::unordered_map<int, std::function<void(float x, float y)>> _scroll_listeners;
	};

	Input::Key to_key_from_glfw(int key);
}

#endif //_INPUT_H_
