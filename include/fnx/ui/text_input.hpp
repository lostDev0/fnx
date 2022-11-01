#pragma once

namespace fnx
{
	class text_input : public fnx::label
	{
	public:
		text_input(const std::string& font, const std::string& name = create_type_name<fnx::text_input>());
		~text_input() {}

		void set_minimum_size(size_t min) { _minimum = min; }
		void set_max_size(size_t max) { _maximum = max; }

		virtual void update(double delta) override;

		virtual void set_text(const std::string& label) override;

		void restrict_to_alpha()
		{
			_alpha_allowed = true;
			_numeric_allowed = false;
			_space_allowed = false;
			_all = false;
		}
		void restrict_to_numeric()
		{
			_alpha_allowed = false;
			_numeric_allowed = true;
			_space_allowed = false;
			_all = false;
		}
		void restrict_to_alpha_numeric()
		{
			_alpha_allowed = true;
			_numeric_allowed = true;
			_space_allowed = false;
			_all = false;
		}
		void allow_all()
		{
			_all = true;
		}
		void allow_space()
		{
			_space_allowed = true;
		}
	protected:
		fnx::timer _blinker{ 0.5, 0.5 };
		bool _is_on{ false };
		std::string _current_label;
		size_t _minimum{ 0u };
		size_t _maximum{ 0xffffffffu };
		size_t _current_pos{ 0u };
		fnx::vector2 _cursor_pos;
		bool _alpha_allowed{ true };
		bool _numeric_allowed{ true };
		bool _space_allowed{ true }; 
		bool _all{ true };

		void process_key(fnx::FNX_KEY key);

		virtual bool do_mouse_move(double x, double y) override;
		virtual bool do_mouse_press(FNX_BUTTON btn, double x, double y) override;
		virtual bool do_mouse_release(FNX_BUTTON btn, double x, double y) override;
		virtual bool do_key_press(FNX_KEY key) override;
		virtual bool do_key_release(FNX_KEY key) override;
		virtual bool do_key_repeat(FNX_KEY key) override;
		virtual void do_active() override;
		virtual void do_inactive() override;

		bool test_char(char c);
	};
}
