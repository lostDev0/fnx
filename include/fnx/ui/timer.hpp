#pragma once

#include <initializer_list>

namespace fnx
{
	/// @brief Timer consisting of on and off periods.
	class timer
	{
	public:
		timer(std::initializer_list<double> l);
		virtual ~timer() {}

		/// @brief Update timer with the delta between the last update.
		void update(double delta);

		/// @brief Start the timer from the beginning.
		void start();

		/// @brief Stop the timer.
		void stop();

		/// @brief Is the timer currently on or off.
		bool is_on();

		/// @brief Tell the time to restart automatically when if inishes all on/off periods.
		void set_repeating() { _repeating = true; }

	protected:
		std::vector<double> _times;		/// Periods of on, off, on, off, etc
		double _current_time{ 0.0 };	/// currently passed amount of time
		double _total{ 0.0 };			/// total time of the timer periods
		bool _repeating{ false };		/// is repeating
		bool _running{ false };			/// is running

		/// @brief Return where in the timer we are.
		/// @return true if in an on period, false otherwise
		bool get_time_slice();
	};
}