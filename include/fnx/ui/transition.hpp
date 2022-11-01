#pragma once

namespace fnx
{
	class widget;
	class transition
	{
	public:
		transition(double time) : _duration(time) {}
		virtual ~transition() = default;
		transition(const transition& other) = default;
		transition(transition&& other) = default;
		transition& operator=(const transition& other) = default;
		transition& operator=(transition&& other) = default;

		/// @brief Start the transition from the very beginning.
		void start(double delay = 0.0);

		/// @brief Stop the transition where ever it is in the transition.
		void pause();

		/// @brief Play the transition backwards.
		void rewind(double delay = 0.0);

		/// @brief Adjusts the delta to account for any delay or direction in the transition.
		double get_adjusted_delta(double delta);

		/// @brief Apply the time delta to the transition and update the widget accordingly
		virtual void apply(double delta_in_seconds, widget& w) {};

		/// @brief Has the transition completed.
		auto is_done() -> bool const { return _rewind ? _current_time <= 0.0 : _current_time >= _duration; }

		/// @brief Is the transition paused.
		auto is_paused() -> bool const { return _paused; }

		/// @brief Is the transition backwards incrementing.
		auto is_backward() -> bool const { return _rewind; }

		/// @brief Get the duration including any delay.
		auto get_duration() const { return _duration + _delay; }

		/// @brief Get the time remaining including any delay.
		auto get_remainder() -> double const
		{
			if (_rewind)
			{
				return fnx::maximum(_current_time + _delay, 0.0);
			}
			else
			{
				return fnx::maximum(_delay + (_duration - _current_time), 0.0);
			}
		}
		/// @brief Go to a specific time period
		auto seek_to(double time)
		{ 
			_current_time = fnx::clamp(time, 0.0, _current_time);
		}
		
		virtual std::unique_ptr<transition> clone()
		{
			return std::make_unique<transition>(*this);
		};

	protected:
		double _duration{ 0.0 };			/// total length of the animation without the delay
		double _delay{ 0.0 };				/// time left to wait before starting actual transition
		double _current_time{ 0.0 };		/// time position within the transition (no delay counted)
		bool _paused{ true };
		bool _rewind{ false };

		virtual void apply_self(double delta_in_seconds, widget& w) {};
	};

	class transitions
	{
	public:
		transitions() = default;
		~transitions() = default;

		void apply(double delta_in_seconds, widget& w);

		void set_delay(double delay_in_seconds) { _delay = delay_in_seconds; }
		auto get_delay() const { return _delay; }
		auto get_duration() const
		{
			double duration = 0.0;
			for (const auto& t : _transitions)
			{
				duration = fnx::maximum(duration, t->get_duration());
			}
			return duration;
		}

		auto get_remainder() const
		{
			double duration = 0.0;
			for (const auto& t : _transitions)
			{
				if(!t->is_done() && !t->is_paused())
					duration = fnx::maximum(duration, t->get_remainder());
			}
			return duration;
		}

		void play(bool rewind, bool delay = false);
		void pause();
		bool is_done();
		bool is_paused();
		bool is_backward() { return _rewind; }

		/// @brief Assumes the transition has completed and moves the transition to it's end state.
		void seek_to_end();

		void seek_to_start();

		template <typename... Ts>
		transitions(const Ts&... ts)
		{
			(add(ts), ...);
		}

		transitions(const transitions& other)
		{
			_delay = other._delay;
			_rewind = other._rewind;
			_transitions.reserve(other._transitions.size());
			for (const auto& e : other._transitions)
			{
				_transitions.push_back(e->clone());
			}
		}

		transitions& operator=(const transitions& other)
		{
			_delay = other._delay;
			_rewind = other._rewind;
			_transitions.clear();
			_transitions.reserve(other._transitions.size());
			for (const auto& e : other._transitions)
			{
				_transitions.push_back(e->clone());
			}
			return *this;
		}

		template<typename T, typename std::enable_if<std::is_base_of<fnx::transition, T>::value, T>::type * = nullptr>
		void add(const T& transition)
		{
			_transitions.push_back(std::make_unique<T>(transition));
		}

		bool is_empty() { return _transitions.empty(); }

	private:
		std::vector<std::unique_ptr<transition>> _transitions;
		double _delay{ 0.0 };
		bool _rewind{ false };
	};

	class slide_transition : public transition
	{
	public:
		slide_transition(fnx::vector2 min, fnx::vector2 max, double time)
			: transition(time)
		{
			_tween.add(min);
			_tween.add(max);
		}

		slide_transition(const tween<fnx::vector2>& tween, double time)
			: transition(time)
			, _tween(tween)
		{
		}

		virtual std::unique_ptr<transition> clone() override
		{
			return std::make_unique<slide_transition>(*this);
		};

		virtual ~slide_transition() = default;

		virtual void apply(double delta_in_seconds, widget& w) override;

	protected:
		tween<fnx::vector2> _tween;
	};

	class fade_transition : public transition
	{
	public:
		fade_transition(float min, float max, double time)
			: transition(time)
		{
			_tween.add(min);
			_tween.add(max);
		}

		fade_transition(const tween<float>& tween, double time)
			: transition(time)
			, _tween(tween)
		{
		}

		virtual std::unique_ptr<transition> clone() override
		{
			return std::make_unique<fade_transition>(*this);
		};

		virtual ~fade_transition() = default;

		virtual void apply(double delta_in_seconds, widget& w) override;

	protected:
		tween<float> _tween;
	};

	class size_transition : public transition
	{
	public:
		size_transition(fnx::vector2 min, fnx::vector2 max, double time)
			: transition(time)
		{
			_tween.add(min);
			_tween.add(max);
		}

		virtual std::unique_ptr<transition> clone() override
		{
			return std::make_unique<size_transition>(*this);
		};

		virtual ~size_transition() = default;

		virtual void apply(double delta_in_seconds, widget& w) override;

	protected:
		tween<fnx::vector2> _tween;
	};
}