#pragma once

namespace fnx
{
	class animator
	{
		enum type
		{
			mouse_enter,
			mouse_exit,
			mouse_press,
			mouse_release,
			show,
			hide,
			activate,
			inactivate,
			max
		};

		struct status
		{
			transitions _transitions{};
			bool _ignore_mouse_input{ false };
			bool _hide_widget_on_finish{ false };
			bool _has_finished{ true };
			double _wait_to_finish{ 0.0 };
		};

	public:
		animator() = default;
		~animator() = default;

		/// @brief Plays the mouse enter animation in reverse when exiting the widget.
		void rewind_on_mouse_exit() { _rewind_on_exit = true; }

		/// @brief Plays the mouse press animation in reverse when releasing the mouse over the widget.
		void rewind_on_mouse_release() { _rewind_on_release = true; }

		/// @brief Plays the show animation in reverse when hiding the widget.
		void rewind_on_hide() { _rewind_on_hide = true; }

		/// @brief Plays the active animation in reverse when inactivating the widget.
		void rewind_on_inactive() { _rewind_on_inactive = true; }

		void add_on_mouse_enter_transitions(const transitions& transition) { _transition_map[type::mouse_enter]._transitions = transition; }
		void add_on_mouse_exit_transitions(const transitions& transition) { _transition_map[type::mouse_exit]._transitions = transition; }
		void add_on_mouse_press_transitions(const transitions& transition) { _transition_map[type::mouse_press]._transitions = transition; }
		void add_on_mouse_release_transitions(const transitions& transition) { _transition_map[type::mouse_release]._transitions = transition; }
		void add_on_show_transitions(const transitions& transition) { _transition_map[type::show]._transitions = transition; }
		void add_on_hide_transitions(const transitions& transition) { _transition_map[type::hide]._transitions = transition; }
		void add_on_active_transitions(const transitions& transition) { _transition_map[type::activate]._transitions = transition; }
		void add_on_inactive_transitions(const transitions& transition) { _transition_map[type::inactivate]._transitions = transition; }

		/// @brief Perform the on mouse enter animations.
		/// @return Duration in seconds of the animations.
		double on_mouse_enter() { return start(type::mouse_enter); };

		/// @brief Perform the on mouse exit animations.
		/// @return Duration in seconds of the animations.
		double on_mouse_exit() { return start(type::mouse_exit); };

		/// @brief Perform the on mouse press animations.
		/// @return Duration in seconds of the animations.
		double do_mouse_press() { return start(type::mouse_press); };

		/// @brief Perform the on mouse release animations.
		/// @return Duration in seconds of the animations.
		double do_mouse_release() { return start(type::mouse_release); };

		/// @brief Perform the on show animations.
		/// @return Duration in seconds of the animations.
		double on_show() { return start(type::show); };

		/// @brief Perform the on hide animations.
		/// @param[in] wait : minimum duration before making the widget invisible
		/// @return Duration in seconds of the animations.
		double on_hide(double wait = 0.0) { return start(type::hide, wait); };

		/// @brief Perform the on active animations.
		/// @return Duration in seconds of the animations.
		double do_active() { return start(type::activate); };

		/// @brief Perform the on in active animations.
		/// @return Duration in seconds of the animations.
		double do_inactive() { return start(type::inactivate); };

		/// @brief Call cyclically to update the animations.
		void update(double delta, widget& w);

	protected:
		bool _rewind_on_exit{ false };
		bool _rewind_on_hide{ false };
		bool _rewind_on_release{ false };
		bool _rewind_on_inactive{ false };

		std::array<status, type::max> _transition_map;

		/// @brief Start the animation.
		/// @return The duration of the transition
		double start(type, double wait = 0.0);

		/// @brief Complete any processing as the animation has finished.
		void finish(type, widget& w);

		double start(animator::type start, animator::type stop, bool rewind, bool ignore_io, bool hide_on_finish, double wait);
	};
}